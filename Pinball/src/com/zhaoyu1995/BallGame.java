package com.zhaoyu1995;
import java.io.IOException;
import javax.swing.JFrame;
//游戏入口
public class BallGame {
	public static void main(String[] args) throws IOException {
		BallFrame ballFrame = new BallFrame();
		ballFrame.pack();
		ballFrame.setVisible(true);
		ballFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	}
}
//学习参考 《疯狂java实战演义》
//2016-7-20
