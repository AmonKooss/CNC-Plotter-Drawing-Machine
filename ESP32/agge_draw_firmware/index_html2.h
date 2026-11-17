#pragma once
#include <pgmspace.h>

const char INDEX_sHTML[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="pt-BR">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<title>AGGE DRAW</title>
<style>
html,body{margin:0;padding:0;background:#fff;color:#111;font-family:Arial,Helvetica,sans-serif}
body{padding:10px;max-width:760px;margin:0 auto;-webkit-text-size-adjust:100%}
*{-webkit-tap-highlight-color:transparent;box-sizing:border-box}
header{font-size:18px;font-weight:bold;padding:5px 0 10px;border-bottom:2px solid #111;margin-bottom:10px}
#paper{width:100%;background:#fff;border:1px solid #111;line-height:0;overflow:hidden}
#cv{display:block;width:100%;height:auto;background:#fff;touch-action:none;-ms-touch-action:none}
#toolbar{padding:8px 0 4px;text-align:center}
.tool{display:inline-block;vertical-align:middle;min-width:54px;height:40px;margin:2px;padding:0 9px;border:1px solid #111;border-radius:4px;background:#fff;color:#111;font-size:14px;font-weight:bold}
.tool.active{background:#111;color:#fff}
#settings{border-top:1px solid #bbb;border-bottom:1px solid #bbb;margin:7px 0;padding:8px 2px}
.setting{display:block;margin:5px 0;font-size:13px}
.setting label{display:inline-block;min-width:112px}
.setting input{width:55%;vertical-align:middle}
#sizeValue,#smoothValue{display:inline-block;min-width:32px;text-align:right;font-weight:bold}
#note{font-size:11px;color:#444;margin:5px 0 0}
#textHint{display:none;border:1px solid #111;background:#fff;padding:8px;margin:7px 0;font-size:13px;text-align:center}
#actions{margin-top:10px}
#send{display:block;width:100%;height:48px;border:2px solid #111;border-radius:4px;background:#111;color:#fff;font-size:16px;font-weight:bold}
#send:disabled{background:#777;border-color:#777}
#status{min-height:20px;text-align:center;font-size:13px;padding:7px 0;color:#111}
#progressWrap{display:none;margin:4px 0 8px;border:1px solid #111;height:13px;background:#fff}
#progress{height:11px;width:0;background:#111}
#jobControls{display:none;text-align:center;margin:5px 0}
.jobButton{display:inline-block;margin:2px;padding:8px 14px;border:1px solid #111;background:#fff;color:#111;border-radius:4px;font-size:13px}
#home{display:inline-block;margin:7px 0 0;width:100%;padding:9px;border:1px solid #111;background:#fff;color:#111;border-radius:4px;font-size:13px}
#reset{display:inline-block;margin:7px 0 0;width:100%;padding:9px;border:1px solid #777;background:#fff;color:#111;border-radius:4px;font-size:13px}
@media (min-width:600px){body{padding:16px}header{font-size:20px}}
/* Landscape tablet layout: keep the complete workspace visible without page scrolling. */
@media screen and (orientation:landscape){
 html,body{width:100%;height:100%;overflow:hidden}
 body{padding:0;margin:0;max-width:none;position:relative;overflow:hidden}
 header{display:none}
 #paper{position:absolute;left:6px;top:6px;bottom:6px;right:224px;width:auto;height:auto;border:1px solid #111;text-align:center}
 #cv{height:100%;width:auto;max-width:100%;margin:0 auto}
 #toolbar{position:absolute;right:4px;top:4px;width:214px;padding:0;text-align:left;line-height:1.1}
 .tool{min-width:62px;height:34px;margin:2px;padding:0 5px;font-size:12px}
 #settings{position:absolute;right:5px;top:150px;width:210px;margin:0;padding:5px 2px;border-top:1px solid #bbb;border-bottom:1px solid #bbb}
 .setting{margin:4px 0;font-size:11px;white-space:nowrap}
 .setting label{min-width:90px}
 .setting input{width:42%;margin:0}
 #note{font-size:9px;margin-top:3px}
 #textHint{position:absolute;right:5px;top:300px;width:210px;margin:0;padding:5px;font-size:11px}
 #actions{position:absolute;right:5px;bottom:4px;width:210px;margin:0}
 #send{height:38px;font-size:13px}
 #home,#reset{width:49%;margin:3px 0 0;padding:6px 2px;font-size:11px}
 #status{min-height:15px;padding:3px 0;font-size:10px}
 #progressWrap{margin:2px 0 4px;height:9px}
 #progress{height:7px}
 #jobControls{margin:2px 0}
 .jobButton{margin:1px;padding:5px 7px;font-size:10px}
}
</style>
</head>
<body>
<header>AGGE DRAW</header>
<div id="paper"><canvas id="cv" width="700" height="990">Seu navegador nao suporta canvas.</canvas></div>
<div id="toolbar">
  <button type="button" class="tool active" id="brush">Pincel</button>
  <button type="button" class="tool" id="eraser">Borracha</button>
  <button type="button" class="tool" id="line">Linha</button>
  <button type="button" class="tool" id="square">Quadrado</button>
  <button type="button" class="tool" id="rect">Retangulo</button>
  <button type="button" class="tool" id="circle">Circulo</button>
  <button type="button" class="tool" id="text">Texto</button>
  <button type="button" class="tool" id="undo">Desfazer</button>
  <button type="button" class="tool" id="redo">Refazer</button>
  <button type="button" class="tool" id="clear">Limpar</button>
</div>
<div id="settings">
  <div class="setting"><label for="brushSize">Tamanho do pincel</label><input type="range" id="brushSize" min="1" max="12" value="3"><span id="sizeValue">3</span></div>
  <div class="setting"><label for="stabilizer">Estabilizacao</label><input type="range" id="stabilizer" min="0" max="90" value="55"><span id="smoothValue">55%</span></div>
  <div class="setting"><label for="textSize">Tamanho do texto</label><input type="range" id="textSize" min="7" max="28" value="14"><span id="textValue">14</span></div>
  <div id="note">Desenho preto no papel branco. O tamanho do pincel altera a visualizacao; a largura fisica depende da caneta instalada.</div>
</div>
<div id="textHint">Toque no papel para posicionar o texto. Toque em Texto novamente para cancelar.</div>
<div id="actions">
  <button type="button" id="send">Fazer homing e desenhar</button>
  <button type="button" id="home">Somente homing</button>
  <button type="button" id="reset">Novo desenho / limpar papel</button>
  <div id="status">Pronto. Area A4: 210 x 297 mm.</div>
  <div id="progressWrap"><div id="progress"></div></div>
  <div id="jobControls">
    <button type="button" class="jobButton" id="pause">Pausar</button>
    <button type="button" class="jobButton" id="resume">Continuar</button>
    <button type="button" class="jobButton" id="cancel">Cancelar trabalho</button>
  </div>
</div>
<script type="text/javascript">
(function(){
var canvas=document.getElementById('cv');
var ctx=canvas.getContext('2d');
var statusBox=document.getElementById('status');
var sendButton=document.getElementById('send');
var homeButton=document.getElementById('home');
var progressWrap=document.getElementById('progressWrap');
var progressBar=document.getElementById('progress');
var jobControls=document.getElementById('jobControls');
var strokes=[];
var undoStack=[];
var redoStack=[];
var tool='brush';
var brushSize=3;
var smoothAmount=0.55;
var textHeight=14;
var pendingText='';
var drawing=false;
var currentStroke=null;
var shapeStart=null;
var shapePreview=null;
var lastFiltered=null;
var eraseChanged=false;
var lastTouchTime=0;
var pollTimer=null;
var operation='';
var homeStartedAt=0;
var payloadText='';
var busy=false;
var toolIds=['brush','eraser','line','square','rect','circle','text'];

function setStatus(s){statusBox.textContent=s;}
function setTool(name){
  tool=name;
  var i,b;
  for(i=0;i<toolIds.length;i++){
    b=document.getElementById(toolIds[i]);
    b.className='tool'+(tool===toolIds[i]?' active':'');
  }
  if(name!=='text' && pendingText){pendingText='';document.getElementById('textHint').style.display='none';}
}
function copyStrokes(a){return JSON.parse(JSON.stringify(a));}
function remember(){
  undoStack.push(copyStrokes(strokes));
  if(undoStack.length>40)undoStack.shift();
  redoStack=[];
}
function redraw(){
  var i,j,s,p;
  ctx.clearRect(0,0,canvas.width,canvas.height);
  ctx.fillStyle='#fff';ctx.fillRect(0,0,canvas.width,canvas.height);
  ctx.strokeStyle='#000';ctx.fillStyle='#000';ctx.lineCap='round';ctx.lineJoin='round';
  for(i=0;i<strokes.length;i++)drawStroke(strokes[i]);
  if(shapePreview)drawStroke(shapePreview);
}
function drawStroke(s){
  var p=s.p;
  if(!p || !p.length)return;
  ctx.strokeStyle='#000';ctx.fillStyle='#000';ctx.lineWidth=s.w||brushSize;ctx.lineCap='round';ctx.lineJoin='round';
  if(p.length===1){ctx.beginPath();ctx.arc(p[0].x,p[0].y,(s.w||brushSize)/2,0,Math.PI*2,false);ctx.fill();return;}
  ctx.beginPath();ctx.moveTo(p[0].x,p[0].y);
  var i;for(i=1;i<p.length;i++)ctx.lineTo(p[i].x,p[i].y);
  ctx.stroke();
}
function localPoint(clientX,clientY){
  var r=canvas.getBoundingClientRect();
  return {x:(clientX-r.left)*(canvas.width/r.width),y:(clientY-r.top)*(canvas.height/r.height)};
}
function distance(a,b){var dx=a.x-b.x,dy=a.y-b.y;return Math.sqrt(dx*dx+dy*dy);}
function addBrushPoint(raw){
  var alpha=1-(smoothAmount*0.88),q;
  if(!lastFiltered){lastFiltered={x:raw.x,y:raw.y};q=lastFiltered;}
  else{q={x:lastFiltered.x+(raw.x-lastFiltered.x)*alpha,y:lastFiltered.y+(raw.y-lastFiltered.y)*alpha};lastFiltered=q;}
  var p=currentStroke.p;
  if(!p.length || distance(p[p.length-1],q)>=1.8)p.push({x:q.x,y:q.y});
  redraw();
}
function pointLineDistance(p,a,b){
  var dx=b.x-a.x,dy=b.y-a.y;
  if(dx===0 && dy===0)return distance(p,a);
  var t=((p.x-a.x)*dx+(p.y-a.y)*dy)/(dx*dx+dy*dy);
  if(t<0)t=0;if(t>1)t=1;
  return distance(p,{x:a.x+t*dx,y:a.y+t*dy});
}
function eraseAt(q){
  var i,j,s,hit=false,radius=18+brushSize*2;
  for(i=strokes.length-1;i>=0;i--){
    s=strokes[i];
    for(j=0;j<s.p.length;j++){
      if(distance(q,s.p[j])<radius || (j>0 && pointLineDistance(q,s.p[j-1],s.p[j])<radius)){hit=true;break;}
    }
    if(hit){
      if(!eraseChanged){remember();eraseChanged=true;}
      strokes.splice(i,1);hit=false;
    }
  }
  if(eraseChanged)redraw();
}
function linePoints(a,b){
  var d=distance(a,b),n=Math.max(1,Math.ceil(d/7)),p=[],i,t;
  for(i=0;i<=n;i++){t=i/n;p.push({x:a.x+(b.x-a.x)*t,y:a.y+(b.y-a.y)*t});}
  return p;
}
function shapePoints(name,a,b){
  var p=[],dx=b.x-a.x,dy=b.y-a.y,i,t,rx,ry,ang;
  if(name==='line')return linePoints(a,b);
  if(name==='square'){
    var side=Math.min(Math.abs(dx),Math.abs(dy));
    dx=(dx<0?-1:1)*side;dy=(dy<0?-1:1)*side;
  }
  if(name==='rect'||name==='square'){
    p=[{x:a.x,y:a.y},{x:a.x+dx,y:a.y},{x:a.x+dx,y:a.y+dy},{x:a.x,y:a.y+dy},{x:a.x,y:a.y}];
    return p;
  }
  if(name==='circle'){
    rx=Math.min(Math.abs(dx),Math.abs(dy));ry=rx;
    if(rx<1)rx=1;if(ry<1)ry=1;
    for(i=0;i<=64;i++){ang=(Math.PI*2*i)/64;p.push({x:a.x+Math.cos(ang)*rx,y:a.y+Math.sin(ang)*ry});}
    return p;
  }
  return p;
}
function beginDraw(q){
  if(busy)return;
  if(pendingText){placeText(q);return;}
  drawing=true;
  if(tool==='eraser'){eraseChanged=false;eraseAt(q);return;}
  if(tool==='line'||tool==='square'||tool==='rect'||tool==='circle'){
    shapeStart=q;shapePreview={w:brushSize,p:[q,q]};return;
  }
  remember();
  currentStroke={w:brushSize,p:[{x:q.x,y:q.y}]};
  strokes.push(currentStroke);
  lastFiltered={x:q.x,y:q.y};
  redraw();
}
function moveDraw(q){
  if(!drawing || busy)return;
  if(tool==='eraser'){eraseAt(q);return;}
  if(tool==='line'||tool==='square'||tool==='rect'||tool==='circle'){
    if(shapeStart){shapePreview={w:brushSize,p:shapePoints(tool,shapeStart,q)};redraw();}
    return;
  }
  if(currentStroke)addBrushPoint(q);
}
function endDraw(q){
  if(!drawing)return;
  if(tool==='eraser'){drawing=false;eraseChanged=false;return;}
  if(tool==='line'||tool==='square'||tool==='rect'||tool==='circle'){
    if(shapeStart){remember();strokes.push({w:brushSize,p:shapePoints(tool,shapeStart,q)});}
    shapeStart=null;shapePreview=null;drawing=false;redraw();return;
  }
  if(currentStroke){
    var p=currentStroke.p;
    if(q && distance(p[p.length-1],q)>0.8)p.push({x:q.x,y:q.y});
    if(p.length>2){var last=p[p.length-1],prev=p[p.length-2];if(distance(last,prev)<1.8)p.pop();}
  }
  currentStroke=null;lastFiltered=null;drawing=false;redraw();
}
function onTouchStart(e){
  e.preventDefault();lastTouchTime=(new Date()).getTime();
  if(e.touches.length!==1)return;
  var t=e.touches[0];beginDraw(localPoint(t.clientX,t.clientY));
}
function onTouchMove(e){
  e.preventDefault();lastTouchTime=(new Date()).getTime();
  if(e.touches.length!==1)return;
  var t=e.touches[0];moveDraw(localPoint(t.clientX,t.clientY));
}
function onTouchEnd(e){
  e.preventDefault();lastTouchTime=(new Date()).getTime();
  var t=e.changedTouches&&e.changedTouches.length?e.changedTouches[0]:null;
  endDraw(t?localPoint(t.clientX,t.clientY):null);
}
canvas.addEventListener('touchstart',onTouchStart,false);
canvas.addEventListener('touchmove',onTouchMove,false);
canvas.addEventListener('touchend',onTouchEnd,false);
canvas.addEventListener('touchcancel',onTouchEnd,false);
canvas.addEventListener('mousedown',function(e){if((new Date()).getTime()-lastTouchTime<700)return;e.preventDefault();beginDraw(localPoint(e.clientX,e.clientY));},false);
window.addEventListener('mousemove',function(e){if(!drawing)return;e.preventDefault();moveDraw(localPoint(e.clientX,e.clientY));},false);
window.addEventListener('mouseup',function(e){if(!drawing)return;endDraw(localPoint(e.clientX,e.clientY));},false);

/* Fonte vetorial simples de traco unico para texto. */
var TL=[0,0],TM=[1.5,0],TR=[3,0],uM=[1.5,1.25],ML=[0,2.5],MM=[1.5,2.5],MR=[3,2.5],BL=[0,5],BM=[1.5,5],BR=[3,5];
function seg(n){var s={top:[TL,TR],topleft:[TL,ML],topright:[TR,MR],middle:[ML,MR],bottomleft:[ML,BL],bottomright:[MR,BR],bottom:[BL,BR]};return s[n];}
var DIGITS={
'0':['top','topleft','topright','bottomleft','bottomright','bottom'],'1':['topright','bottomright'],'2':['top','topright','middle','bottomleft','bottom'],'3':['top','topright','middle','bottomright','bottom'],'4':['topleft','topright','middle','bottomright'],'5':['top','topleft','middle','bottomright','bottom'],'6':['top','topleft','middle','bottomleft','bottomright','bottom'],'7':['top','topright'],'8':['top','topleft','topright','middle','bottomleft','bottomright','bottom'],'9':['top','topleft','topright','middle','bottomright','bottom']};
var LETTERS={
A:[[BL,TM,BR],[ML,MR]],B:[[TL,BL],[TL,TR,MR,ML],[ML,BR,BL]],C:[[TR,TL,BL,BR]],D:[[TL,BL],[TL,TR,BR,BL]],E:[[TR,TL,BL,BR],[ML,MR]],F:[[BL,TL,TR],[ML,MR]],G:[[TR,TL,BL,BR,MR],[MM,MR]],H:[[TL,BL],[TR,BR],[ML,MR]],I:[[TL,TR],[TM,BM],[BL,BR]],J:[[TR,BR,BL]],K:[[TL,BL],[TR,ML,BR]],L:[[TL,BL,BR]],M:[[BL,TL,uM,TR,BR]],N:[[BL,TL,BR,TR]],O:[[TL,TR,BR,BL,TL]],P:[[BL,TL,TR,MR,ML]],Q:[[TL,TR,BR,BL,TL],[[1.8,3.5],BR]],R:[[BL,TL,TR,MR,ML],[ML,BR]],S:[[TR,TL,ML,MR,BR,BL]],T:[[TL,TR],[TM,BM]],U:[[TL,BL,BR,TR]],V:[[TL,BM,TR]],W:[[TL,BL,MM,BR,TR]],X:[[TL,BR],[TR,BL]],Y:[[TL,MM],[TR,MM],[MM,BM]],Z:[[TL,TR,BL,BR]],'.':[[[1.4,4.9],[1.6,5]]],',':[[[1.4,4.9],[1.7,5.3]]],'-':[[ML,MR]],':':[[[1.5,1.7],[1.5,1.8]],[[1.5,4.2],[1.5,4.3]]]};
function glyphStrokes(ch){if(ch===' ')return [];if(DIGITS[ch]){var a=[],i;for(i=0;i<DIGITS[ch].length;i++)a.push(seg(DIGITS[ch][i]));return a;}return LETTERS[ch]||[];}
function placeText(anchor){
  var x=anchor.x,y=anchor.y,unit=textHeight/5,w=unit*3,gap=unit*1.25,i,j,k,parts,src;
  remember();
  for(i=0;i<pendingText.length;i++){
    parts=glyphStrokes(pendingText.charAt(i));
    for(j=0;j<parts.length;j++){
      src=parts[j];var points=[];
      for(k=0;k<src.length;k++)points.push({x:x+src[k][0]*unit,y:y+src[k][1]*unit});
      if(points.length)strokes.push({w:Math.max(1,brushSize),p:points});
    }
    x+=w+gap;
  }
  pendingText='';document.getElementById('textHint').style.display='none';setTool('brush');redraw();
}

function buildPayload(){
  var out=[],i,j,s,p,prev,dx,dy;
  for(i=0;i<strokes.length;i++){
    s=strokes[i];p=[];prev=null;
    for(j=0;j<s.p.length;j++){
      var x=s.p[j].x*0.3,y=s.p[j].y*0.3;
      if(prev){dx=x-prev[0];dy=y-prev[1];if(dx*dx+dy*dy<0.0036 && j<s.p.length-1)continue;}
      p.push([Math.round(x*10)/10,Math.round(y*10)/10]);prev=[x,y];
    }
    if(p.length)out.push({w:s.w||brushSize,p:p});
  }
  return JSON.stringify(out);
}
function xhr(method,url,body,done){
  var req=new XMLHttpRequest(),completed=false;
  function finish(code,text){if(completed)return;completed=true;done(code,text);}
  req.open(method,url,true);
  req.onreadystatechange=function(){if(req.readyState===4)finish(req.status,req.responseText);};
  req.onerror=function(){finish(0,'Falha de conexao');};
  if(body!==null){req.setRequestHeader('Content-Type','application/json');}
  req.send(body===null?null:body);
}
function parseStatus(text){try{return JSON.parse(text);}catch(e){return null;}}
function stopPolling(){if(pollTimer){clearTimeout(pollTimer);pollTimer=null;}}
function setBusy(v){busy=v;sendButton.disabled=v;homeButton.disabled=v;document.getElementById('reset').disabled=v;}
function showProgress(p){if(p<0)p=0;if(p>100)p=100;progressWrap.style.display='block';progressBar.style.width=p+'%';}
function pollHome(){
  if(!busy)return;
  xhr('GET','/api/status',null,function(code,text){
    if(code!==200){failJob('Nao foi possivel consultar o homing.');return;}
    var d=parseStatus(text);
    if(!d){failJob('Resposta de status invalida.');return;}
    if(d.erro){failJob('Falha no homing: '+d.erro);return;}
    if(d.homing){
      if((new Date()).getTime()-homeStartedAt>200000){failJob('Tempo limite aguardando o homing.');return;}
      setStatus('Homing em andamento. Aguardando a origem X0 Y0...');
      pollTimer=setTimeout(pollHome,700);return;
    }
    if(operation==='draw')sendDrawing();
    else{setStatus('Homing concluido. Origem X0 Y0 definida.');finishOperation();}
  });
}
function sendDrawing(){
  setStatus('Homing concluido. Enviando desenho...');
  xhr('POST','/api/draw',payloadText,function(code,text){
    if(code!==200){failJob('Erro ao enviar desenho ('+code+'): '+text);return;}
    operation='plot';jobControls.style.display='block';showProgress(0);
    setStatus('Desenho enviado. Plotagem em andamento...');
    pollDrawing();
  });
}
function pollDrawing(){
  if(!busy || operation!=='plot')return;
  xhr('GET','/api/status',null,function(code,text){
    if(code!==200){pollTimer=setTimeout(pollDrawing,1000);return;}
    var d=parseStatus(text);
    if(!d){pollTimer=setTimeout(pollDrawing,1000);return;}
    showProgress(d.pct||0);
    if(d.erro){failJob('Erro do GRBL: '+d.erro);return;}
    if(!d.busy){
      if(d.pct===100){showProgress(100);setStatus('Plotagem concluida.');}
      else setStatus('Trabalho interrompido ou cancelado.');
      finishOperation();return;
    }
    setStatus('Plotando... '+(d.pct||0)+'%');
    pollTimer=setTimeout(pollDrawing,900);
  });
}
function finishOperation(){
  stopPolling();setBusy(false);jobControls.style.display='none';operation='';
  sendButton.disabled=false;homeButton.disabled=false;
}
function failJob(message){stopPolling();setStatus(message);finishOperation();}
function startHome(drawAfter){
  if(busy)return;
  if(drawAfter){
    if(!strokes.length){setStatus('Desenhe algo antes de enviar.');return;}
    payloadText=buildPayload();
    if(payloadText.length>110000){setStatus('Desenho muito detalhado. Simplifique os tracos.');return;}
  }
  operation=drawAfter?'draw':'home';setBusy(true);showProgress(0);setStatus('Solicitando homing antes do desenho...');
  homeStartedAt=(new Date()).getTime();
  xhr('GET','/api/home',null,function(code,text){
    if(code!==200){failJob('Nao foi possivel iniciar o homing: '+text);return;}
    pollTimer=setTimeout(pollHome,500);
  });
}
function command(url){xhr('GET',url,null,function(code,text){if(code!==200)setStatus('Falha no comando: '+text);});}

document.getElementById('brush').onclick=function(){setTool('brush');};
document.getElementById('eraser').onclick=function(){setTool('eraser');};
document.getElementById('line').onclick=function(){setTool('line');};
document.getElementById('square').onclick=function(){setTool('square');};
document.getElementById('rect').onclick=function(){setTool('rect');};
document.getElementById('circle').onclick=function(){setTool('circle');};
document.getElementById('text').onclick=function(){
  if(pendingText){pendingText='';document.getElementById('textHint').style.display='none';setTool('brush');return;}
  var t=window.prompt('Digite o texto (letras A-Z e numeros):','');
  if(t){pendingText=t.toUpperCase().substring(0,30);setTool('text');document.getElementById('textHint').style.display='block';setStatus('Toque no papel para posicionar o texto.');}
};
document.getElementById('undo').onclick=function(){if(undoStack.length){redoStack.push(copyStrokes(strokes));strokes=undoStack.pop();redraw();}};
document.getElementById('redo').onclick=function(){if(redoStack.length){undoStack.push(copyStrokes(strokes));strokes=redoStack.pop();redraw();}};
document.getElementById('clear').onclick=function(){if(!strokes.length)return;remember();strokes=[];redraw();};
document.getElementById('brushSize').onchange=document.getElementById('brushSize').oninput=function(){brushSize=parseInt(this.value,10)||3;document.getElementById('sizeValue').innerHTML=brushSize;};
document.getElementById('stabilizer').onchange=document.getElementById('stabilizer').oninput=function(){smoothAmount=(parseInt(this.value,10)||0)/100;document.getElementById('smoothValue').innerHTML=this.value+'%';};
document.getElementById('textSize').onchange=document.getElementById('textSize').oninput=function(){textHeight=parseInt(this.value,10)||14;document.getElementById('textValue').innerHTML=textHeight;};
sendButton.onclick=function(){startHome(true);};
homeButton.onclick=function(){startHome(false);};
document.getElementById('pause').onclick=function(){command('/api/pause');setStatus('Pausa solicitada.');};
document.getElementById('resume').onclick=function(){command('/api/resume');setStatus('Continuacao solicitada.');};
document.getElementById('cancel').onclick=function(){command('/api/cancel');setStatus('Cancelamento solicitado.');};
document.getElementById('reset').onclick=function(){
  if(busy)return;
  if(!window.confirm('Apagar o desenho atual?'))return;
  strokes=[];undoStack=[];redoStack=[];pendingText='';shapePreview=null;redraw();
  document.getElementById('textHint').style.display='none';progressWrap.style.display='none';setTool('brush');setStatus('Papel limpo. Pronto para desenhar.');
};
redraw();
})();
</script>
</body>
</html>

)rawliteral";
