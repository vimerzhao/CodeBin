package com.example.worldworaircraft;

import java.util.Random;
import java.util.Vector;

import android.content.Context;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.media.AudioManager;
import android.media.SoundPool;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
public class MySurfaceView extends SurfaceView implements Callback,Runnable {
	private SurfaceHolder 	surfaceHolder;
	private Paint 			paint;
	private Canvas 			canvas;
	private Thread 			thread;
	public 	static 	int 	screenW, screenH;
	public 	static 	int 	gameState = Constant.GAME_MENU;
	private boolean	 		flag;
	private float 			scaleX, scaleY;

	private	Resources		res = this.getResources();	//声明一个Resources实例便于加载图片
	//声明游戏所用到的资源
	private Bitmap 			mGameBg;							//游戏背景
	private Bitmap 			mBoom;								//爆炸效果
	private Bitmap	 		mBossBoom;							//Boss爆炸效果
	private Bitmap 			mButton;							//游戏开始按钮
	private Bitmap 			mButtonPress;						//游戏开始按钮被点击
	private Bitmap 			mEnemyDuck;							//怪物鸭子
	private Bitmap 			mEnemyFly;							//怪物苍蝇
	private Bitmap 			mEnemyBoss;							//怪物猪头Boss
	private Bitmap 			mGameWin;							//游戏胜利背景
	private Bitmap 			mGameLost;							//游戏失败背景
	private Bitmap 			mPlayer;							//游戏主角飞机
	private Bitmap 			mPlayerBlood;						//主角飞机血量
	private Bitmap		 	mMenuBg;							//菜单背景
	public 	static 	Bitmap 	mBullet;							//子弹
	public 	static 	Bitmap 	mEnemyBullet;						//敌机子弹
	public 	static	Bitmap 	mBossBullet;						//Boss子弹
	
	
	private Menu 			gameMenu;							//声明游戏组件的对象
	private GameBg 			backGround;							//声明一个滚动游戏背景对象
	private Player 			player;								//声明主角对象
	private Vector<Bullet> 	enemyBullet;						//敌机子弹容器
	private int 			countEnemyBullet;					//添加子弹的计数器
	private Vector<Bullet> 	playerBullet;						//主角子弹容器
	private int 			countPlayerBullet;					//添加子弹的计数器
	private Vector<Enemy> 	enemy;								//声明一个敌机容器
	private int 			count;								//计数器
	//敌人数组：1和2表示敌机的种类，-1表示Boss
	private int 			enemyArray[][] = new int[Constant.ENEMY_NUM][Constant.ENEMY_NUM_KIND];
	private int 			enemyArrayIndex = 0;				//当前取出一维数组的下标
	private boolean 		isBoss;								//是否出现Boss标识位
	private Random 			random;								//是否出现Boss标识位
	private Vector<Boom> 	boom;								//爆炸效果容器	
	private Boss 			boss;
	public static Vector<Bullet> bossBullet;					//Boss的子弹容器
	private SoundPool		shotPool;							//音效
	private SoundPool		boomPool;
	private int 			shotID;
	private int 			boomID;
	
	
	/**
	 * @author 赵裕
	 * 功能：构造函数
	 */
	public MySurfaceView(Context context) {
		super(context);
		surfaceHolder = this.getHolder();
		surfaceHolder.addCallback(this);
		paint = new Paint();
		paint.setColor(Color.WHITE);
		paint.setAntiAlias(true);
		setFocusable(true);
		setFocusableInTouchMode(true);
		//设置背景常亮
		this.setKeepScreenOn(true);
	}
	
	@Override
	public void surfaceCreated(SurfaceHolder arg0) {
		DisplayMetrics dm = getResources().getDisplayMetrics();  
        screenW = dm.widthPixels;  
        screenH = dm.heightPixels;  
		init();
		shotPool	= new SoundPool(4, AudioManager.STREAM_MUSIC, 100);
		boomPool	= new SoundPool(4, AudioManager.STREAM_MUSIC, 100);		
		shotID 		= shotPool.load(getContext(), R.raw.shot, 1);
		boomID		= boomPool.load(getContext(), R.raw.boom, 1);
		
		flag 		= true;
		thread 		= new Thread(this);
		thread.start();
		
	}

	
	/**
	 * @author 赵裕
	 * 功能：初始化游戏
	 */
	private void init() {
		if (gameState == Constant.GAME_MENU) {
			//1.加载游戏资源
			mGameBg 	= 	BitmapFactory.decodeResource(res, R.drawable.background);
			mBoom 		= 	BitmapFactory.decodeResource(res, R.drawable.boom);
			mBossBoom 	= 	BitmapFactory.decodeResource(res, R.drawable.boos_boom);
			mButton 	= 	BitmapFactory.decodeResource(res, R.drawable.button);
			mButtonPress= 	BitmapFactory.decodeResource(res, R.drawable.button_press);
			mEnemyDuck 	= 	BitmapFactory.decodeResource(res, R.drawable.enemy_duck);
			mEnemyFly 	= 	BitmapFactory.decodeResource(res, R.drawable.enemy_fly);
			mEnemyBoss 	= 	BitmapFactory.decodeResource(res, R.drawable.enemy_pig);
			mGameWin 	= 	BitmapFactory.decodeResource(res, R.drawable.gamewin);
			mGameLost 	= 	BitmapFactory.decodeResource(res, R.drawable.gamelost);
			mPlayer 	= 	BitmapFactory.decodeResource(res, R.drawable.player);
			mPlayerBlood= 	BitmapFactory.decodeResource(res, R.drawable.hp);
			mMenuBg 	= 	BitmapFactory.decodeResource(res, R.drawable.menu);
			mBullet 	= 	BitmapFactory.decodeResource(res, R.drawable.bullet);
			mEnemyBullet= 	BitmapFactory.decodeResource(res, R.drawable.bullet_enemy);
			mBossBullet = 	BitmapFactory.decodeResource(res, R.drawable.boosbullet);
			
			
			scaleX 		= 	screenW*1.0f/mMenuBg.getWidth();
			scaleY 		= 	screenH*1.0f/mMenuBg.getHeight();
			
			//2.适配屏幕
			mGameBg 	= 	Bitmap.createScaledBitmap(mGameBg, screenW,
					(int)(screenW*mGameBg.getHeight()*1.0f/mGameBg.getWidth()), true);//注意比例比较特殊
			mMenuBg 	= 	Bitmap.createScaledBitmap(mMenuBg, screenW, screenH, true);
			mButton 	= 	Bitmap.createScaledBitmap(mButton, (int)scaleX*mButton.getWidth(),
					(int)scaleY*mButton.getHeight(), true);
			mGameWin 	=	Bitmap.createScaledBitmap(mGameWin, screenW, screenH, true);
			mGameLost 	=	Bitmap.createScaledBitmap(mGameLost, screenW, screenH, true);
			mPlayer 	= 	Bitmap.createScaledBitmap(mPlayer, (int)scaleX*mPlayer.getWidth(),
					(int)scaleY*mPlayer.getHeight(), true);
			mPlayerBlood= 	Bitmap.createScaledBitmap(mPlayerBlood, 
					(int)scaleX*mPlayerBlood.getWidth(), 
						(int)scaleY*mPlayerBlood.getHeight(), true);
			
			//实例化组件
			gameMenu 	= 	new Menu(mMenuBg, mButton, mButtonPress);	//菜单类实例
			backGround 	= 	new GameBg(mGameBg);
			player 		= 	new Player(mPlayer, mPlayerBlood);
			enemy		= 	new Vector<Enemy>();						//实例敌机容器
			random		= 	new Random();								//实例随机库
			boom 		= 	new Vector<Boom>();						//爆炸效果容器实例
			enemyBullet = 	new Vector<Bullet>();						//敌机子弹容器实例
			playerBullet=	new Vector<Bullet>();						//主角子弹容器实例
			
			
			
			//---Boss相关
			boss = new Boss(mEnemyBoss);							//实例boss对象
			bossBullet = new Vector<Bullet>();						//实例Boss子弹容器
			
			count = 0;
			enemyArray = CreateEnemy.create();
		}
	}

	/**
	 * @author 赵裕
	 * 功能：根据游戏状态绘制
	 */
	private void draw() {
		try {
			canvas = surfaceHolder.lockCanvas();
			if (null != canvas) {
				canvas.drawColor(Color.WHITE);						//设置背景色
				switch (gameState) {								//判断状态
				case Constant.GAME_MENU:
					gameMenu.draw(canvas, paint);
					break;
				case Constant.GAME_RUN:
					backGround.draw(canvas, paint);//绘制背景
					player.draw(canvas, paint);//绘制玩家
					drawEnemy();
					for (int i = 0; i < playerBullet.size(); i++) {	//处理主角子弹绘制
						playerBullet.elementAt(i).draw(canvas, paint);
					}
					for (int i = 0; i < boom.size(); i++) {			//爆炸效果绘制
						boom.elementAt(i).draw(canvas, paint);
					}
					
					break;
				case Constant.GAME_WIN:
					canvas.drawBitmap(mGameWin, 0, 0, paint);
					break;
				case Constant.GAME_LOST:
					canvas.drawBitmap(mGameLost, 0, 0, paint);
					break;
				case Constant.GAME_PAUSE:
					
					break;
				default:
					break;
				}
			}
		} catch (Exception e) {
			// TODO: handle exception
		} finally {
			if (canvas != null)
				surfaceHolder.unlockCanvasAndPost(canvas);
		}
	}
	
	private void drawEnemy() {
		if (!isBoss) {
			for (int i = 0; i < enemy.size(); i++) {			//敌机绘制
				enemy.elementAt(i).draw(canvas, paint);
			}

			for (int i = 0; i < enemyBullet.size(); i++) {		//敌机子弹绘制
				enemyBullet.elementAt(i).draw(canvas, paint);
			}
			
		} else {
			boss.draw(canvas, paint);							//Boos的绘制
			for (int i = 0; i < bossBullet.size(); i++) {		//Boss子弹逻辑
				bossBullet.elementAt(i).draw(canvas, paint);
			}
		}
		
	}
	
	/**
	 * @author 赵裕
	 * 功能：游戏逻辑框架
	 */
	private void logic() {
		switch (gameState) {
		case Constant.GAME_MENU:
			
			break;
		case Constant.GAME_RUN:
			backGround.logic();			//背景逻辑
			player.logic();				//玩家逻辑
			logicEnemy();				//敌机逻辑
			logicPlayer();				//其他与主角有关的逻辑
			break;
		default:
			break;
		}
	}
	private void logicEnemy() {
		if (!isBoss) {
			for (int i = 0; i < enemy.size(); i++) {	//敌机逻辑
				Enemy en = enemy.elementAt(i);
				if (en.isDead) {						//如果已死亡那么就从容器中删除,对容器起到了优化作用；
					enemy.removeElementAt(i);
				} else {
					en.logic();
				}
			}
			count++;//生成敌机
			if (count % Constant.CREATE_ENEMY_TIME== 0) {
				for (int i = 0; i < enemyArray[enemyArrayIndex].length; i++) {
					if (enemyArray[enemyArrayIndex][i] == Constant.ENEMY_TYPE_FLY) {//苍蝇
						int x = random.nextInt(screenW - 100) + 50;Log.d("Run", "生成敌机");
						enemy.addElement(new Enemy(mEnemyFly, Constant.ENEMY_TYPE_FLY, x, -50));
					} else if (enemyArray[enemyArrayIndex][i] == Constant.ENEMY_TYPE_DUCKL) {//鸭子左
						int y = random.nextInt(20);Log.d("Run", "生成敌机1");
						enemy.addElement(new Enemy(mEnemyDuck, Constant.ENEMY_TYPE_DUCKL, -50, y));
					} else if (enemyArray[enemyArrayIndex][i] == Constant.ENEMY_TYPE_DUCKR) {//鸭子右
						int y = random.nextInt(20);Log.d("Run", "生成敌机2");
						enemy.addElement(new Enemy(mEnemyDuck, Constant.ENEMY_TYPE_DUCKR, screenW + 50, y));
					}
				}

				if (enemyArrayIndex == enemyArray.length - 1) {			
					isBoss = true;										//这里判断下一组是否为最后一组(Boss)
				} else {
					enemyArrayIndex++;
				}
			}
			//处理敌机与主角的碰撞
			for (int i = 0; i < enemy.size(); i++) {
				if (player.isCollsionWith(enemy.elementAt(i))) {
					player.setPlayerBlood(player.getPlayerBlood() - 1);	//发生碰撞，主角血量-1
					if (player.getPlayerBlood() <= -1) {					//当主角血量小于0，判定游戏失败
						gameState = Constant.GAME_LOST;
					}
				}
			}
			//每2秒添加一个敌机子弹
			countEnemyBullet++;
			if (countEnemyBullet % 40 == 0) {
				for (int i = 0; i < enemy.size(); i++) {
					Enemy en = enemy.elementAt(i);
					//不同类型敌机不同的子弹运行轨迹
					int bulletType = 0;
					switch (en.type) {
					//苍蝇
					case Constant.ENEMY_TYPE_FLY:
						bulletType = Constant.BULLET_FLY;
						break;
					//鸭子
					case Constant.ENEMY_TYPE_DUCKL:
					case Constant.ENEMY_TYPE_DUCKR:
						bulletType = Constant.BULLET_DUCK;
						break;
					}
					enemyBullet.add(new Bullet(mEnemyBullet, en.x + 10, en.y + 20, bulletType));
				}
			}
			//处理敌机子弹逻辑
			for (int i = 0; i < enemyBullet.size(); i++) {
				Bullet b = enemyBullet.elementAt(i);
				if (b.isDead) {
					enemyBullet.removeElement(b);
				} else {
					b.logic();
				}
			}
			//处理敌机子弹与主角碰撞
			for (int i = 0; i < enemyBullet.size(); i++) {
				if (player.isCollsionWith(enemyBullet.elementAt(i))) {
					//发生碰撞，主角血量-1
					player.setPlayerBlood(player.getPlayerBlood() - 1);
					//当主角血量小于0，判定游戏失败
					if (player.getPlayerBlood() <= -1) {
						gameState = Constant.GAME_LOST;
					}
				}
			}
			//处理主角子弹与敌机碰撞
			for (int i = 0; i < playerBullet.size(); i++) {
				//取出主角子弹容器的每个元素
				Bullet blPlayer = playerBullet.elementAt(i);
				for (int j = 0; j < enemy.size(); j++) {
					//添加爆炸效果
					//取出敌机容器的每个元与主角子弹遍历判断
					if (enemy.elementAt(j).isCollsionWith(blPlayer)) {
						boomPool.play(boomID, 1f, 1f, 1, 0, 1.0f);
						boom.add(new Boom(mBoom, enemy.elementAt(j).x, enemy.elementAt(j).y, 7));
					}
				}
			}
			
		} else {
			//Boss相关逻辑
			//每0.5秒添加一个主角子弹
			boss.logic();
			if (countPlayerBullet % 10 == 0) {
				//Boss的没发疯之前的普通子弹
				bossBullet.add(new Bullet(mBossBullet, boss.x + 35, boss.y + 40, Constant.BULLET_FLY));
			}
			//Boss子弹逻辑
			for (int i = 0; i < bossBullet.size(); i++) {
				Bullet b = bossBullet.elementAt(i);
				if (b.isDead) {
					bossBullet.removeElement(b);
				} else {
					b.logic();
				}
			}
			//Boss子弹与主角的碰撞
			for (int i = 0; i < bossBullet.size(); i++) {
				if (player.isCollsionWith(bossBullet.elementAt(i))) {
					//发生碰撞，主角血量-1
					player.setPlayerBlood(player.getPlayerBlood() - 1);
					//当主角血量小于0，判定游戏失败
					if (player.getPlayerBlood() <= -1) {
						gameState = Constant.GAME_LOST;
					}
				}
			}
			
			//Boss被主角子弹击中，产生爆炸效果
			for (int i = 0; i <playerBullet.size(); i++) {
				Bullet b = playerBullet.elementAt(i);
				if (boss.isCollsionWith(b)) {
					boomPool.play(boomID, 1f, 1f, 1, 0, 1.0f);
					if (boss.blood <= 0) {
						//游戏胜利
						gameState = Constant.GAME_WIN;
					} else {
						//及时删除本次碰撞的子弹，防止重复判定此子弹与Boss碰撞、
						b.isDead = true;
						//Boss血量减1
						boss.setBlood(boss.blood - 1);
						//在Boss上添加三个Boss爆炸效果
						boom.add(new Boom(mBossBoom, boss.x + 25, boss.y + 30, 5));
						boom.add(new Boom(mBossBoom, boss.x + 35, boss.y + 40, 5));
						boom.add(new Boom(mBossBoom, boss.x + 45, boss.y + 50, 5));
					}
				}
			}
			
		}
	}
	
	private void logicPlayer() {
		//每1秒添加一个主角子弹
		countPlayerBullet++;
		if (countPlayerBullet % 20 == 0) {
			shotPool.play(shotID, 1f, 1f, 1, 0, 1.0f);
			playerBullet.add(new Bullet(mBullet, player.x + 15, player.y - 20,
					Constant.BULLET_PLAYER));
		}
		//处理主角子弹逻辑
		for (int i = 0; i < playerBullet.size(); i++) {
			Bullet b = playerBullet.elementAt(i);
			if (b.isDead) {
				playerBullet.removeElement(b);
			} else {
				b.logic();
			}
		}
		//爆炸效果逻辑
		for (int i = 0; i < boom.size(); i++) {
			Boom b = boom.elementAt(i);
			if (b.playEnd) {
				//播放完毕的从容器中删除
				boom.removeElementAt(i);
			} else {
				boom.elementAt(i).logic();
			}
		}
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		switch (gameState) {
		case Constant.GAME_MENU:
			gameMenu.onTouchEvent(event);
			break;
		case Constant.GAME_RUN:
			player.onTouchEvent(event);
			break;
		default:
			break;
		}
		return true;//注意这一句，必须是ture，否则现MotionEvent.ACTION_UP分支未响应
	}
	
	@Override
	public void run() {
		while (flag) {
			long start = System.currentTimeMillis();
			draw();
			//Log.d("RUN", "RUN");
			logic();
			long end = System.currentTimeMillis();
			try {
				if (end - start < Constant.SLEEP_TIME) {
					Thread.sleep(Constant.SLEEP_TIME - (end - start));
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder arg0) {
		flag = false;
		boomPool.release();
		shotPool.release();
	}
	
	@Override
	public void surfaceChanged(SurfaceHolder surfaceHolder, int arg1, int arg2, int arg3) {

	}
}
