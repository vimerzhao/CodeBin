window.onload = function() {
	//获取内容
	var canvas = document.getElementById("viewport");
	var run = document.getElementById("start");
	var clear = document.getElementById("clear");
	var context = canvas.getContext("2d");

	// 计时变量
	var lastframe = 0;
	var fpstime = 0;
	var framecount = 0;
	var fps = 0;

	// 是否可达
	var isReachable = false;
	// 

	// 状态数组：0-空，1-入口、出口，2-路径，3-ClOSE表，4-OPEN表,5-障碍
	var state = [];

	//起点、终点
	var start_x = 0;
	var start_y = 0;
	var end_x = 20;
	var end_y = 13;



	//初始化
	function init() {
		//添加鼠标单击事件
		canvas.addEventListener("mousedown", onMouseDown);
		run.addEventListener("click", findPath);
		clear.addEventListener("click", function() {
			for (var x = 0; x < 21; x++) {
				for (var y = 0; y < 14; y++) {
					if (5 != state[x][y] && 1 != state[x][y]) {
						state[x][y] = 0;
					}
				}
			}
		});
		//初始化状态数组
		for (var i = 0; i < 21; i++) {
			state[i] = [];
			for (var j = 0; j < 14; j++) {
				state[i][j] = 0;
			}
		}

		state[start_x][start_y] = 1;
		state[end_x][end_y] = 1;


		main(0);
	}

	//主循环
	function main(tframe) {
		// 重绘
		window.requestAnimationFrame(main);

		update(tframe);
		render();
	}

	function update(tframe) {
		var dt = (tframe - lastframe) / 1000;
		lastframe = tframe;

		updateFps(dt);
	}

	function updateFps(dt) {
		if (fpstime > 0.25) {
			fps = Math.round(framecount/fpstime);
			fpstime = 0;
			framecount = 0;
		}

		fpstime += dt;
		framecount++;
	}

	function render() {
		drawFrame();
	}

	function drawFrame() {

        context.fillStyle = "#d0d0d0";
        context.fillRect(0, 0, canvas.width, canvas.height);
        context.fillStyle = "#e8eaec";
        context.fillRect(1, 1, canvas.width-2, canvas.height-2);
        
        context.fillStyle = "#303030";
        context.fillRect(0, 0, canvas.width, 65);
        
        // Draw title
        context.fillStyle = "#ffffff";
        context.font = "24px Verdana";
        context.fillText("A*寻路算法演示---zhaoyu1995", 10, 30);
        
        context.fillStyle = "#ffffff";
        context.font = "12px Verdana";
        context.fillText("Fps: " , 13, 50);

        //绘制网格
        context.lineWidth = 0.5;
        context.strokeStyle = "#000";
        for (var y = 65; y <= canvas.height; y += 30) {
        	context.beginPath();
        	context.moveTo(0,y);
        	context.lineTo(640,y);
        	context.stroke();
        }
        for (var x = 0; x <= canvas.width; x += 30) {
        	context.beginPath();
        	context.moveTo(x,65);
        	context.lineTo(x,485);
        	context.stroke();
        }

        //根据状态数组绘制方块
        context.fillStyle = "#e8eaec";
        for (var x = 0; x < 21; x++) {
        	for (var y = 0; y < 14; y++) {
        		if (1 == state[x][y]) {
			        context.fillStyle = "#0F0";
        		} else if (2 == state[x][y]) {
			        context.fillStyle = "#FF0000";
        		} else if (3 == state[x][y]) {
			        context.fillStyle = "#191970";
        		} else if (4 == state[x][y]) {
			        context.fillStyle = "#40E0D0";
        		}	else if (5 == state[x][y]) {
			        context.fillStyle = "#777";
        		}
        		context.fillRect(x*30+1, 66+y*30, 28, 28);
        		 context.fillStyle = "#e8eaec";
        	}
        }
	}

	function onMouseDown(e) {
		var pos = getMousePos(canvas,e);		
		var x = Math.floor(pos.x/30);
		var y = Math.floor((pos.y-65)/30);
		//如果为空则设置为障碍，如果为障碍则设置为空，注意这里的逻辑
		if (0 == state[x][y]) {
			state[x][y] = 5;
		} else if (5 == state[x][y]) {
			state[x][y] = 0;
		}
	}
    function getMousePos(canvas, e) {
    
    var rect = canvas.getBoundingClientRect();
	    return {
	        x: Math.round((e.clientX - rect.left)/(rect.right - rect.left)*canvas.width),
	        y: Math.round((e.clientY - rect.top)/(rect.bottom - rect.top)*canvas.height)
	    };
	}

	findPath = function(){
		//A*寻路算法，核心代码
		var openList = [],//open表
			closeList = [],//close表
			result = [],//辅助变量
			result_index = 0;

		//把入口加入open表
		openList.push({x: start_x, y:start_y, G:0});
		do {
			// 获取当前点
			var currentPoint = openList.pop();

			// 加入close表
			closeList.push(currentPoint);

			//如果不是起点或者重点则标记为搜索过的点，方便后面显示
			if ((currentPoint.x != start_x || currentPoint.y != start_y) && 
				(currentPoint.x != end_x || currentPoint.y != end_y)) {
				state[currentPoint.x][currentPoint.y] = 3;	
			}
			
			//拓展当前节点，即上下左右四个邻居节点
			var surroundPoint = getSurroundPoint(currentPoint);

			//遍历邻居节点
			for (var i in surroundPoint) {
				var item = surroundPoint[i];
				if (item.x >= 0 &&	//判断是否在边界内 
					item.y >= 0 &&
					item.x < 21 &&
					item.y < 14 &&
					state[item.x][item.y] != 5 &&	//判断是否是障碍
					!existInList(item, closeList)) {//判断是否在close表中
					
					//更新实际代价 G
					var g = currentPoint.G + 1;

					//判断是否已经存在open表中
					if (!existInList(item, openList)) {
						// 如果不存在则计算启发值以及实际代价
						// 1.2表示贪心程度，可以自由设置，越大则越接近贪心算法
						item['H'] = 1.2*(Math.abs(end_x-item.x)+Math.abs(end_y-item.y));
						item['G'] = g;
						item['F'] = item.G + item.H;

						//注意记录其父亲节点，方便后面显示路径
						item['parent'] = currentPoint;
						
						// 加入open表
						openList.push(item);

						// 如果不是起点或者终点就标记一下，方便后面显示
						if ((currentPoint.x != start_x || currentPoint.y != start_y) && 
							(currentPoint.x != end_x || currentPoint.y != end_y)) {
							state[currentPoint.x][currentPoint.y] = 4;	
						}

					} else {
						// 如果已经在open表中，则比较新的代价是否更低，更低则更新open表
						var index = existInList(item, openList);
						if (g < openList[index].G) {
							// 更新父亲节点
							openList[index].parent = currentPoint;
							openList[index].G = g;

							// 更新代价
							openList[index].F = g + openList[index].H;
						}
					}
				}
			}

			// 如果open表为空，则跳出
			if (openList.length == 0) {
				break;
			}

			// 根据F值重排open表
			openList.sort(sortF);

		// 如果终点找到则结束循环
		} while(!(result_index = existInList({x:end_x, y:end_y}, openList)));
		
		//如果没找到路径，则提示没找到
		if (!result_index) {
			result = [];
			alert("找不到路径");
		} else {
			// 如果找到路径则存如result数组中
			var currentObject = openList[result_index];
			do {
				result.unshift({x:currentObject.x, y:currentObject.y});
				currentObject = currentObject.parent;
			} while (currentObject.x != start_x || currentObject.y != start_y);
		}

		// 弹出终点方便后面绘图
		result.pop();

		// 更新状态数组，方便绘制路径
		for (var i in result) {
			var item = result[i];
			state[item.x][item.y] =2;
		}
	}

	function sortF(a, b) {
		return b.F - a.F;
	}
	function getSurroundPoint(currentPoint) {
		var x = currentPoint.x, y = currentPoint.y;
		return [{x:x-1, y:y},
				{x:x+1, y:y},
				{x:x, y:y-1},
				{x:x, y:y+1}];
	}
	function existInList(point,list) {
	    for(var i in list) {
	        if(point.x==list[i].x && point.y==list[i].y) {
	            return i;
	        }
	    }
	    return false;
	}
    init();
}