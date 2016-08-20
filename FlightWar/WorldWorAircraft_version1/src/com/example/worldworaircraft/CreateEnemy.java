package com.example.worldworaircraft;

import java.util.Random;

public class CreateEnemy {
	private static Random random = new Random();
	public static int[][] create() {
		int[] e = {Constant.ENEMY_TYPE_FLY, Constant.ENEMY_TYPE_DUCKL, Constant.ENEMY_TYPE_DUCKR};
		int[][] p = new int[Constant.ENEMY_NUM][Constant.ENEMY_NUM_KIND];
		for (int i = 0; i < Constant.ENEMY_NUM; i++) {
			for (int j = 0; j < Constant.ENEMY_NUM_KIND; j++) {
				p[i][j] = e[random.nextInt(3)];
			}	
		}
		return p;
	}
}
