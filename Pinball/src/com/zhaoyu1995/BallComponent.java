package com.zhaoyu1995;
import java.awt.Image;
import java.io.File;
import javax.imageio.ImageIO;
import java.io.IOException;
public class BallComponent {
	//坐标
	private int x = -1;
	private int y = -1;
	private Image image = null;
	private int speed = 5;
	//构造函数
	public BallComponent(String path) throws IOException {
		super();
		this.image = ImageIO.read(new File(path));
	}
	public BallComponent(int panelWidth, int panelHeight, String path)
				throws IOException {
		super();
		this.image = ImageIO.read(new File(path));
		this.x = (int)((panelWidth-image.getWidth(null))/2);
	}
	public BallComponent(String path, int x, int y) throws IOException {
		super();
		this.image = ImageIO.read(new File(path));
		this.x = x;
		this.y = y;
	}
	
	//set get 
	public int getX() {
		return this.x;
	}
	public int getY() {
		return this.y;
	}
	public int getSpeed() {
		return this.speed;
	}
	public void setX(int x) {
		this.x = x;
	}

	public void setY(int y) {
		this.y = y;
	}
	public Image getImage() {
		return this.image;
	}	
}