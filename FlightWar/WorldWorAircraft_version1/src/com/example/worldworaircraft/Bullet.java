package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class Bullet {
	public	Bitmap 	mBullet;					//子弹图片资源
	public	int		bulletX, bulletY;			//子弹的坐标
	public	int 	speed;						//子弹的速度
	public	int 	bulletType;					//子弹的种类以
	public	boolean isDead;						//子弹是否超屏， 优化处理
	private int 	dirBoss;					//当前Boss子弹方向
	public Bullet(Bitmap mBullet, int bulletX, int bulletY, int bulletType) {
		this.mBullet 	= 	mBullet;
		this.bulletX 	= 	bulletX;
		this.bulletY 	= 	bulletY;
		this.bulletType =	bulletType;
		switch (bulletType) {
		case Constant.BULLET_PLAYER:
			speed = Constant.BULLET_PLAYER_SPEED;
			break;
		case Constant.BULLET_DUCK:
			speed = Constant.BULLET_DUCK_SPEED;
			break;
		case Constant.BULLET_FLY:
			speed = Constant.BULLET_FLY_SPEED;
			break;
		case Constant.BULLET_BOSS:
			speed = Constant.BULLET_BOSS_SPEED;
			break;
		}
	}
	public Bullet(Bitmap mBullet, int bulletX, int bulletY, int bulletType, int dir) {
		this.mBullet 	= 	mBullet;
		this.bulletX 	= 	bulletX;
		this.bulletY 	= 	bulletY;
		this.bulletType = 	bulletType;
		speed 			=	5;
		this.dirBoss 	= 	dir;
	}
	public void draw(Canvas canvas, Paint paint) {
		canvas.drawBitmap(mBullet, bulletX, bulletY, paint);
	}
	
	//子弹的逻辑
	public void logic() {									//不同的子弹类型逻辑不一
		switch (bulletType) {								//主角的子弹垂直向上运动
		case Constant.BULLET_PLAYER:
			bulletY -= speed;
			if (bulletY < -50) {
				isDead = true;
			}
			break;
		case Constant.BULLET_DUCK:							//鸭子和苍蝇的子弹都是垂直下落运动
		case Constant.BULLET_FLY:
			bulletY += speed;
			if (bulletY > MySurfaceView.screenH) {
				isDead = true;
			}
			break;
		case Constant.BULLET_BOSS:							//Boss疯狂状态下的8方向子弹逻辑
			switch (dirBoss) {
			case Constant.DIR_UP:							//方向上的子弹
				bulletY -= speed;
				break;
			case Constant.DIR_DOWN:							//方向下的子弹
				bulletY += speed;
				break;
			case Constant.DIR_LEFT:							//方向左的子弹
				bulletX -= speed;
				break;
			case Constant.DIR_RIGHT:						//方向右的子弹
				bulletX += speed;
				break;
			case Constant.DIR_UP_LEFT:						//方向左上的子弹
				bulletY -= speed;
				bulletX -= speed;
				break;
			case Constant.DIR_UP_RIGHT:						//方向右上的子弹
				bulletX += speed;
				bulletY -= speed;
				break;
			case Constant.DIR_DOWN_LEFT:					//方向左下的子弹
				bulletX -= speed;
				bulletY += speed;
				break;
			case Constant.DIR_DOWN_RIGHT:					//方向右下的子弹
				bulletY += speed;
				bulletX += speed;
				break;
			}
			if (bulletY > MySurfaceView.screenH || bulletY <= -40 ||
					bulletX > MySurfaceView.screenW || bulletX <= -40) {
				isDead = true;								//边界处理
			}
			break;
		}
	}
}
