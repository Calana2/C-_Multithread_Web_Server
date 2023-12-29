// Your javascript code here
function start()
{
 let colorizedText = document.getElementById("colorflush");
 setInterval(function() {colorChange(colorizedText);},500);
}

function colorChange(text)
{
 const color = '#' + Math.floor(Math.random()*16777215).toString(16);
 text.style.color = color; console.log(color);       
}

window.addEventListener('load',start,false);
