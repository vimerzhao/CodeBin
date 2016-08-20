package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class Enemy {
	public 	int 	type;						//敌机的种类标识
	public 	Bitmap 	mEnemy;						//敌机图片资源
	public 	int 	x, y;						//敌机坐标
	public 	int 	frameW, frameH;				//敌机每帧的宽高
	private int 	frameIndex;					//敌机当前帧下标
	private int 	speed;						//敌机的移动速度
	public 	boolean isDead;						//判断敌机是否已经出屏
	//敌机的构造函数
	public Enemy(Bitmap mEnemy, int enemyType, int x, int y) {
		this.mEnemy	= 	mEnemy;
		frameW 		= 	mEnemy.getWidth() / 10;
		frameH 		=	mEnemy.getHeight();
		this.type 	= 	enemyType;
		this.x 		= 	x;
		this.y 		= 	y;
		switch (type) {
		case Constant.ENEMY_TYPE_FLY:				//苍蝇
			speed = Constant.ENEMY_FLY_SPEED;
			break;
		case Constant.ENEMY_TYPE_DUCKL:				//鸭子
			speed = Constant.ENEMY_DUCK_SPEED;
			break;
		case Constant.ENEMY_TYPE_DUCKR:									
			speed = Constant.ENEMY_DUCK_SPEED;
			break;
		}
		isDead =false;
	}

	//敌机绘图函数
	public void draw(Canvas canvas, Paint paint) {
		canvas.save();
		canvas.clipRect(x, y, x + frameW, y + frameH);
		canvas.drawBitmap(mEnemy, x - frameIndex * frameW, y, paint);
		canvas.restore();
	}

	//敌机逻辑AI
	public void logic() {
		frameIndex++;								//不断循环播放帧形成动画
		if (frameIndex >= 10) {
			frameIndex = 0;
		}
		//不同种类的敌机拥有不同的AI逻辑
		switch (type) {
		case Constant.ENEMY_TYPE_FLY:
			if (isDead == false) {
				//减速出现，加速返回
				speed -= 1;
				y += speed;
				if (y <= -200) {
					isDead = true;
				}
			}
			break;
		case Constant.ENEMY_TYPE_DUCKL:
			if (isDead == false) {
				//斜右下角运动
				x += speed / 2;
				y += speed;
				if (x > MySurfaceView.screenW) {
					isDead = true;
				}
			}
			break;
		case Constant.ENEMY_TYPE_DUCKR:
			if (isDead == false) {
				//斜左下角运动
				x -= speed / 2;
				y += speed;
				if (x < -50) {
					isDead = true;
				}
			}
			break;
		}
	}

	//判断碰撞(敌机与主角子弹碰撞)
	public boolean isCollsionWith(Bullet bullet) {
		int x2 = bullet.bulletX;
		int y2 = bullet.bulletY;
		int w2 = bullet.mBullet.getWidth();
		int h2 = bullet.mBullet.getHeight();
		if (x >= x2 && x >= x2 + w2) {
			return false;
		} else if (x <= x2 && x + frameW <= x2) {
			return false;
		} else if (y >= y2 && y >= y2 + h2) {
			return false;
		} else if (y <= y2 && y + frameH <= y2) {
			return false;
		}
		//发生碰撞，让其死亡
		isDead = true;
		return true;
	}
}
