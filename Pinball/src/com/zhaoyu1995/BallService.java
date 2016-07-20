package com.zhaoyu1995;
import java.awt.event.KeyEvent;
import java.awt.Image;
import java.awt.Graphics;
import java.io.IOException;
public class BallService {
	// 定义一个Stick(档板)
	private Stick stick = null;
	// 定义一个弹球
	private Ball ball = null;
	// 定义一个游戏结束图片
	private BallComponent gameOver = null;
	// 定义一个赢了游戏的图片
	private BallComponent won = null;
	// 定义一个砖块图片数组
	private Brick[][] bricks = null;
	private int width;
	private int height;
	BallFrame ballFrame = null;
	//私有空构造器
	@SuppressWarnings("unused")
	private BallService() {
		super();
	}
	
	public BallService(BallFrame frame, int width, int height) throws IOException {
		// 初始化变量
		this.width = width;
		this.height = height;
		this.ballFrame = frame;
		// 创建一个Stick(档板)实例
		stick = new Stick(width, height, "images/stick.jpg");
		// 创建一个弹球的实例
		ball = new Ball(width, height, stick.getImage().getHeight(null), "images/ball.gif");
		// 游戏结束图片
		gameOver = new BallComponent("images/over.gif");
		// 赢图片
		won = new BallComponent("images/win.gif");
		// 砖块图片数组
		bricks = createBrickArr("images/brick.gif", 11, 6);
	}	
	public void run() {
		// 弹球坐标改变
		setBallPos();
		// 道具坐标改改变
		setMagicPos();
	}
	public void setStickPos(KeyEvent ke) {
		// 把弹球的运动状态设为true
		ball.setStarted(true);
		// 如果是左方向键
		if (ke.getKeyCode() == KeyEvent.VK_LEFT) {
			if (stick.getX() - Stick.SPEED > 0) {
				// x坐标向左移动
				stick.setX(stick.getX() - Stick.SPEED);
			}
		}
		// 如果是右方向键
		if (ke.getKeyCode() == KeyEvent.VK_RIGHT) {
			if (stick.getX() + Stick.SPEED < width - stick.getPreWidth()) {
				// x坐标向右移动
				stick.setX(stick.getX() + Stick.SPEED);
				// ballFrame.getBallGame().reStart( ballFrame );
			}
		}
		// 如果是F2键
		if (ke.getKeyCode() == KeyEvent.VK_F2) {
			// 初始化ballFrame
			try {
				ballFrame.initialize();
			} catch (IOException e) {
				System.out.println(e.getMessage());
			}
		}
	}

	public void setBallPos() {
		// 正数的数度
		int absSpeedX = Math.abs(ball.getSpeedX());
		int absSpeedY = Math.abs(ball.getSpeedY());
		// 如果游戏已经开始而且没有结束
		if (ball.isStarted()) {
			// 如果小球碰到左边界
			if (ball.getX() - absSpeedX < 0) {
				// 重新设置x坐标
				ball.setX(ball.getImage().getWidth(null));
				// 把x方向的速度设为反方向
				ball.setSpeedX(-ball.getSpeedX());
			}
			// 如果小球碰到右边界
			if (ball.getX() + absSpeedX > width
					- ball.getImage().getWidth(null)) {
				// 重新设置x坐标
				ball.setX(width - ball.getImage().getWidth(null) * 2);
				// 把x方向的速度设为反方向
				ball.setSpeedX(-ball.getSpeedX());
			}
			// 如果小球碰到上边界
			if (ball.getY() - absSpeedY < 0) {
				// 重新设置y坐标
				ball.setY(ball.getImage().getWidth(null));
				// 把y方向的速度设为反方向
				ball.setSpeedY(-ball.getSpeedY());
			}
			// 如果小球碰到下边界
			if (ball.getY() + absSpeedY > height
					- stick.getImage().getHeight(null)) {
				// 如果小球与档板有碰撞
				if (isHitStick(ball)) {
					// 重新设置y坐标
					ball.setY(height - ball.getImage().getHeight(null) * 2);
					// 把y方向的速度设为反方向
					ball.setSpeedY(-ball.getSpeedY());
				}
			}
			// 与砖块碰撞后的运动
			for (int i = bricks.length - 1; i > -1; i--) {
				for (int j = bricks[i].length - 1; j > -1; j--) {
					// 如果小球与砖块有碰撞
					if (isHitBrick(bricks[i][j])) {
						if (ball.getSpeedY() > 0) {
							ball.setSpeedY(-ball.getSpeedY());
						}
					}
				}
			}
			// 结束游戏
			if (ball.getY() > height) {
				ball.setStop(true);
			}

			// 设置x坐标
			ball.setX(ball.getX() - (int) (Math.random() * 2) - ball.getSpeedX());
			// 设置y坐标
			ball.setY(ball.getY() - (int) (Math.random() * 2) - ball.getSpeedY());
		}
	}
	
	public boolean isHitBrick(Brick brick) {
		if (brick.isDisable()) {
			return false;
		}
		// ball的圆心x坐标
		double ballX = ball.getX() + ball.getImage().getWidth(null) / 2;
		// ball的圆心y坐标
		double ballY = ball.getY() + ball.getImage().getHeight(null) / 2;
		// brick的中心x坐标
		double brickX = brick.getX() + brick.getImage().getWidth(null) / 2;
		// brick的中心y坐标
		double brickY = brick.getY() + brick.getImage().getHeight(null) / 2;
		// 两个坐标点的距离
		double distance = Math.sqrt(Math.pow(ballX - brickX, 2)
				+ Math.pow(ballY - brickY, 2));
		// 如果两个图形重叠，返回true;
		if (distance < (ball.getImage().getWidth(null) + brick.getImage()
				.getWidth(null)) / 2) {
			// 使brick无效
			brick.setDisable(true);
			return true;

		}
		return false;
	}
	
	public boolean isHitStick(BallComponent bc) {
		// 获取图片对象
		Image tempImage = bc.getImage();
		// 如果与档板有碰撞
		if (bc.getX() + tempImage.getWidth(null) > stick.getX()
				&& bc.getX() < stick.getX() + stick.getPreWidth()
					&& bc.getY() + tempImage.getHeight(null) > stick.getY()) {
			return true;
		}
		return false;
	}
	
	public void setMagicPos() {
		for (int i = 0; i < bricks.length; i++) {
			for (int j = 0; j < bricks[i].length; j++) {
				// 获取magic
				Magic magic = bricks[i][j].getMagic();
				if (magic != null) {
					// 如果这个brick的状态是无效的
					if (bricks[i][j].isDisable() && magic.getY() < height) {
						// 设置magic的y坐标向下增加
						magic.setY(magic.getY() + magic.getSpeed());
						// 设置档板的宽度
						setStickWidth(magic);

					}
				}
			}
		}
	}

	public void setStickWidth(Magic magic) {
		if (isHitStick(magic)) {
			// 道具的作用
			magic.magicDo(stick);
		}
	}
	
	public boolean isWon() {
		// 如果消了全部砖块，则为赢
		for (int i = 0; i < bricks.length; i++) {
			for (int j = 0; j < bricks[i].length; j++) {
				if (!bricks[i][j].isDisable()) {
					return false;
				}
			}
		}
		return true;
	}
	
	public Brick[][] createBrickArr(String path, int xSize, int ySize)
			throws IOException {
		// 创建一个Brick[][]
		Brick[][] bricks = new Brick[xSize][ySize];
		int x = 0;
		int y = 0;
		int random = 0;
		int imageSize = 28;
		boolean isDisable = false;
		// 迭代初始化数组
		for (int i = 0; i < xSize; i++) {
			for (int j = 0; j < ySize; j++) {
				// 创建一个新的砖块
				random = (int) (Math.random() * 3);
				x = i * imageSize;
				y = j * imageSize;
				// 一定机率没有砖块
				isDisable = Math.random() > 0.8 ? true : false;
				if (isDisable) {
					random = 0;
				}
				Brick brick = new Brick(path, random, x, y);
				brick.setDisable(isDisable);
				// 设置x坐标
				brick.setX(x);
				// 设置y坐标
				brick.setY(y);
				bricks[i][j] = brick;
			}
		}
		return bricks;
	}
	
	public void draw(Graphics g) {
		// 如果赢了
		if (isWon()) {
			// 绘制赢的图片
			g.drawImage(won.getImage(), won.getX(), won.getY(), width, height - 10, null);
		} else if (ball.isStop()) {
			// 绘制游戏结束图像
			g.drawImage(gameOver.getImage(), gameOver.getX(), gameOver.getY(), width, height - 10, null);
		} else {
			// 清除原来的图像
			g.clearRect(0, 0, width, height);
			// 绘制档板图像
			g.drawImage(stick.getImage(), stick.getX(), stick.getY(), stick
					.getPreWidth(), stick.getImage().getHeight(null), null);
			// 绘制弹球图像
			g.drawImage(ball.getImage(), ball.getX(), ball.getY(), null);
			// 迭代绘制砖块图像
			for (int i = 0; i < bricks.length; i++) {
				for (int j = 0; j < bricks[i].length; j++) {
					BallComponent magic = bricks[i][j].getMagic();
					// 如果这个砖块图像对像是有效的
					if (!bricks[i][j].isDisable()) {
						// 里面的数字1为砖块图像间的间隙
						g.drawImage(bricks[i][j].getImage(), bricks[i][j]
								.getX(), bricks[i][j].getY(), bricks[i][j]
								.getImage().getWidth(null) - 1, bricks[i][j]
								.getImage().getHeight(null) - 1, null);
					} else if (magic != null && magic.getY() < height) {
						g.drawImage(magic.getImage(), magic.getX(), magic
								.getY(), null);
					}
				}
			}
		}
	}
}
