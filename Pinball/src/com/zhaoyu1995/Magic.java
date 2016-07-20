package com.zhaoyu1995;
import java.io.IOException;
//µÀ¾ß
public abstract class Magic extends BallComponent {
	public Magic (String path, int x, int y) throws IOException {
		super(path, x, y);
	}
	public abstract void magicDo(Stick stick);
}
