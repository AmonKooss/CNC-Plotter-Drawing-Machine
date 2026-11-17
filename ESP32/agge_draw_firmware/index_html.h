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
html,body{margin:0;padding:0;background:#55595d;color:#111;font-family:Arial,Helvetica,sans-serif;min-height:100vh;width:100%}
body{padding:10px;max-width:none;margin:0 auto;-webkit-text-size-adjust:100%}
*{-webkit-tap-highlight-color:transparent;box-sizing:border-box}
header{font-size:18px;font-weight:bold;padding:8px 12px;background:#222;color:#fff;border-radius:6px;text-align:center;margin:0 auto 12px auto;max-width:760px;box-shadow:0 2px 6px rgba(0,0,0,0.3)}

#paper{width:100%;max-width:700px;margin:0 auto 16px auto;background:#55595d;border:none;line-height:0;overflow:hidden;border-radius:2px;display:flex;justify-content:center}
#cv{display:block;width:auto;max-width:100%;height:auto;background:#fff;touch-action:none;-ms-touch-action:none;box-shadow:0 6px 16px rgba(0,0,0,0.4)}

#toolbar{padding:4px 0;text-align:center;max-width:760px;margin:0 auto}
.btn-grid{display:grid;grid-template-columns:1fr 1fr;gap:8px;margin-bottom:8px}
.btn-grid.full{grid-template-columns:1fr}

.tool{display:flex;align-items:center;justify-content:center;height:44px;padding:0 8px;border:none;border-radius:6px;background:#ffffff;color:#111;font-size:14px;font-weight:bold;cursor:pointer;box-shadow:0 2px 5px rgba(0,0,0,0.25)}
.tool svg{margin-right:6px;fill:currentColor;flex-shrink:0}
.tool.active{background:#222222;color:#ffffff}
.tool.active svg{fill:#ffffff}

.gap-large{height:16px}
.separator{border-bottom:1px solid rgba(255,255,255,0.2);margin:14px 0}

#settings{background:#ffffff;border:none;border-radius:8px;padding:12px;margin:10px 0;box-shadow:0 2px 6px rgba(0,0,0,0.25)}
.setting{display:flex;align-items:center;margin:10px 0;font-size:13px;font-weight:bold;color:#222}
.setting label{flex:1;white-space:nowrap}
.setting input[type=range]{-webkit-appearance:none;width:55%;height:14px;background:#ddd;border:none;border-radius:7px;outline:none;margin:0 8px}
.setting input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:26px;height:26px;border-radius:50%;background:#222;border:none;box-shadow:0 2px 5px rgba(0,0,0,0.4);cursor:pointer}
#sizeValue,#smoothValue,#textValue{min-width:36px;text-align:right;font-weight:bold;color:#111}

#note{font-size:11px;color:#555;margin:6px 0 0;text-align:center}
#textHint{display:none;border:none;background:#fff8c5;color:#222;padding:10px;margin:8px 0;font-size:13px;text-align:center;border-radius:6px;font-weight:bold;box-shadow:0 2px 6px rgba(0,0,0,0.2)}

#actions{margin-top:10px}
.action-btn{display:flex;align-items:center;justify-content:center;width:100%;height:48px;margin:8px 0;border:none;border-radius:6px;background:#222222;color:#ffffff;font-size:15px;font-weight:bold;cursor:pointer;box-shadow:0 3px 6px rgba(0,0,0,0.3)}
.action-btn svg{margin-right:8px;fill:currentColor}
.action-btn.secondary{background:#ffffff;color:#111111}
.action-btn:disabled{background:#888888;color:#cccccc}

#status{min-height:20px;text-align:center;font-size:13px;padding:8px;color:#fff;background:#222;border-radius:6px;margin-top:8px;box-shadow:0 2px 5px rgba(0,0,0,0.2)}
#progressWrap{display:none;margin:6px 0;border:none;height:14px;background:#ffffff;border-radius:7px;overflow:hidden;box-shadow:0 1px 4px rgba(0,0,0,0.3)}
#progress{height:100%;width:0;background:#222}
#jobControls{display:none;text-align:center;margin:6px 0}
.jobButton{display:inline-flex;align-items:center;margin:2px;padding:8px 12px;border:none;background:#ffffff;color:#111;border-radius:4px;font-size:12px;font-weight:bold;box-shadow:0 2px 4px rgba(0,0,0,0.2)}

/* Modal Virtual Keyboard UI */
#textModal{display:none;position:fixed;top:0;left:0;right:0;bottom:0;background:rgba(0,0,0,0.75);z-index:999;align-items:center;justify-content:center;padding:10px}
.modal-content{background:#ffffff;border:none;border-radius:8px;padding:12px;width:100%;max-width:500px;box-shadow:0 6px 20px rgba(0,0,0,0.5)}
.modal-title{font-size:16px;font-weight:bold;margin-bottom:8px;text-align:center;color:#111}
#textPreview{width:100%;height:40px;border:none;border-radius:6px;padding:6px;font-size:18px;font-weight:bold;text-align:center;background:#f0f0f0;margin-bottom:10px;overflow:hidden;white-space:nowrap;color:#111}
.keyboard-row{display:flex;justify-content:center;margin-bottom:4px}
.key{flex:1;height:42px;margin:2px;border:none;background:#e2e6e9;color:#111;border-radius:4px;font-size:16px;font-weight:bold;display:flex;align-items:center;justify-content:center;cursor:pointer;-webkit-user-select:none;user-select:none;box-shadow:0 1px 3px rgba(0,0,0,0.2)}
.key:active{background:#bbb}
.key.wide{flex:1.5}
.key.action-key{background:#d0d5d9}
.modal-actions{display:flex;gap:8px;margin-top:10px}
.modal-btn{flex:1;height:44px;border:none;border-radius:6px;font-weight:bold;font-size:14px;cursor:pointer;box-shadow:0 2px 4px rgba(0,0,0,0.2)}
.modal-btn.cancel{background:#e74c3c;color:#fff}
.modal-btn.confirm{background:#2ecc71;color:#fff}

@media screen and (orientation:landscape){
 html,body{width:100%;height:100%;overflow:hidden;background:#55595d}
 body{padding:0;margin:0;max-width:none;position:relative;overflow:hidden}
 header{display:none}
 #paper{position:absolute;left:6px;top:6px;bottom:6px;right:254px;width:auto;height:auto;max-width:none;border:none;text-align:center;margin:0;background:#55595d;box-shadow:none}
 #cv{height:100%;width:auto;max-width:100%;margin:0 auto;box-shadow:0 4px 12px rgba(0,0,0,0.4)}
 #toolbar{position:absolute;right:6px;top:6px;width:240px;padding:0;overflow-y:auto;max-height:98vh;margin:0}
 .tool{height:36px;font-size:12px}
 .tool svg{width:14px;height:14px}
 .setting{margin:4px 0;font-size:11px}
 .setting input[type=range]{height:10px}
 .setting input[type=range]::-webkit-slider-thumb{width:20px;height:20px}
 .gap-large{height:6px}
 .separator{margin:6px 0}
 .action-btn{height:38px;font-size:12px;margin:4px 0}
 #status{font-size:10px;padding:3px}
}
</style>
</head>
<body>
<header>AGGE DRAW</header>

<div id="paper"><canvas id="cv" width="700" height="990">Seu navegador nao suporta canvas.</canvas></div>

<div id="toolbar">
  <!-- Row 1: Undo / Redo -->
  <div class="btn-grid">
    <button type="button" class="tool" id="undo">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M12.5 8c-2.65 0-5.05.99-6.9 2.6L2 7v9h9l-3.62-3.62c1.39-1.16 3.16-1.88 5.12-1.88 3.54 0 6.55 2.31 7.6 5.5l2.37-.78C21.08 11.03 17.15 8 12.5 8z"/></svg>
      Desfazer
    </button>
    <button type="button" class="tool" id="redo">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M18.4 10.6C16.55 8.99 14.15 8 11.5 8c-4.65 0-8.58 3.03-9.96 7.22l2.37.78C4.95 12.81 7.96 10.5 11.5 10.5c1.96 0 3.73.72 5.12 1.88L13 16h9V7l-3.6 3.6z"/></svg>
      Refazer
    </button>
  </div>

  <!-- Row 2: Brush / Eraser -->
  <div class="btn-grid">
    <button type="button" class="tool active" id="brush">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M7 14c-1.66 0-3 1.34-3 3 0 1.31-1.16 2-2 2 .92 1.22 2.49 2 4 2 2.21 0 4-1.79 4-4 0-1.66-1.34-3-3-3zm13.71-9.37l-1.34-1.34a.996.996 0 0 0-1.41 0L9 12.25 11.75 15l9.96-9.96c.39-.39.39-1.02 0-1.41z"/></svg>
      Pincel
    </button>
    <button type="button" class="tool" id="eraser">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M15.14 3c-.51 0-1.02.2-1.41.59L2.59 14.73c-.78.78-.78 2.05 0 2.83L6.44 21.4c.39.39.9.59 1.41.59h11.16v-2h-5.58l7.12-7.12c.78-.78.78-2.05 0-2.83l-3.99-3.99c-.39-.39-.9-.59-1.42-.59zM8.44 19.58l-3.56-3.57 6.42-6.42 3.56 3.57-6.42 6.42z"/></svg>
      Borracha
    </button>
  </div>

  <!-- Row 3: Circle / Square -->
  <div class="btn-grid">
    <button type="button" class="tool" id="circle">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M12 2C6.47 2 2 6.47 2 12s4.47 10 10 10 10-4.47 10-10S17.53 2 12 2zm0 18c-4.41 0-8-3.59-8-8s3.59-8 8-8 8 3.59 8 8-3.59 8-8 8z"/></svg>
      Circulo
    </button>
    <button type="button" class="tool" id="square">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M3 3v18h18V3H3zm16 16H5V5h14v14z"/></svg>
      Quadrado
    </button>
  </div>

  <!-- Row 4: Text -->
  <div class="btn-grid full">
    <button type="button" class="tool" id="text">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M5 4v3h5v12h3V7h5V4H5z"/></svg>
      Texto
    </button>
  </div>

  <!-- Large Gap -->
  <div class="gap-large"></div>

  <!-- Row 5: Clear -->
  <div class="btn-grid full">
    <button type="button" class="tool" id="clear">
      <svg width="16" height="16" viewBox="0 0 24 24"><path d="M15 16h4v2h-4zm0-8h7v2h-7zm0 4h5v2h-5zM3 18c0 1.1.9 2 2 2h6c1.1 0 2-.9 2-2V8H3v10zM14 5h-3l-1-1H6L5 5H2v2h12z"/></svg>
      Limpar
    </button>
  </div>

  <!-- Separator -->
  <div class="separator"></div>

  <!-- Sliders -->
  <div id="settings">
    <div class="setting">
      <label for="brushSize">Tamanho do pincel</label>
      <input type="range" id="brushSize" min="1" max="12" value="3">
      <span id="sizeValue">3</span>
    </div>
    <div class="setting">
      <label for="stabilizer">Estabilizador</label>
      <input type="range" id="stabilizer" min="0" max="90" value="55">
      <span id="smoothValue">55%</span>
    </div>
    <div class="setting">
      <label for="textSize">Tamanho do texto</label>
      <input type="range" id="textSize" min="7" max="40" value="14">
      <span id="textValue">14</span>
    </div>
    <div id="note">Desenho preto no papel branco. O tamanho do pincel altera a visualizacao.</div>
  </div>

  <div id="textHint">Toque no papel para posicionar o texto. Toque em Texto novamente para cancelar.</div>

  <!-- Large Gap -->
  <div class="gap-large"></div>

  <!-- Actions -->
  <div id="actions">
    <button type="button" id="send" class="action-btn">
      <svg width="18" height="18" viewBox="0 0 24 24"><path d="M9.5 5.5c1.1 0 2-.9 2-2s-.9-2-2-2-2 .9-2 2 .9 2 2 2zM5.75 8.9L3 23h2.1l1.75-8 2.15 2v6h2v-7.5l-2.1-2 .6-3c1.05 1.2 2.65 2 4.5 2v-2c-1.55 0-2.95-.8-3.75-2.1l-1-1.6c-.4-.65-1.1-1.1-1.9-1.1-.3 0-.5.05-.8.15L2 5.8V11h2V7.55l1.75-.65"/></svg>
      Fazer homing e desenhar
    </button>
    <button type="button" id="home" class="action-btn secondary">
      <svg width="18" height="18" viewBox="0 0 24 24"><path d="M10 20v-6h4v6h5v-8h3L12 3 2 12h3v8z"/></svg>
      Fazer apenas homing
    </button>
    <button type="button" id="reset" class="action-btn secondary" style="display:none;">Novo desenho / limpar papel</button>

    <div id="status">Pronto. Area A4: 210 x 297 mm.</div>
    <div id="progressWrap"><div id="progress"></div></div>
    <div id="jobControls">
      <button type="button" class="jobButton" id="pause">Pausar</button>
      <button type="button" class="jobButton" id="resume">Continuar</button>
      <button type="button" class="jobButton" id="cancel">Cancelar trabalho</button>
    </div>
  </div>
</div>

<!-- Internal Virtual Keyboard Modal -->
<div id="textModal">
  <div class="modal-content">
    <div class="modal-title">Digite o texto</div>
    <div id="textPreview"></div>
    <div id="virtualKeyboard">
      <div class="keyboard-row">
        <div class="key">1</div><div class="key">2</div><div class="key">3</div><div class="key">4</div><div class="key">5</div>
        <div class="key">6</div><div class="key">7</div><div class="key">8</div><div class="key">9</div><div class="key">0</div>
      </div>
      <div class="keyboard-row">
        <div class="key">Q</div><div class="key">W</div><div class="key">E</div><div class="key">R</div><div class="key">T</div>
        <div class="key">Y</div><div class="key">U</div><div class="key">I</div><div class="key">O</div><div class="key">P</div>
      </div>
      <div class="keyboard-row">
        <div class="key">A</div><div class="key">S</div><div class="key">D</div><div class="key">F</div><div class="key">G</div>
        <div class="key">H</div><div class="key">J</div><div class="key">K</div><div class="key">L</div>
      </div>
      <div class="keyboard-row">
        <div class="key">Z</div><div class="key">X</div><div class="key">C</div><div class="key">V</div><div class="key">B</div>
        <div class="key">N</div><div class="key">M</div><div class="key wide action-key" id="keyBksp">←</div>
      </div>
      <div class="keyboard-row">
        <div class="key wide action-key" id="keySpace">Espaço</div>
      </div>
    </div>
    <div class="modal-actions">
      <button type="button" class="modal-btn cancel" id="btnCancelText">Cancelar</button>
      <button type="button" class="modal-btn confirm" id="btnConfirmText">OK</button>
    </div>
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

var textModal=document.getElementById('textModal');
var textPreview=document.getElementById('textPreview');
var modalBuffer='';

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
var toolIds=['brush','eraser','circle','square','text'];

function setStatus(s){statusBox.textContent=s;}
function setTool(name){
  tool=name;
  var i,b;
  for(i=0;i<toolIds.length;i++){
    b=document.getElementById(toolIds[i]);
    if(b) b.className='tool'+(tool===toolIds[i]?' active':'');
  }
  if(name!=='text' && pendingText){
    pendingText='';
    document.getElementById('textHint').style.display='none';
  }
}

function copyStrokes(a){return JSON.parse(JSON.stringify(a));}
function remember(){
  undoStack.push(copyStrokes(strokes));
  if(undoStack.length>40)undoStack.shift();
  redoStack=[];
}

function redraw(){
  ctx.clearRect(0,0,canvas.width,canvas.height);
  ctx.fillStyle='#ffffff';
  ctx.fillRect(0,0,canvas.width,canvas.height);
  ctx.strokeStyle='#000000';
  ctx.fillStyle='#000000';
  ctx.lineCap='round';
  ctx.lineJoin='round';

  for(var i=0;i<strokes.length;i++) drawStroke(strokes[i]);
  if(shapePreview) drawStroke(shapePreview);
}

function drawStroke(s){
  var p=s.p;
  if(!p || !p.length)return;
  ctx.strokeStyle='#000000';
  ctx.fillStyle='#000000';
  ctx.lineWidth=s.w||brushSize;
  ctx.lineCap='round';
  ctx.lineJoin='round';
  if(p.length===1){
    ctx.beginPath();
    ctx.arc(p[0].x,p[0].y,(s.w||brushSize)/2,0,Math.PI*2,false);
    ctx.fill();
    return;
  }
  ctx.beginPath();
  ctx.moveTo(p[0].x,p[0].y);
  for(var i=1;i<p.length;i++) ctx.lineTo(p[i].x,p[i].y);
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
  var radius=20+brushSize*2;
  var removedAny=false;
  for(var i=strokes.length-1;i>=0;i--){
    var s=strokes[i];
    var hit=false;
    for(var j=0;j<s.p.length;j++){
      if(distance(q,s.p[j])<radius || (j>0 && pointLineDistance(q,s.p[j-1],s.p[j])<radius)){
        hit=true;
        break;
      }
    }
    if(hit){
      if(!eraseChanged){remember();eraseChanged=true;}
      strokes.splice(i,1);
      removedAny=true;
    }
  }
  if(removedAny||eraseChanged) redraw();
}

function linePoints(a,b){
  var d=distance(a,b),n=Math.max(1,Math.ceil(d/7)),p=[],i,t;
  for(i=0;i<=n;i++){t=i/n;p.push({x:a.x+(b.x-a.x)*t,y:a.y+(b.y-a.y)*t});}
  return p;
}

function shapePoints(name,a,b){
  var p=[],dx=b.x-a.x,dy=b.y-a.y,i,rx,ry,ang;
  if(name==='square'){
    var side=Math.min(Math.abs(dx),Math.abs(dy));
    dx=(dx<0?-1:1)*side;dy=(dy<0?-1:1)*side;
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
  if(tool==='square'||tool==='circle'){
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
  if(tool==='square'||tool==='circle'){
    if(shapeStart){shapePreview={w:brushSize,p:shapePoints(tool,shapeStart,q)};redraw();}
    return;
  }
  if(currentStroke)addBrushPoint(q);
}

function endDraw(q){
  if(!drawing)return;
  if(tool==='eraser'){drawing=false;eraseChanged=false;redraw();return;}
  if(tool==='square'||tool==='circle'){
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

/* Single-stroke Vector Font */
var TL=[0,0],TM=[1.5,0],TR=[3,0],uM=[1.5,1.25],ML=[0,2.5],MM=[1.5,2.5],MR=[3,2.5],BL=[0,5],BM=[1.5,5],BR=[3,5];
function seg(n){var s={top:[TL,TR],topleft:[TL,ML],topright:[TR,MR],middle:[ML,MR],bottomleft:[ML,BL],bottomright:[MR,BR],bottom:[BL,BR]};return s[n];}
var DIGITS={'0':['top','topleft','topright','bottomleft','bottomright','bottom'],'1':['topright','bottomright'],'2':['top','topright','middle','bottomleft','bottom'],'3':['top','topright','middle','bottomright','bottom'],'4':['topleft','topright','middle','bottomright'],'5':['top','topleft','middle','bottomright','bottom'],'6':['top','topleft','middle','bottomleft','bottomright','bottom'],'7':['top','topright'],'8':['top','topleft','topright','middle','bottomleft','bottomright','bottom'],'9':['top','topleft','topright','middle','bottomright','bottom']};
var LETTERS={A:[[BL,TM,BR],[ML,MR]],B:[[TL,BL],[TL,TR,MR,ML],[ML,BR,BL]],C:[[TR,TL,BL,BR]],D:[[TL,BL],[TL,TR,BR,BL]],E:[[TR,TL,BL,BR],[ML,MR]],F:[[BL,TL,TR],[ML,MR]],G:[[TR,TL,BL,BR,MR],[MM,MR]],H:[[TL,BL],[TR,BR],[ML,MR]],I:[[TL,TR],[TM,BM],[BL,BR]],J:[[TR,BR,BL]],K:[[TL,BL],[TR,ML,BR]],L:[[TL,BL,BR]],M:[[BL,TL,uM,TR,BR]],N:[[BL,TL,BR,TR]],O:[[TL,TR,BR,BL,TL]],P:[[BL,TL,TR,MR,ML]],Q:[[TL,TR,BR,BL,TL],[[1.8,3.5],BR]],R:[[BL,TL,TR,MR,ML],[ML,BR]],S:[[TR,TL,ML,MR,BR,BL]],T:[[TL,TR],[TM,BM]],U:[[TL,BL,BR,TR]],V:[[TL,BM,TR]],W:[[TL,BL,MM,BR,TR]],X:[[TL,BR],[TR,BL]],Y:[[TL,MM],[TR,MM],[MM,BM]],Z:[[TL,TR,BL,BR]],'.':[[[1.4,4.9],[1.6,5]]],',':[[[1.4,4.9],[1.7,5.3]]],'-':[[ML,MR]],':':[[[1.5,1.7],[1.5,1.8]],[[1.5,4.2],[1.5,4.3]]]};

function glyphStrokes(ch){if(ch===' ')return [];if(DIGITS[ch]){var a=[],i;for(i=0;i<DIGITS[ch].length;i++)a.push(seg(DIGITS[ch][i]));return a;}return LETTERS[ch]||[];}
function placeText(anchor){
  var scale = textHeight * 1.5;
  var x=anchor.x,y=anchor.y,unit=scale/5,w=unit*3,gap=unit*1.25,i,j,k,parts,src;
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

/* Modal Virtual Keyboard setup */
function openTextModal(){
  modalBuffer='';
  textPreview.textContent='_';
  textModal.style.display='flex';
}
function closeTextModal(){
  textModal.style.display='none';
}
var keys=document.querySelectorAll('.key');
for(var k=0;k<keys.length;k++){
  keys[k].onclick=function(){
    var id=this.id;
    if(id==='keyBksp'){
      if(modalBuffer.length>0) modalBuffer=modalBuffer.substring(0,modalBuffer.length-1);
    }else if(id==='keySpace'){
      if(modalBuffer.length<30) modalBuffer+=' ';
    }else{
      var txt=this.textContent;
      if(modalBuffer.length<30) modalBuffer+=txt;
    }
    textPreview.textContent=modalBuffer+'_';
  };
}
document.getElementById('btnCancelText').onclick=function(){
  closeTextModal();
  setTool('brush');
};
document.getElementById('btnConfirmText').onclick=function(){
  closeTextModal();
  if(modalBuffer.length>0){
    pendingText=modalBuffer.toUpperCase();
    setTool('text');
    document.getElementById('textHint').style.display='block';
    setStatus('Toque no papel para posicionar o texto.');
  }else{
    setTool('brush');
  }
};

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
document.getElementById('circle').onclick=function(){setTool('circle');};
document.getElementById('square').onclick=function(){setTool('square');};
document.getElementById('text').onclick=function(){
  if(pendingText){
    pendingText='';
    document.getElementById('textHint').style.display='none';
    setTool('brush');
    return;
  }
  openTextModal();
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
