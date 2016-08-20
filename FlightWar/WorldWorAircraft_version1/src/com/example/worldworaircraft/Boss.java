package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class Boss {
	public	int		blood 	=	Constant.BOSS_BLOOD;		//Boss的血量
	private	Bitmap 	mBoss;									//Boss的图片资源
	public	int 	x, y;									//Boss坐标
	public	int 	frameW, frameH;							//Boss每帧的宽高
	private int 	frameIndex;								//Boss当前帧下标
	private int 	speed	= Constant.ENEMY_BOSS_SPEED;	//Boss运动的速度
	//Boss的运动轨迹
	//一定时间会向着屏幕下方运动，并且发射大范围子弹，（是否狂态）
	//正常状态下 ，子弹垂直朝下运动
	private boolean isCrazy;
	private int		crazyTime = 200;						//进入疯狂状态的状态时间间隔
	private int		count;									//计数器

	//Boss的构造函数
	public Boss(Bitmap mBoss) {
		this.mBoss	=	mBoss;
		frameW 		=	mBoss.getWidth() / 10;
		frameH 		=	mBoss.getHeight();
		x			=	MySurfaceView.screenW / 2 - frameW / 2;
		y			= 	0;
	}

	//Boss的绘制
	public void draw(Canvas canvas, Paint paint) {
		canvas.save();
		canvas.clipRect(x, y, x + frameW, y + frameH);
		canvas.drawBitmap(mBoss, x - frameIndex * frameW, y, paint);
		canvas.restore();
	}

	//Boss的逻辑
	public void logic() {
		//不断循环播放帧形成动画
		frameIndex++;
		if (frameIndex >= 10) {
			frameIndex = 0;
		}
		//没有疯狂的状态
		if (isCrazy == false) {
			x += speed;
			if (x + frameW >= MySurfaceView.screenW) {
				speed = -speed;
			} else if (x <= 0) {
				speed = -speed;
			}
			count++;
			if (count % crazyTime == 0) {
				isCrazy = true;
				speed = 24;
			}
		} else {							//疯狂的状态
			speed -= 1;
			if (speed == 0) {				//当Boss返回时创建大量子弹
				//添加8方向子弹
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet,
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_UP));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_DOWN));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_LEFT));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_RIGHT));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_UP_LEFT));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_UP_RIGHT));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_DOWN_LEFT));
				MySurfaceView.bossBullet.add(new Bullet(MySurfaceView.mBossBullet, 
								x+40, y+10, Constant.BULLET_BOSS, Constant.DIR_DOWN_RIGHT));
			}
			y += speed;
			if (y <= 0) {
				isCrazy = false;					//恢复正常状态
				speed = Constant.ENEMY_BOSS_SPEED;
			}
		}
	}

	//判断碰撞(Boss被主角子弹击中)
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
		return true;
	}

	//设置Boss血量
	public void setBlood(int blood) {
		this.blood = blood;
	}
}
