package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;

public class GameBg {
	//游戏背景的图片资源
	//为了循环播放，这里定义两个位图对象，
	//其资源引用的是同一张图片
	private Bitmap 	mBackground_1;
	private Bitmap 	mBackground_2;
	//游戏背景坐标
	private int 	bg1x, bg1y, bg2x, bg2y;
	//背景滚动速度
	private int speed = Constant.BG_SPEED;

	//游戏背景构造函数
	public GameBg(Bitmap mBackground) {
		this.mBackground_1 = mBackground;
		this.mBackground_2 = mBackground;
		//首先让第一张背景底部正好填满整个屏幕
		bg1y = -Math.abs(mBackground_1.getHeight() - MySurfaceView.screenH);
		//第二张背景图紧接在第一张背景的上方
		//+101的原因：虽然两张背景图无缝隙连接但是因为图片资源头尾
		//直接连接不和谐，为了让视觉看不出是两张图连接而修正的位置
		bg2y = bg1y - mBackground_1.getHeight() + 111;
	}
	
	//游戏背景的绘图函数
	public void draw(Canvas canvas, Paint paint) {
		//绘制两张背景
		canvas.drawBitmap(mBackground_1, bg1x, bg1y, paint);
		canvas.drawBitmap(mBackground_2, bg2x, bg2y, paint);
	}
	
	//游戏背景的逻辑函数
	public void logic() {
		bg1y += speed;
		bg2y += speed;
		//当第一张图片的Y坐标超出屏幕，
		//立即将其坐标设置到第二张图的上方
		if (bg1y > MySurfaceView.screenH) {
			bg1y = bg2y - mBackground_1.getHeight() + Constant.MATCH_DIS;
		}
		//当第二张图片的Y坐标超出屏幕，
		//立即将其坐标设置到第一张图的上方
		if (bg2y > MySurfaceView.screenH) {
			bg2y = bg1y - mBackground_1.getHeight() + Constant.MATCH_DIS;
		}
	}
}
