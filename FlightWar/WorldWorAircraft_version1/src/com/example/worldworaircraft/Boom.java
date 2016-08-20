package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class Boom {
	private	Bitmap	mBoom;								//爆炸效果资源图
	private int 	boomX, boomY;						//爆炸效果的位置坐标
	private int 	cureentFrameIndex;					//爆炸动画播放当前的帧下标
	private int	 	totleFrame;							//爆炸效果的总帧数
	private int 	frameW, frameH;						//每帧的宽高
	public 	boolean playEnd;							//是否播放完毕，优化处理

	//爆炸效果的构造函数
	public Boom(Bitmap mBoom, int x, int y, int totleFrame) {
		this.mBoom		=	mBoom;
		this.boomX		=	x;
		this.boomY 		=	y;
		this.totleFrame =	totleFrame;
		frameW 			=	mBoom.getWidth() / totleFrame;
		frameH 			= 	mBoom.getHeight();
	}

	//爆炸效果绘制
	public void draw(Canvas canvas, Paint paint) {
		canvas.save();
		canvas.clipRect(boomX, boomY, boomX + frameW, boomY + frameH);
		canvas.drawBitmap(mBoom, boomX - cureentFrameIndex * frameW, boomY, paint);
		canvas.restore();
	}

	//爆炸效果的逻辑
	public void logic() {
		if (cureentFrameIndex < totleFrame) {
			cureentFrameIndex++;
		} else {
			playEnd = true;
		}
	}
}
