package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.MotionEvent;

public class Player {
	//主角的血量与血量位图
	private int 	playerBlood = 3;							//默认3血
	private Bitmap 	mPlayerBlood;
	public int 		x, y;										//主角的坐标以及位图
	private Bitmap 	mPlayer;

	//碰撞后处于无敌时间
	private int 	noCollisionCount = 0;						//计时器
	private int 	noCollisionTime = Constant.NO_DIE_TIME;
	//是否碰撞的标识位
	private boolean isCollision;

	//主角的构造函数
	public Player(Bitmap mPlayer, Bitmap mPlayerBlood) {
		this.mPlayer 		= 	mPlayer;
		this.mPlayerBlood 	= 	mPlayerBlood;
		x 					= 	MySurfaceView.screenW / 2 - mPlayer.getWidth() / 2;
		y 					= 	MySurfaceView.screenH - mPlayer.getHeight();	//底部
	}

	//主角的绘图函数
	public void draw(Canvas canvas, Paint paint) {
		//绘制主角
		//当处于无敌时间时，让主角闪烁
		if (isCollision) {
			if (noCollisionCount % 2 == 0) {				//每2次游戏循环，绘制一次主角
				canvas.drawBitmap(mPlayer, x, y, paint);	//闪烁效果
			}
		} else {
			canvas.drawBitmap(mPlayer, x, y, paint);
		}
		//绘制主角血量
		for (int i = 0; i < playerBlood; i++) {
			canvas.drawBitmap(mPlayerBlood, i * mPlayerBlood.getWidth(), 
					MySurfaceView.screenH - mPlayerBlood.getHeight(), paint);
		}
	}



	//主角的逻辑
	public void logic() {
		//判断屏幕X边界
		if (x + mPlayer.getWidth() >= MySurfaceView.screenW) {
			x = MySurfaceView.screenW - mPlayer.getWidth();
		} else if (x <= 0) {
			x = 0;
		}
		//判断屏幕Y边界
		if (y + mPlayer.getHeight() >= MySurfaceView.screenH) {
			y = MySurfaceView.screenH - mPlayer.getHeight();
		} else if (y <= 0) {
			y = 0;
		}
		//处理无敌状态
		if (isCollision) {
			noCollisionCount++;					//计时器开始计时
			if (noCollisionCount >= noCollisionTime) {
				isCollision = false;			//无敌时间过后，接触无敌状态及初始化计数器
				noCollisionCount = 0;
			}
		}
	}
	
	public void onTouchEvent(MotionEvent event) {
		int touchX	= (int) event.getX();
		int touchY	= (int) event.getY();
		//距离太远移动无效
		if (Math.abs(touchX-x) > MySurfaceView.screenW/8 ||
				Math.abs(touchY-y) > MySurfaceView.screenH/8) {
			return;
		}
			
		if(event.getAction() == MotionEvent.ACTION_MOVE) {
			x = (int) event.getX();
			y = (int) event.getY();
		}

	}
	
	//设置主角血量
	public void setPlayerBlood(int Blood) {
		this.playerBlood = Blood;
	}

	//获取主角血量
	public int getPlayerBlood() {
		return playerBlood;
	}

	//判断碰撞(主角与敌机)
	public boolean isCollsionWith(Enemy en) {
		if (isCollision == false) {			//是否处于无敌时间
			int x2 = en.x;
			int y2 = en.y;
			int w2 = en.frameW;
			int h2 = en.frameH;
			if (x >= x2 && x >= x2 + w2) {
				return false;
			} else if (x <= x2 && x + mPlayer.getWidth() <= x2) {
				return false;
			} else if (y >= y2 && y >= y2 + h2) {
				return false;
			} else if (y <= y2 && y + mPlayer.getHeight() <= y2) {
				return false;
			}
			isCollision = true;				//碰撞即进入无敌状态
			return true;
		} else {							//处于无敌状态，无视碰撞
			return false;
		}
	}
	//判断碰撞(主角与敌机子弹)
	public boolean isCollsionWith(Bullet bullet) {
		if (isCollision == false) {							//是否处于无敌时间
			int x2 = bullet.bulletX;
			int y2 = bullet.bulletY;
			int w2 = bullet.mBullet.getWidth();
			int h2 = bullet.mBullet.getHeight();
			if (x >= x2 && x >= x2 + w2) {
				return false;
			} else if (x <= x2 && x + mPlayer.getWidth() <= x2) {
				return false;
			} else if (y >= y2 && y >= y2 + h2) {
				return false;
			} else if (y <= y2 && y + mPlayer.getHeight() <= y2) {
				return false;
			}
			isCollision = true;								//碰撞即进入无敌状态
			return true;
		} else {											//处于无敌状态，无视碰撞
			return false;
		}
	}
}
