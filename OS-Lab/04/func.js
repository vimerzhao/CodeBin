// max x = 200;so x*3
function drawline(x1, y1, x2, y2, num) {
    var c=document.getElementById("canvas");
    var ctx=c.getContext("2d");

    x1 *= 3
    x2 *= 3
    y1 *= (400/num);
    y2 *= (400/num);
    ctx.fillStyle="#FF0000";
    ctx.fillRect(x1-5, y1-5, 10, 10);
    ctx.fillRect(x2-5, y2-5, 10, 10);
    ctx.moveTo(x1,y1);
    ctx.lineTo(x2,y2);
    ctx.stroke();
}
function draw() {
    var data = new String(document.getElementById("data").value);
    var xArr= new Array();
    var yArr= new Array();
    var arr = data.split(/\s+/);
    var max = -1;
    for (var i = 0; i < arr.length; i+=3) {
        yArr[i/3] = arr[i];
        xArr[i/3] = arr[i+1];
        max = arr[i] > max ? arr[i] : max;
    }
    for (var i = 0; i < xArr.length-1; ++i) {
        drawline(xArr[i], yArr[i], xArr[i+1], yArr[i+1], max);
    }
    console.log(arr.length);
    console.log(xArr);
    console.log(yArr);
}

