package com.zhaoyu1995;
import java.io.IOException;

public class ShortMagic extends Magic {
	public ShortMagic(String path, int x, int y) throws IOException {
		super(path, x, y);
	}
	//实现道具功能
	public void magicDo(Stick stick) {
		double imageWidth = stick.getImage().getWidth(null);
		//如果挡板没有变短过
		if (stick.getPreWidth() >= imageWidth) {
			//将挡板的宽度改为一半
			stick.setPreWidth((int)(stick.getPreWidth()*0.5));
		}
	}
}
