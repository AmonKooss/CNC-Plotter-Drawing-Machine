/*
  AGGE DRAW - firmware do ESP32
  - Access Point proprio + captive portal
  - Serve a pagina embutida no proprio firmware (index_html.h), sem precisar enviar arquivos
  - POST /api/draw  : recebe [{w, p:[[x,y],...]}] em mm (A4), gera G-code e plota
  - GET  /api/status: {"busy":bool,"homing":bool,"pct":n[,"erro":"..."]}
  - Fala com o GRBL pela Serial2 (D26 = RX, D27 = TX), esperando o "ok" de cada linha
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <LittleFS.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "index_html.h"   // pagina da interface (gerada a partir do index.html)

// ===================== Configuracoes =====================

// Rede propria do ESP32 (aberta, como na documentacao)
const char* AP_SSID = "AGGE-DRAW";

// Serial com o GRBL: D26 = RX, D27 = TX
#define RX2 26
#define TX2 27
#define GRBL_BAUD 115200

// Area util
const float A4_LARGURA = 210.0;
const float A4_ALTURA  = 297.0;

// Curso da maquina, conforme o GRBL_Config: $130 (X), $131 (Y) e $27 (folga do homing)
const float CURSO_X    = 250.0;
const float CURSO_Y    = 300.0;
const float FOLGA_HOME = 5.0;
// Area realmente utilizavel: nunca passa do curso da maquina
const float LIM_X = (A4_LARGURA < CURSO_X - FOLGA_HOME) ? A4_LARGURA : (CURSO_X - FOLGA_HOME);
const float LIM_Y = (A4_ALTURA  < CURSO_Y - FOLGA_HOME) ? A4_ALTURA  : (CURSO_Y - FOLGA_HOME);

// O firmware executa $H e define G92 X0 Y0 automaticamente na primeira
// impressao apos ligar/reiniciar ou quando a referencia deixa de ser valida.

// O navegador manda Y crescendo para baixo. Se o (0,0) da maquina fica no
// canto INFERIOR esquerdo do papel, deixe true. Se fica no SUPERIOR esquerdo, false.
const bool INVERTER_Y = true;

// Velocidade de desenho (mm/min). Os deslocamentos G0 usam o $110/$111 do GRBL.
const int FEED_DESENHO = 3000;

// Caneta com servo no GRBL. AJUSTE conforme a sua maquina (teste com /api/pen?pos=up|down).
const char* PEN_DOWN  = "M5";  // caneta toca o papel
const char* PEN_UP    = "M3 S90";        // caneta levantada
const char* PEN_PAUSA = "G4 P0.2";   // tempo (s) para o servo chegar na posicao

// Voltar para o (0,0) ao terminar
const bool VOLTAR_A_ORIGEM = true;

// Limites de seguranca
const size_t        MAX_BODY       = 120000;   // bytes do JSON recebido
const unsigned long TIMEOUT_OK_MS  = 120000;   // espera maxima por um "ok"

const char* ARQ_JOB = "/job.gcode";

// ===================== Estado =====================

WebServer server(80);
DNSServer dnsServer;

File job;
bool busy = false;
bool aguardandoOk = false;
bool pausado = false;
bool jobConcluido = false;
String erroJob = "";
size_t totalBytes = 0, enviados = 0;
unsigned long tUltimoEnvio = 0;
String rxLinha = "";
String ultimoStatus = "";

// Estado nao bloqueante do homing. O GRBL responde "ok" ao terminar $H.
enum EstadoHoming {
  HOME_IDLE,
  HOME_ESPERANDO_H,
  HOME_ESPERANDO_ZERO
};

EstadoHoming estadoHoming = HOME_IDLE;

// Volta a false ao iniciar o ESP32, reiniciar o GRBL ou detectar uma falha.
bool homedSinceBoot = false;

// True quando existe um desenho esperando o homing terminar.
bool jobAposHoming = false;

// Timeout especifico para o ciclo de homing e para o G92 subsequente.
const unsigned long TIMEOUT_HOME_MS = 180000;

// ===================== Gerador de G-code =====================
// ---- INICIO GERADOR ----

static void wl(File &f, const char* s) {
  f.print(s);
  f.print('\n');
}

// Converte o JSON [{w, p:[[x,y],...]}, ...] em G-code gravado em ARQ_JOB.
// Retorna o numero de tracos gerados, ou -1 em caso de erro.
int gerarGcode(const char* json) {
  File f = LittleFS.open(ARQ_JOB, "w");
  if (!f) return -1;

  char buf[64];

  wl(f, "G21");
  wl(f, "G90");
  snprintf(buf, sizeof(buf), "G94 F%d", FEED_DESENHO);
  wl(f, buf);
  wl(f, PEN_UP);
  wl(f, PEN_PAUSA);

  int tracos = 0;
  const char* ptr = json;

  while ((ptr = strstr(ptr, "\"p\"")) != nullptr) {
    ptr += 3;
    while (*ptr == ' ' || *ptr == ':') ptr++;
    if (*ptr != '[') { f.close(); LittleFS.remove(ARQ_JOB); return -1; }
    ptr++;

    int pontos = 0;
    float ux = 0, uy = 0;

    while (*ptr) {
      while (*ptr == ' ' || *ptr == ',' || *ptr == '\n' || *ptr == '\r' || *ptr == '\t') ptr++;
      if (*ptr == ']') { ptr++; break; }                       // fim de "p"
      if (*ptr != '[') { f.close(); LittleFS.remove(ARQ_JOB); return -1; }
      ptr++;

      char* fim;
      float x = strtof(ptr, &fim);
      if (fim == ptr) { f.close(); LittleFS.remove(ARQ_JOB); return -1; }
      ptr = fim;
      while (*ptr == ' ' || *ptr == ',') ptr++;
      float y = strtof(ptr, &fim);
      if (fim == ptr) { f.close(); LittleFS.remove(ARQ_JOB); return -1; }
      ptr = fim;
      while (*ptr && *ptr != ']') ptr++;
      if (*ptr == ']') ptr++;

      if (x < 0) x = 0;
      if (x > LIM_X) x = LIM_X;
      if (y < 0) y = 0;
      if (y > A4_ALTURA) y = A4_ALTURA;
      if (INVERTER_Y) y = A4_ALTURA - y;
      if (y > LIM_Y) y = LIM_Y;

      if (pontos == 0) {
        snprintf(buf, sizeof(buf), "G0 X%.2f Y%.2f", x, y);
        wl(f, buf);
        wl(f, PEN_DOWN);
        wl(f, PEN_PAUSA);
        pontos = 1; ux = x; uy = y;
      } else if (fabsf(x - ux) >= 0.05f || fabsf(y - uy) >= 0.05f) {
        snprintf(buf, sizeof(buf), "G1 X%.2f Y%.2f", x, y);
        wl(f, buf);
        ux = x; uy = y;
      }
    }

    if (pontos > 0) {
      wl(f, PEN_UP);
      wl(f, PEN_PAUSA);
      tracos++;
    }
  }

  wl(f, PEN_UP);
  if (VOLTAR_A_ORIGEM) wl(f, "G0 X0 Y0");
  wl(f, "G4 P0");   // o "ok" desta linha so chega quando o ultimo movimento termina
  f.close();

  if (tracos == 0) { LittleFS.remove(ARQ_JOB); return 0; }
  return tracos;
}
// ---- FIM GERADOR ----

// ===================== Envio ao GRBL =====================

bool iniciarJob() {
  job = LittleFS.open(ARQ_JOB, "r");
  if (!job) return false;

  // Nao limpar a serial aqui: estamos iniciando o arquivo imediatamente
  // apos o GRBL confirmar G92, e nao devemos descartar respostas recebidas.
  totalBytes = job.size();
  enviados = 0;
  aguardandoOk = false;
  pausado = false;
  jobConcluido = false;
  erroJob = "";
  tUltimoEnvio = millis();
  busy = true;
  return true;
}

// Inicia $H sem bloquear o servidor web. Se iniciarJobDepois for true,
// o desenho ja gerado em ARQ_JOB sera enviado apos $H e G92 serem confirmados.
bool iniciarHoming(bool iniciarJobDepois) {
  if (busy || estadoHoming != HOME_IDLE) return false;

  // Remover respostas antigas para que um "ok" anterior nao seja confundido
  // com a confirmacao de $H. O fluxo normal so chama isto quando o GRBL esta livre.
  while (Serial2.available()) Serial2.read();
  rxLinha = "";

  estadoHoming = HOME_ESPERANDO_H;
  jobAposHoming = iniciarJobDepois;
  homedSinceBoot = false;
  erroJob = "";
  jobConcluido = false;
  aguardandoOk = false;
  pausado = false;
  totalBytes = 0;
  enviados = 0;
  tUltimoEnvio = millis();

  if (iniciarJobDepois) busy = true;

  Serial.println("[HOME] Enviando $H");
  Serial2.print("$H\n");
  return true;
}

void falhaJob(const String& motivo) {
  estadoHoming = HOME_IDLE;
  jobAposHoming = false;
  homedSinceBoot = false;
  erroJob = motivo;
  if (job) job.close();
  busy = false;
  aguardandoOk = false;
  pausado = false;
  jobConcluido = false;
  Serial2.print("M5\n");  // tenta levantar a caneta
  Serial.println("[JOB] falhou: " + motivo);
}

void cancelarJob() {
  if (job) job.close();
  busy = false;
  aguardandoOk = false;
  pausado = false;
  jobConcluido = false;
  erroJob = "cancelado";
  estadoHoming = HOME_IDLE;
  jobAposHoming = false;
  homedSinceBoot = false;

  // Reset interrompe movimentos pendentes. Nao enviar $X automaticamente:
  // depois de um reset a posicao pode ser desconhecida e o proximo desenho
  // deve executar $H antes de se mover.
  Serial2.write((uint8_t)0x18);
  Serial.println("[JOB] cancelado; sera necessario novo homing");
}

void tratarLinhaGrbl(const String& l) {
  if (l.length() == 0) return;
  Serial.println("[GRBL] " + l);

  // Banner de inicializacao do GRBL: a origem anterior nao pode ser confiada.
  if (l.startsWith("Grbl")) {
    homedSinceBoot = false;
    if (busy && estadoHoming == HOME_IDLE) {
      falhaJob("GRBL reinicializou durante o trabalho");
    }
    return;
  }

  if (l.startsWith("<")) {
    ultimoStatus = l;  // resposta do comando realtime "?"
    return;
  }

  if (l == "ok") {
    // O primeiro ok confirma que $H terminou.
    if (estadoHoming == HOME_ESPERANDO_H) {
      Serial.println("[HOME] Homing concluido; definindo origem X0 Y0");
      estadoHoming = HOME_ESPERANDO_ZERO;
      tUltimoEnvio = millis();
      Serial2.print("G92 X0 Y0\n");
      return;
    }

    // O segundo ok confirma que G92 foi aceito.
    if (estadoHoming == HOME_ESPERANDO_ZERO) {
      estadoHoming = HOME_IDLE;
      homedSinceBoot = true;
      Serial.println("[HOME] Origem X0 Y0 definida");

      if (jobAposHoming) {
        jobAposHoming = false;
        if (!iniciarJob()) {
          busy = false;
          aguardandoOk = false;
          erroJob = "Falha ao abrir o G-code apos o homing";
          jobConcluido = false;
          homedSinceBoot = false;
          Serial.println("[JOB] Falha ao abrir o arquivo");
        } else {
          Serial.printf("[JOB] Iniciado apos homing: %u bytes de G-code\n",
                        (unsigned)totalBytes);
        }
      }
      return;
    }

    // Resposta normal do envio de uma linha G-code.
    aguardandoOk = false;
    return;
  }

  if (l.startsWith("error") || l.startsWith("ALARM")) {
    homedSinceBoot = false;

    if (estadoHoming != HOME_IDLE) {
      if (jobAposHoming) {
        falhaJob(l);
      } else {
        // Homing manual, sem desenho na fila.
        estadoHoming = HOME_IDLE;
        jobAposHoming = false;
        busy = false;
        aguardandoOk = false;
        erroJob = l;
        jobConcluido = false;
        Serial.println("[HOME] Falha: " + l);
      }
    } else if (busy) {
      falhaJob(l);
    }
  }
}

// Evita deixar a maquina presa indefinidamente aguardando $H ou o ok de G92.
void verificarTimeoutHoming() {
  if (estadoHoming == HOME_IDLE) return;
  if (millis() - tUltimoEnvio <= TIMEOUT_HOME_MS) return;

  String motivo = (estadoHoming == HOME_ESPERANDO_H)
    ? "timeout aguardando fim do homing ($H)"
    : "timeout aguardando confirmacao de G92 X0 Y0";

  if (jobAposHoming) {
    falhaJob(motivo);
  } else {
    estadoHoming = HOME_IDLE;
    jobAposHoming = false;
    homedSinceBoot = false;
    busy = false;
    aguardandoOk = false;
    erroJob = motivo;
    jobConcluido = false;
    Serial.println("[HOME] " + motivo);
  }
}

// Leitura sem bloqueio da serial do GRBL
void lerGrbl() {
  while (Serial2.available()) {
    char c = (char)Serial2.read();
    if (c == '\r') continue;
    if (c == '\n') {
      tratarLinhaGrbl(rxLinha);
      rxLinha = "";
    } else if (rxLinha.length() < 120) {
      rxLinha += c;
    }
  }
}

// Manda a proxima linha do arquivo quando o GRBL confirmou a anterior
void avancarJob() {
  // Durante $H/G92, busy pode estar ativo para reservar o trabalho, mas
  // nenhuma linha do arquivo deve ser enviada antes da confirmacao de G92.
  if (estadoHoming != HOME_IDLE) return;
  if (!busy) return;

  if (aguardandoOk) {
    if (!pausado && millis() - tUltimoEnvio > TIMEOUT_OK_MS) falhaJob("timeout do GRBL");
    return;
  }

  if (!job.available()) {        // arquivo acabou e o ultimo "ok" ja chegou
    job.close();
    busy = false;
    jobConcluido = true;
    Serial.println("[JOB] concluido");
    return;
  }

  String linha = job.readStringUntil('\n');
  linha.trim();
  enviados = job.position();
  if (linha.length() == 0) return;

  Serial2.print(linha);
  Serial2.print('\n');
  aguardandoOk = true;
  tUltimoEnvio = millis();
}

// ===================== Servidor web =====================

void enviarIndex() {
  server.send_P(200, "text/html", INDEX_sHTML);
}

void configurarRotas() {
  server.on("/", HTTP_GET, enviarIndex);

  server.on("/api/draw", HTTP_POST, []() {
    if (busy || estadoHoming != HOME_IDLE) {
      server.send(409, "text/plain", "ocupado");
      return;
    }
    if (!server.hasArg("plain")) {
      server.send(400, "text/plain", "corpo ausente");
      return;
    }

    const String& body = server.arg("plain");
    if (body.length() > MAX_BODY) {
      server.send(413, "text/plain", "desenho grande demais");
      return;
    }

    int tracos = gerarGcode(body.c_str());
    if (tracos <= 0) {
      server.send(400, "text/plain", "desenho invalido ou vazio");
      return;
    }

    // Primeiro desenho depois de ligar/reiniciar: homing e G92 antes do G-code.
    if (!homedSinceBoot) {
      if (!iniciarHoming(true)) {
        server.send(409, "text/plain", "nao foi possivel iniciar homing");
        return;
      }
      Serial.printf("[JOB] %d tracos; aguardando homing antes de plotar\n", tracos);
      server.send(200, "text/plain", "homing iniciado; desenho na fila");
      return;
    }

    if (!iniciarJob()) {
      server.send(500, "text/plain", "falha ao abrir o job");
      return;
    }

    Serial.printf("[JOB] iniciado: %d tracos, %u bytes de G-code\n",
                  tracos, (unsigned)totalBytes);
    server.send(200, "text/plain", "ok");
  });

  server.on("/api/status", HTTP_GET, []() {
    int pct = totalBytes ? (int)(((uint64_t)enviados * 100) / totalBytes) : 0;
    if (busy && pct > 99) pct = 99;
    if (!busy && jobConcluido) pct = 100;

    // Codifica caracteres que poderiam invalidar o JSON do campo erro.
    String erroJson = "";
    for (size_t i = 0; i < erroJob.length(); ++i) {
      char c = erroJob[i];
      if (c == '"' || c == '\\') erroJson += '\\';
      if (c == '\n') erroJson += "\\n";
      else if (c == '\r') erroJson += "\\r";
      else erroJson += c;
    }

    String j = "{\"busy\":";
    j += busy ? "true" : "false";
    j += ",\"homing\":";
    j += (estadoHoming != HOME_IDLE) ? "true" : "false";
    j += ",\"pct\":";
    j += pct;
    if (erroJob.length()) {
      j += ",\"erro\":\"";
      j += erroJson;
      j += "\"";
    }
    j += "}";
    server.send(200, "application/json", j);
  });

  server.on("/api/pause", HTTP_ANY, []() {
    if (busy && estadoHoming == HOME_IDLE) {
      Serial2.write('!');
      pausado = true;
    }
    server.send(200, "text/plain", "ok");
  });

  server.on("/api/resume", HTTP_ANY, []() {
    if (busy && estadoHoming == HOME_IDLE) {
      Serial2.write('~');
      pausado = false;
      tUltimoEnvio = millis();
    }
    server.send(200, "text/plain", "ok");
  });

  server.on("/api/cancel", HTTP_ANY, []() {
    cancelarJob();
    server.send(200, "text/plain", "ok");
  });

  // Utilitarios de caneta; ficam bloqueados durante qualquer homing ou job.
  server.on("/api/pen", HTTP_ANY, []() {
    if (busy || estadoHoming != HOME_IDLE) {
      server.send(409, "text/plain", "ocupado");
      return;
    }
    String pos = server.arg("pos");
    if (pos == "down") {
      Serial2.print(PEN_DOWN); Serial2.print('\n');
    } else if (pos == "up") {
      Serial2.print(PEN_UP); Serial2.print('\n');
    } else {
      server.send(400, "text/plain", "use ?pos=up ou ?pos=down");
      return;
    }
    server.send(200, "text/plain", "ok");
  });

  server.on("/api/unlock", HTTP_ANY, []() {
    if (busy || estadoHoming != HOME_IDLE) {
      server.send(409, "text/plain", "ocupado");
      return;
    }
    // Desbloquear nao equivale a homing; o proximo desenho ainda deve fazer $H.
    homedSinceBoot = false;
    Serial2.print("$X\n");
    server.send(200, "text/plain", "comando $X enviado; homing necessario antes de plotar");
  });

  server.on("/api/home", HTTP_ANY, []() {
    if (busy || estadoHoming != HOME_IDLE) {
      server.send(409, "text/plain", "ocupado");
      return;
    }
    if (!iniciarHoming(false)) {
      server.send(409, "text/plain", "nao foi possivel iniciar homing");
      return;
    }
    server.send(200, "text/plain", "homing iniciado; a origem sera definida ao terminar");
  });

  // Define manualmente a origem de trabalho; nao substitui o homing validado.
  server.on("/api/zero", HTTP_ANY, []() {
    if (busy || estadoHoming != HOME_IDLE) {
      server.send(409, "text/plain", "ocupado");
      return;
    }
    homedSinceBoot = false;
    Serial2.print("G92 X0 Y0\n");
    server.send(200, "text/plain", "G92 enviado; proxima impressao fara homing se necessario");
  });

  // Pergunta a posicao ao GRBL (? em tempo real).
  server.on("/api/grbl", HTTP_ANY, []() {
    ultimoStatus = "";
    Serial2.write('?');
    unsigned long t = millis();
    while (ultimoStatus.length() == 0 && millis() - t < 400) {
      lerGrbl();
      delay(5);
    }
    server.send(200, "text/plain", ultimoStatus.length() ? ultimoStatus : "sem resposta do GRBL");
  });

  // Captive portal: qualquer outro endereco volta para a pagina principal.
  server.onNotFound([]() {
    server.sendHeader("Location", String("http://") + WiFi.softAPIP().toString() + "/", true);
    server.send(302, "text/plain", "");
  });
}

// ===================== Setup / Loop =====================

void setup() {
  Serial.begin(115200);

  Serial2.setRxBufferSize(1024);                       // antes do begin()
  Serial2.begin(GRBL_BAUD, SERIAL_8N1, RX2, TX2);

  if (!LittleFS.begin(true)) Serial.println("Falha ao iniciar o LittleFS (usado so para o arquivo do desenho)");

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, nullptr, 6, 0, 4);              // aberta, canal 6, ate 4 clientes
  Serial.print("AP ativo: ");
  Serial.print(AP_SSID);
  Serial.print("  IP: ");
  Serial.println(WiFi.softAPIP());

  dnsServer.start(53, "*", WiFi.softAPIP());

  configurarRotas();
  server.begin();
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  lerGrbl();
  verificarTimeoutHoming();
  avancarJob();
}
