package com.example.snake;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import android.graphics.Point;

public class AiPlayer {
	private int[][] board;//游戏二维数组
	private Point mApplePoint;//食物位置
	private Point mHeadPoint;
	private Point mTailPoint;
	private int dir;//方向
	//这个四个量封装性不好，降低移植能力
	private final int D_UP = 100;
	private final int D_DOWN = 200;
	private final int D_LEFT = 300;
	private final int D_RIGHT = 400;
	//U D L R
	private int[][] D = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
	private int SIZE;//布局大小
	
	//标记
	private final int EMPTY = 0;
	private final int HEAD = 1;
	private final int BODY = 2;
	private final int TAIL= 3;
	private final int FOOD = 4;
	private final int BLOCK = 5;
	private final int INF = 1000000;
	private final int UN_REACHABLE = INF-100;
	
	
	private int[][] food_distance;//用于记录食物到每个位置的距离
	public AiPlayer(int size) {
		this.SIZE = size;
		board = new int[this.SIZE][this.SIZE];
		//初始化墙
		for (int i = 0; i < board.length; i++) {
			board[0][i] = board[SIZE-1][i] = BLOCK;
			board[i][0] = board[i][SIZE-1] = BLOCK;
		}
		
		
		food_distance = new int[SIZE][SIZE];
		mApplePoint = new Point();
		mHeadPoint  = new Point();
		mTailPoint  = new Point();
	}
	
	public void setData(final List<Point> snakeList,final int Direction, final Point Apple) {
		
		for (int i = 1; i < board.length-1; i++) {
			for (int j = 1; j < board[0].length - 1; j++) {
				board[i][j] = EMPTY;
			}
		}
		for (Point point:snakeList) {
			board[point.y][point.x] = BODY;
		}	

		//注意不能直接赋值对象
		mApplePoint.x = Apple.x;
		mApplePoint.y = Apple.y;
		mHeadPoint.x  = snakeList.get(0).x;
		mHeadPoint.y  = snakeList.get(0).y;
		mTailPoint.x  = snakeList.get(snakeList.size()-1).x;
		mTailPoint.y  = snakeList.get(snakeList.size()-1).y;
		
		board[mApplePoint.y][mApplePoint.x] = FOOD;
		board[mHeadPoint.y][mHeadPoint.x]   = HEAD;
		board[mTailPoint.y][mTailPoint.x]   = TAIL;		

		dir = Direction;
	}
	
	//最简单的bfs找到一条路即可，对食物
	public int getDirection() {
		BFS();
		if (!getMin()) {
			followTail();
		}
		return this.dir;
	}

	private int translateDirection(int i) {
		switch (i) {
		case 0:
			return D_UP;
		case 1:
			return D_DOWN;
		case 2:
			return D_LEFT;
		case 3:
			return D_RIGHT;
		default:
			return D_DOWN;
		}
		
	}
	private void BFS() {
		//初始化，每次依据board刷新
		for (int i = 0; i < food_distance.length; i++) {
			for (int j = 0; j < food_distance[0].length; j++) {
				if (EMPTY == board[i][j]) {
					food_distance[i][j] = UN_REACHABLE;
				} else {
					food_distance[i][j] = INF;
				}
			}
		}
		food_distance[mApplePoint.y][mApplePoint.x] = 1;
		
		//BFS
		Point temp = new Point(0, 0);
		Point cur  = new Point(0, 0);
		cur.x = mApplePoint.x; cur.y = mApplePoint.y;
		List<Point> Q = new LinkedList<Point>();
		Q.clear();
		Q.add(cur);
		while (!Q.isEmpty()) {
			cur.x = Q.get(0).x; cur.y = Q.get(0).y;
			Q.remove(0);
			for (int i = 0; i < 4; i++) {
				temp.x = cur.x+D[i][0];
				temp.y = cur.y+D[i][1];
				if (temp.x < 0 || temp.x >= SIZE || temp.y < 0 || temp.y >= SIZE) {
					continue;
				}
				if (food_distance[temp.y][temp.x] == UN_REACHABLE) {
					food_distance[temp.y][temp.x] = food_distance[cur.y][cur.x] + 1;
					Q.add(new Point(temp.x, temp.y));
				}
			}
		}
	}
	private boolean getMin() {
		Point temp = new Point();
		int MAX = INF;
		for (int i =0; i < 4; i++) {
			temp.x = mHeadPoint.x+D[i][0];
			temp.y = mHeadPoint.y+D[i][1];
			if (temp.x < 0 || temp.x >= SIZE || temp.y < 0 || temp.y >= SIZE) {
				continue;
			}
			if (food_distance[temp.y][temp.x] < MAX) {
				MAX = food_distance[temp.y][temp.x];
				this.dir = translateDirection(i);
			}
		}
		if (MAX == INF) {
			return false;
		}
		return true;
	}
	
	private void followTail() {
		Point temp = new Point();
		for (int i =0; i < 4; i++) {
			temp.x = mHeadPoint.x+D[i][0];
			temp.y = mHeadPoint.y+D[i][1];
			if (temp.x < 0 || temp.x >= SIZE || temp.y < 0 || temp.y >= SIZE) {
				continue;
			}
			if (food_distance[temp.y][temp.x] != INF) {
				this.dir = translateDirection(i);
				return;
			}
		}
		
	}
}
