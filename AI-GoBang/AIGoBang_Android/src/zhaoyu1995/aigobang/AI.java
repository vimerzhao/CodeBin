package zhaoyu1995.aigobang;

import android.graphics.Point;

public class AI {
	boolean wins[][][] = new boolean[15][15][600];//赢法数组，统计所有可能的获胜策略
	private int winCount;//统计获胜策略数量，可以计算得到是572种
	private int board[][] = new int[15][15];//记录棋盘局势的二维数组，0：无棋子，1：，黑棋， 2：白棋
	//AI负责统计玩家和自己在每一种赢法上的积累，达到五则获胜
	private int playerWin[] = new int[600];
	private int aiWin[] = new int[600];
	private int gameType;//0未结束；1黑子胜；2白子胜 
	
	public AI () {
		init();
	}
	
	public void init(){
		winCount = 0;
		gameType = 0;
		//统计所有的获胜方法
		//横
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 11; j++) {
				for (int k = 0; k < 5; k++) {
					wins[i][j+k][winCount] = true;
				}
				winCount++;
			}
		}
		//竖
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 15; j++) {
				for (int k = 0; k < 5; k++) {
					wins[i+k][j][winCount] = true;
				}
				winCount++;
			}
		}
		//斜
		for (int i = 0; i < 11; i++){
			for (int j = 0; j < 11; j++){
				for (int k = 0; k < 5; k++){
					wins[i+k][j+k][winCount] = true;
				}
				winCount++;
			}
		}
		for (int i = 0; i < 11; i++){
			for (int j = 14; j > 3; j--){
				for (int k = 0; k < 5; k++){
					wins[i+k][j-k][winCount] = true;
				}
				winCount++;
			}
		}
//		//初始时刻双方都没有积累
//		Arrays.fill(aiWin, 0);
//		Arrays.fill(playerWin, 0);
//		//棋盘没有落子
//		Arrays.fill(board, 0);	
		for (int k = 0; k < winCount; k++) {
			aiWin[k] = playerWin[k] = 0;
		}
		for (int i = 0; i < 15; i++){
			for (int j = 0; j < 15; j++){
				board[i][j] = 0;
			}
		}			
	}

	//CheckerBoard没有实例化，如何获取棋数据？？？以后学习中留心这个问题，数据的传输
		
	//根据收集的信息落子
	public Point getAnswer() {
		int x = 0;
		int y = 0;
		int max = -1;
		//统计玩家和AI在每个位置的得分，初始为0
		int playerScore[][] = new int[15][15];
		int aiScore[][] = new int[15][15];
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				playerScore[i][j] = 0;
				aiScore[i][j] = 0;
			}
		}
		
		//为每个位置打分
		for (int i = 0; i < 15; i++) {
			for (int j = 0; j < 15; j++) {
				if (0 == board[i][j]) {//如果是空位
					for (int k = 0; k < winCount; k++) {
						//查看每种赢法完成的程度
						if (wins[i][j][k]) {
							//如果该位置对第k中赢法有帮助
							if (1 == playerWin[k]) {
								playerScore[i][j]+=200;
							} else if (2 == playerWin[k]) {
								playerScore[i][j]+=400;
							} else if (3 == playerWin[k]) {
								playerScore[i][j]+=2000;
							} else if (4 == playerWin[k]) {
								playerScore[i][j]+=10000;
							}
							
							if (1 == aiWin[k]) {
								aiScore[i][j]+=320;
							} else if (2 == aiWin[k]) {
								aiScore[i][j]+=420;
							} else if (3 == aiWin[k]) {
								aiScore[i][j]+=4200;
							} else if (4 == aiWin[k]) {
								aiScore[i][j]+=20000;
							}
						}
					}
				}//if
				//寻找最应该落子的位置
				if (playerScore[i][j] > max) {
					max = playerScore[i][j];
					x = j;
					y = i;//注意坐标系
				} else if (playerScore[i][j] == max) {
					//容易遗漏
					if (aiScore[i][j] > aiScore[y][x]) {
						x = j;
						y = i;
					}
				}//if
				if (aiScore[i][j] > max) {
					max = aiScore[i][j];
					x = j;
					y = i;
				} else if (aiScore[i][j] == max) {
					if (playerScore[i][j] > playerScore[y][x]) {
						x = j;
						y = i;
					}
				}//if
				
			}
		}//for
		//遍历结束，返回最佳落子点
		
		if (100 > max) {
			x = (int)(Math.random()*15);
			y = (int)(Math.random()*15);
		}
		return new Point(x, y);
	}
	//获取落子位置，和落子类型，更新赢法统计数组
	
	public void updateWinCount(Point point, int type) {
		//更新board
		board[point.y][point.x] = type;
		
		if (1 == type) {
			//黑子
			for (int k = 0; k < winCount; k++) {
				if (wins[point.y][point.x][k]) {
					//该位置对第k种赢法有贡献
					playerWin[k]++;
					aiWin[k] = -1;//对手不可能通过这个方法获胜，置为异常
					if (5 == playerWin[k]) {
						gameType = 1;
						return;
					}
				}
			}
		} else if (2 == type) {
			//白子
			for (int k = 0; k < winCount; k++) {
				if (wins[point.y][point.x][k]) {
					aiWin[k]++;
					playerWin[k] = -1;
					if (5 == aiWin[k]) {
						gameType = 2;
						return;
					}
				}
			}
		}
	}
	//利用AI的board检查胜负, 0:未结束；1：黑子胜利；2：白子胜；3：平局
	public int isGmaeOver() {
		//检查是否平局
		if (0 == gameType) {
			gameType = 3;
			ALL:
			for (int i = 0; i < 15; i++) {
				for (int j = 0; j < 15; j++) {
					if (0 == board[i][j]) {
						gameType = 0;
						break ALL;
					}
				}
			}
		}
		return this.gameType;
	}
	public int[][] getBoard() {
		return board;
	}
}
