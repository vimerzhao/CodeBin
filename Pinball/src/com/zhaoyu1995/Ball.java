package com.zhaoyu1995;
//import java.awt.Image;
//import java.io.File;
//import javax.imageio.ImageIO;
import java.io.IOException;

public class Ball extends BallComponent {
	// 定义球的竖向速度
	private int speedY = 10;
	// 定义弹球的横向速度
	private int speedX = 8;
	// 定义是否在运动
	private boolean started = false;
	// 定义是否结束运动
	private boolean stop = false;
	
	
	public Ball(int panelWidth, int panelHeight, int offset, String path) throws IOException {
		// 调用父构造器
		super(panelWidth, panelHeight, path);
		// 设置y坐标
		this.setY(panelHeight - super.getImage().getHeight(null) - offset);
	}
	//设置横向速度
	public void setSpeedX(int speed) {
		this.speedX = speed;
	}
	//设置纵向速度
	public void setSpeedY(int speed) {
		this.speedY = speed;
	}
	//设置是否运动
	public void setStarted(boolean b) {
		this.started = b;
	}
	//是否结束运动
	public void setStop(boolean b) {
		this.stop = b;
	}
	//返回横向速度
	public int getSpeedX() {
		return this.speedX;
	}
	//返回纵向速度
	public int getSpeedY() {
		return this.speedY;
	}	
	//是否在运动
	public boolean isStarted() {
		return this.started;
	}
	//是否已经结束运动
	public boolean isStop() {
		return this.stop;
	}	
}