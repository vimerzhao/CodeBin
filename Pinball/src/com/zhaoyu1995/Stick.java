package com.zhaoyu1995;
import java.io.IOException;

public class Stick extends BallComponent {
	//定义挡板移动速度
	public static final int SPEED = 20;
	//挡板初始长度
	private int preWidth = 0;
	
	public Stick (int panelWidth, int panelHeight, String path) throws IOException {
		//调用父构造器
		super(panelWidth, panelHeight, path);
		//设置y坐标
		this.setY(panelHeight-super.getImage().getHeight(null));
		//设置原本长度
		this.preWidth = super.getImage().getWidth(null);
	}
	
	public int getPreWidth() {
		return this.preWidth;
	}
	public void setPreWidth(int preWidth) {
		this.preWidth = preWidth;
	}
}
