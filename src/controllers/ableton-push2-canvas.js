// brew install pkg-config cairo pango libpng jpeg giflib librsvg
//const fs = require('fs');
const USB = require('usb');
const { createCanvas, loadImage } = require('canvas');
const { initPush, sendFrame } = require('ableton-push-canvas-display');

const canvas = createCanvas(960, 160)
const ctx = canvas.getContext('2d')

function drawFrame(ctx, frameNum) {
  ctx.strokeStyle = "#ff0"
  ctx.fillStyle = "#000"
  ctx.fillRect(0, 0, 960, 160)

  //ctx.fillStyle = "hsl(" + frameNum % 360 +",100%,50%)"
  ctx.lineWidth = 4

  ctx.font = '200 20px "SF Pro Display"';
  ctx.fillStyle = '#fff';
  ctx.textAlign = 'center'; 
  ctx.fillText("Made by Ableton", 480, 110);
  ctx.fillText("Powered by Flocking.js", 480, 140);


  for(let i = 0; i < 8; i++){
    dial(i, 1, "test");
  }
}

let frameNum = 0

function nextFrame() {
  drawFrame(ctx, frameNum)
  frameNum++
  sendFrame(ctx, function(error) {
    // we can ignore any error here, more or less
    setTimeout(nextFrame,10) // Do not use nextTick here, as this will not allow USB callbacks to fire.
  })
}

initPush(function(error) {
  if (error) {
    console.log(error)
  }

/*
  // readFileSync? 
  fs.readFile('test.json', 'utf-8', function(error, data){
	// JSON.parse(data);
	console.log(data);
	console.log(error);
  });
*/


  // It's ok to continue, as a Push 2 device may appear later
  nextFrame()
})


// n is the encoder starting at 0
function dial(n, val, label){
  let x = (960 / 8 ) * n + (960 / 16 );
  ctx.beginPath();
  ctx.arc(x, 50, 30, 0, 100);
  ctx.lineTo(x, 50);
  ctx.stroke();

  ctx.fillText(label, x, 70 );
}

