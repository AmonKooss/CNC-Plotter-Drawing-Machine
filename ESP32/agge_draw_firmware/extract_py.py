import re
import webbrowser
import os

def programa():
    arquivo_entrada = "index_html.h"
    arquivo_saida = "index.html"

    print(f"Lendo o arquivo {arquivo_entrada}...")
    try:
        with     open(arquivo_entrada, "r", encoding="utf-8") as f:
            conteudo = f.read()
    except FileNotFoundError:
        print(f"Erro: O arquivo '{arquivo_entrada}' não foi encontrado no diretório.")
        return

    # Extrai o conteúdo HTML dentro do bloco R"rawliteral(...)rawliteral";
    padrao = r'R"rawliteral\((.*?)\)rawliteral";'
    correspondencia = re.search(padrao, conteudo, re.DOTALL)

    if not correspondencia:
        print("Erro: Não foi possível encontrar o conteúdo HTML delimitado por R\"rawliteral(\n...)\"rawliteral\";")
        return

    html_extraido = correspondencia.group(1)

    # Salva o HTML extraído em um arquivo funcional
    with open(arquivo_saida, "w", encoding="utf-8") as f:
        f.write(html_extraido)

    print(f"Arquivo HTML gerado com sucesso: {arquivo_saida}")

    # Abre o arquivo HTML gerado no navegador padrão
    caminho_absoluto = os.path.abspath(arquivo_saida)
    webbrowser.open(f"file://{caminho_absoluto}")
    print("Abrindo o arquivo visualizável no navegador...")

if __name__ == "__main__":
    programa()
