package com.example.worldworaircraft;

public class Constant {
	//定义游戏中的所有常量，便于管理
	//定义游戏状态常量
	public static final int GAME_MENU  		= 1;			//游戏菜单
	public static final int GAME_RUN   		= 2;			//游戏中
	public static final int GAME_WIN   		= 3;			//游戏胜利
	public static final int GAME_LOST  		= 4;			//游戏失败
	public static final int GAME_PAUSE 		= 5;			//游戏菜单
	public static final int BULLET_PLAYER	= 6;			//主角的
	public static final int BULLET_DUCK		= 7;			//鸭子的	
	public static final int BULLET_FLY  	= 8;			//苍蝇的
	public static final int BULLET_BOSS 	= 9;			//Boss的
	public static final int DIR_UP			= 10;			//Boss子弹的8方向常量
	public static final int DIR_DOWN 		= 11;
	public static final int DIR_LEFT 		= 12;
	public static final int DIR_RIGHT 		= 13;
	public static final int DIR_UP_LEFT 	= 14;
	public static final int DIR_UP_RIGHT	= 15;
	public static final int DIR_DOWN_LEFT	=16;
	public static final int DIR_DOWN_RIGHT	=17;
	public static final int ENEMY_TYPE_FLY	= 18;			//苍蝇
	public static final int ENEMY_TYPE_DUCKL= 19;			//鸭子(从左往右运动)
	public static final int ENEMY_TYPE_DUCKR= 20;			//鸭子(从右往左运动)
	
	public static final int BULLET_PLAYER_SPEED = 15;		//各类型子弹速度
	public static final int BULLET_DUCK_SPEED 	= 4;
	public static final int BULLET_FLY_SPEED	= 6;
	public static final int BULLET_BOSS_SPEED	= 10;
	
	public static final int ENEMY_FLY_SPEED		= 15;		//敌机速度
	public static final int ENEMY_DUCK_SPEED	= 5;
	public static final int ENEMY_BOSS_SPEED	= 5;
	
	public static final int CREATE_ENEMY_TIME	= 50;		//每次生成敌机的时间(毫秒)
	public static final int SLEEP_TIME 			= 60;		//线程挂起时间
	public static final int BG_SPEED   			= 3;		//游戏背景滚动速度
	public static final int NO_DIE_TIME 		= 50;		//玩家保持无敌的时间
	public static final int BUFFER_DIS			= 5;		//手指滑动的缓冲距离
	public static final int MATCH_DIS			= 240;		//游戏背景的重合距离，使得背景衔接更真实，需手动调节
	public static final int ENEMY_NUM 			= 10;		//产生怪物的行数和列数
	public static final int ENEMY_NUM_KIND 		= 3;
	public static final int BOSS_BLOOD 			= 10;		//Boss血量
}
