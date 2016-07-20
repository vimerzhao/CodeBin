package com.zhaoyu1995;
import java.io.IOException;
// 使挡板变长的道具
public class LongMagic extends Magic {
	public LongMagic(String path, int x, int y) throws IOException {
		super(path, x, y);
	}
	
	public void magicDo(Stick stick) {
		double imageWidth = stick.getImage().getWidth(null);
		// 如果档板没有变长过
		if (stick.getPreWidth() <= imageWidth) {
			// 将档板的长度改为双倍
			stick.setPreWidth((int) (stick.getPreWidth() * 2));
		}
	}	
}
