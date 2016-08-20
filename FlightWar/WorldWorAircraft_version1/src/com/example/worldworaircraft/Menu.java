package com.example.worldworaircraft;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.Log;
import android.view.MotionEvent;

public class Menu {
	//菜单背景图
	private Bitmap 	mMenuBg;
	//按钮图片资源(按下和未按下图)
	private Bitmap 	mButton, mButtonPress;
	//按钮的坐标
	private int 	btnX, btnY;
	//按钮是否按下标识位
	private Boolean isPress;
	Menu(Bitmap mMenuBg, Bitmap mButton, Bitmap mButtonPress) {
		this.mMenuBg 		= 	mMenuBg;
		this.mButton 		= 	mButton;
		this.mButtonPress 	=	mButtonPress;
		btnX 				= 	MySurfaceView.screenW/2 - mButton.getWidth()/2;
		btnY 				=	MySurfaceView.screenH   - mButton.getHeight();
		isPress 			= 	false;
	}
	public void draw(Canvas canvas, Paint paint) {
		//绘制菜单背景图
		canvas.drawBitmap(mMenuBg, 0, 0, null);
		//绘制未按下按钮图
		if (isPress) {//根据是否按下绘制不同状态的按钮图
			canvas.drawBitmap(mButtonPress, btnX, btnY, paint);
		} else {
			canvas.drawBitmap(mButton, btnX, btnY, paint);
		}
	}
	public void onTouchEvent(MotionEvent event) {
		//获取用户当前触屏位置
		int pointX = (int) event.getX();
		int pointY = (int) event.getY();
//		Log.d("TAG", "btnX"+btnX+"btnY"+btnY);
//		Log.d("TAG", "pointX"+pointX+"pointY"+pointY);
//		Log.d("TAG", "W"+mButton.getWidth()+"H"+mButton.getHeight());
		//当用户是按下动作或移动动作
		if (event.getAction() == MotionEvent.ACTION_DOWN ||
				event.getAction() == MotionEvent.ACTION_MOVE) {		//判定用户是否点击了按钮
			//Log.d("DOWN", "DOWN");
			if (pointX > btnX && pointX < btnX + mButton.getWidth()) {
				if (pointY > btnY && pointY < btnY + mButton.getHeight()) {
					isPress = true;
				} else {
					isPress = false;
				}
			} else {
				isPress = false;
			}
			
		} else if (event.getAction() == MotionEvent.ACTION_UP) {	//抬起判断是否点击按钮，防止用户移动到别处
			//Log.d("UP", "UP");//当用户是抬起动作
			if (pointX > btnX && pointX < btnX + mButton.getWidth()) {
				if (pointY > btnY && pointY < btnY + mButton.getHeight()) {
					isPress = false;								//还原Button状态为未按下状态
					Log.d("RUN", "RUN");
					MySurfaceView.gameState = Constant.GAME_RUN;	//改变当前游戏状态为开始游戏，游戏入口
				}
			}
		}
	}
}
