/********************************************/
/*变量声明：chessBoard记录棋盘的二维数组	*/
/*			me         控制落子逻辑         */
/*			over  	   判断是否结束         */
/********************************************/
var chessBoard = [];
var me = true;
var over = false;
//赢法数组
var wins = [];
//赢法的统计数组
var myWin = [];
var computerWin = [];
//初始化
for (var i = 0; i < 15; i++){
	chessBoard[i] = [];
	for (var j = 0; j < 15; j++){
		chessBoard[i][j] = 0;
	}
}
for (var i = 0; i < 15; i++){
	wins[i] = [];
	for (var j = 0; j < 15; j++){
		wins[i][j] = []; 
	}
}
var count = 0;						//记录所有可能的赢法
//统计所有可能的赢法,需要好好理解
for (var i = 0; i < 15; i++){
	for (var j = 0; j < 11; j++){
		for (var k = 0; k < 5; k++){
			wins[i][j+k][count] = true;
		}
		count++;
	}
}
for (var i = 0; i < 11; i++){
	for (var j = 0; j < 15; j++){
		for (var k = 0; k < 5; k++){
			wins[i+k][j][count] = true;
		}
		count++;
	}
}
for (var i = 0; i < 11; i++){
	for (var j = 0; j < 11; j++){
		for (var k = 0; k < 5; k++){
			wins[i+k][j+k][count] = true;
		}
		count++;
	}
}
for (var i = 0; i < 11; i++){
	for (var j = 14; j > 3; j--){
		for (var k = 0; k < 5; k++){
			wins[i+k][j-k][count] = true;
		}
		count++;
	}
}
//共 572 赢法

//初始化
for (var i = 0; i < count; i++){
	myWin[i] = 0;
	computerWin[i] = 0;
}

var chess = document.getElementById('chess');
var context = chess.getContext('2d');
context.strokeStyle = "#AFAFAF";
var logo = new Image();
logo.src = "images/logo.png";
logo.onload = function() {
	context.drawImage(logo, 0, 0, 450, 450);
	drawChessBoard();
}
/********************************************/
/*功能：	绘制棋盘						*/
/********************************************/
var drawChessBoard = function() {
	for (var i = 0; i < 15; i++) {
		context.moveTo(15 + i*30, 15);
		context.lineTo(15 + i*30, 435);
		context.moveTo(15, 15 + i*30);
		context.lineTo(435, 15 + i*30);
		context.stroke();
	}
}
/********************************************/
/*功能： 	根据鼠标点击，进行相应落子		*/
/********************************************/
var oneStep = function(i, j, me) {
	context.beginPath();
	context.arc(15 + i*30, 15 + j*30, 13, 0, 2*Math.PI);
	context.closePath();
	var gradient = context.createRadialGradient(15 + i*30 + 2, 15 + j*30 - 2, 13, 15 + i*30 + 2, 15 + j*30 - 2, 0);	
	if (me){
		gradient.addColorStop(0, "#0A0A0A");
		gradient.addColorStop(1, "#636766");
	} else {
		gradient.addColorStop(0, "#D1D1D1");
		gradient.addColorStop(1, "#F9F9F9");		
	}
	context.fillStyle = gradient;
	context.fill();
}
/********************************************/
/*功能：	根据落子结果进行逻辑交互		*/
/********************************************/
chess.onclick = function(e){
	if (over){
		return; 
	}
	if (!me){
		return;
	}
	var x = e.offsetX;
	var y = e.offsetY;
	var i = Math.floor(x / 30);
	var j = Math.floor(y / 30);
	if (0 == chessBoard[i][j]){
		oneStep(i, j, me);
		chessBoard[i][j] = 1;
		//更新赢法统计数组
		for (var k = 0; k < count; k++){
			if (wins[i][j][k]){
				myWin[k]++;//接近胜利
				computerWin[k] = 6;//设置为异常值，因为最多加到5
				if (5 == myWin[k]){
					window.alert("You beat my stupid algorithm");
					over = true;
				}
			}
		}
		if (!over){
			computerAI();
			me = !me;
		}
	}
}
/********************************************/
/*功能：	核心，计算机 AI 算法			*/
/********************************************/
var computerAI = function() {
	var myScore = [];
	var computerScore = [];
	var max = -1;
	var u = 0, v = 0;
	//初始化
	for (var i = 0; i < 15; i++){
		myScore[i] = [];
		computerScore[i] = [];
		for (var j = 0; j < 15; j++){
			myScore[i][j] = 0;
			computerScore[i][j] = 0;
		}
	}
	//evaluate
	for (var i = 0; i < 15; i++){
		for (var j = 0; j < 15; j++){
			if (0 == chessBoard[i][j]){//no chessman
				for (var k = 0; k <count; k++){
					if (wins[i][j][k]){
						if (1 == myWin[k]){
							myScore[i][j] += 200;
						} else if (2 == myWin[k]){
							myScore[i][j] += 400;
						} else if (3 == myWin[k]){
							myScore[i][j] += 2000;
						} else if (4 == myWin[k]){
							myScore[i][j] += 10000;
						}
						if (1 == computerWin[k]){
							computerScore[i][j] += 320;
						} else if (2 == computerWin[k]){
							computerScore[i][j] += 420;
						} else if (3 == computerWin[k]){
							computerScore[i][j] += 4200;
						} else if (4 == computerWin[k]){
							computerScore[i][j] += 20000;
						}
					}
				}
				if (myScore[i][j] > max){
					max = myScore[i][j];
					u = i;
					v = j;
				} else if (myScore[i][j] == max){
					if (computerScore[i][j] > computerScore[u][v]){
						u = i;
						v = j;
					}
				}
				if (computerScore [i][j] > max){
					max = computerScore[i][j];
					u = i;
					v = j;
				} else if (computerScore[i][j] == max){
					if (myScore[i][j] > myScore[u][v]){
						u = i;
						v = j;
					}
				}
			}
		}
	}
	oneStep(u, v, false);
	chessBoard[u][v]  = 2;
	//更新赢法统计数组
	for (var k = 0; k  < count; k++){
		if (wins[u][v][k]){
			computerWin[k]++;//接近胜利
			myWin[k] = 6;//设置为异常值，因为最多加到5
			if (5 == computerWin[k]){
				window.alert("stupid guy");
				over = true;
			}
		}
	}
	if (!over){
		me = !me;
	}	
}

















