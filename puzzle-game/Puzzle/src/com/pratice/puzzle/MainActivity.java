package com.pratice.puzzle;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.view.GestureDetector;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.widget.GridLayout;
import android.widget.ImageView;
import android.widget.RelativeLayout.LayoutParams;
import android.widget.Toast;


public class MainActivity extends Activity {
	//判断游戏是否开始
	private boolean isBegin = false;
	//利用二维数组创建若干个游戏小方块
	private ImageView[][] gameArr = new ImageView[3][5];
	//游戏主界面
	private GridLayout game;
	//保存空方块
	private ImageView nullView;
	//当前手势
	private GestureDetector	curDetector;
	//注意要将手势监听重写,必须要有
	
	//当前动画是否正在执行移动
	boolean isAnimationRun = false;
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		return curDetector.onTouchEvent(event);
	}
	@Override
	public boolean dispatchTouchEvent(MotionEvent ev) {
		curDetector.onTouchEvent(ev);
		return super.dispatchTouchEvent(ev);
	}
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        curDetector = new GestureDetector(this, new OnGestureListener() {
			
			@Override
			public boolean onSingleTapUp(MotionEvent arg0) {
				// TODO Auto-generated method stub
				return false;
			}
			
			@Override
			public void onShowPress(MotionEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public boolean onScroll(MotionEvent arg0, MotionEvent arg1, float arg2,
					float arg3) {
				// TODO Auto-generated method stub
				return false;
			}
			
			@Override
			public void onLongPress(MotionEvent arg0) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
				int type = getDirectionByGesture(e1.getX(), e1.getY(), e2.getX(), e2.getY());
				//Toast.makeText(MainActivity.this, ""+type, Toast.LENGTH_SHORT).show();
				exchangeByDirection(type);
				return false;
			}
			
			@Override
			public boolean onDown(MotionEvent arg0) {
				// TODO Auto-generated method stub
				return false;
			}
		});
        /*初始化游戏的若干个小方块*/
        //获取一张大图??注意这一个的语法
        /*注意！！！这里是改变了图片大小以适应屏幕，如何让不同的图片匹配屏幕是今后需要学习的，这里不是关注的重点*/
        //必须是PNG图片？？？
        Bitmap bigPic;
        switch ((int)(Math.random()*5)) {
		case 0:
			bigPic = ((BitmapDrawable)getResources().getDrawable(R.drawable.game_pic0)).getBitmap();
			break;
		case 1:
			bigPic = ((BitmapDrawable)getResources().getDrawable(R.drawable.game_pic1)).getBitmap();
			break;
		case 2:
			bigPic = ((BitmapDrawable)getResources().getDrawable(R.drawable.game_pic2)).getBitmap();
			break;
		case 3:
			bigPic = ((BitmapDrawable)getResources().getDrawable(R.drawable.game_pic3)).getBitmap();
			break;
		case 4:
			bigPic = ((BitmapDrawable)getResources().getDrawable(R.drawable.game_pic4)).getBitmap();
			break;
		default:
			bigPic = ((BitmapDrawable)getResources().getDrawable(R.drawable.game_pic0)).getBitmap();
			break;
		}
        int picWidth = bigPic.getWidth() / 5;
        //获取屏幕尺寸
        int blockWidth = getWindowManager().getDefaultDisplay().getWidth() / 5;
        for (int i = 0; i < gameArr.length; i++) {
        	for (int j = 0; j < gameArr[0].length; j++) {
        		//根据行和列切成若干游戏小图片
        		Bitmap unit = Bitmap.createBitmap(bigPic, j*picWidth, i*picWidth, picWidth, picWidth);
        		gameArr[i][j] = new ImageView(this);
        		gameArr[i][j].setImageBitmap(unit);
        		//设置图片宽高
        		gameArr[i][j].setLayoutParams(new LayoutParams(blockWidth, blockWidth));
        		gameArr[i][j].setPadding(2, 2, 2, 2);
        		//绑定自定义数据
        		gameArr[i][j].setTag(new GameData(i, j, unit));
        		gameArr[i][j].setOnClickListener(new OnClickListener() {
					
					@Override
					public void onClick(View v) {
						boolean flag = isNeighbourofNull((ImageView)v);						
						//Toast.makeText(MainActivity.this, "是否存在"+flag, Toast.LENGTH_LONG).show();
						if (flag) {
							exchangeData((ImageView)v);
						}
					}
				});
        	}
        }
        
        /*初始化游戏主界面，并添加若干个小方块*/
        game = (GridLayout)findViewById(R.id.game);
        for (int i = 0; i < gameArr.length; i++) {
        	for (int j = 0; j < gameArr[0].length; j++) {
        		game.addView(gameArr[i][j]);
        	}
        }        
        //设置最后一个方块为空
        setNullImageView(gameArr[2][4]);
        
        //随机打乱顺序
        randomExchange();
        isBegin = true;
    }
    /**
     * 设置某个方块为空方块
     * @param mImageView 当前要设置为空的方块实例
     */
    public void setNullImageView(ImageView mImageView) {
		mImageView.setImageBitmap(null);
		nullView = mImageView;
	}
    /**
     * 判断当前点击的方块是否与空方块是相邻关系
     * @param mImageView 当前点击的方块
     * @return true:相邻；false:不相邻
     */
    public boolean isNeighbourofNull(ImageView mImageView) {
    	//获取空方块和当前点击方块的数据
    	GameData nullData = (GameData) nullView.getTag();
    	GameData curData  = (GameData) mImageView.getTag();
    	if (nullData.x==curData.x && Math.abs(nullData.y-curData.y)==1) {
			return true;
		} else if (nullData.y==curData.y && Math.abs(nullData.x-curData.x)==1) {
			return true;
		}
    	return false;
	}
    
    //重载，解决中途更改代码问题
    public void exchangeByDirection(int type) {
    	exchangeByDirection(type, true);
    }
    
    //根据监听到的手势方向，获取空方块对应相邻位置，如果存在方块，则进行交换
    public void exchangeByDirection(int type, boolean isAnimation) {
		//获取当前空方块的位置
    	GameData nullData = (GameData)nullView.getTag();
    	//获取相邻方块位置
    	int new_x = nullData.x;
    	int new_y = nullData.y;
    	switch (type) {
		case 1:
			new_x++;
			break;
		case 2:
			new_x--;
			break;
		case 3:
			new_y++;
			break;			
		case 4:
			new_y--;
			break;
		default:
			break;
		}  	
    	//判断是否合法
    	if (new_x>=0 && new_y>=0 && new_x<gameArr.length && new_y<gameArr[0].length) {
    		//合法则进行移动
    		if (isAnimation) {//判断是否有动画
    			exchangeData(gameArr[new_x][new_y]);
    		} else {
    			exchangeData(gameArr[new_x][new_y], isAnimation);//false
    		}
    	}
    	
	}
    //手势滑动方向判断
    //1:上 	2:下		3：左		4：右
    public int getDirectionByGesture(float start_x, float start_y, float end_x, float end_y) {
    	boolean isLeftOrRight = Math.abs(start_x-end_x) > Math.abs(start_y-end_y)?true:false;
    	if (isLeftOrRight) {
    		boolean isLeft = start_x-end_x>0?true:false;
    		if (isLeft) {
    			return 3;
    		} else {
    			return 4;
    		}
    	} else {
    		boolean isUp = start_y-end_y>0?true:false;
    		if (isUp) {
    			return 1;
    		} else {
    			return 2;
    		}
    	}
    }
    //随机打乱顺序
    public void randomExchange() {
		//随机打乱次数
    	for (int i = 0; i < 100; i++) {
    		//根据手势进行交换，但是无动画
    		int type = (int)(Math.random()*4)+1;
    		exchangeByDirection(type, false);
    	}
	}
    
    //利用重载解决中途更改函数参数的问题
    public void exchangeData(final ImageView mImageView) {
    	exchangeData(mImageView, true);
    }
    
    public void checkGame() {
    	boolean isGameOver = true;
		//遍历每个游戏小方块
    	for (int i = 0; i < gameArr.length; i++) {
    		for (int j = 0; j < gameArr[0].length; j++) {
    			//为空跳过
    			if (gameArr[i][j] == nullView) {
    				continue;
    			}
    			GameData curData = (GameData)gameArr[i][j].getTag();
    			if (!curData.isTrue()) {
    				isGameOver = false;
    				break;
    			}
    		}
    	}
    	if (isGameOver) {
    		Toast.makeText(this, "游戏结束", Toast.LENGTH_SHORT).show();
    	}
	}
    //利用动画结束之后交换两个方块的数据
    public void exchangeData(final ImageView mImageView, boolean isAnimation) {
		if (isAnimationRun) {
			return;
		}
    	//如果没有动画
    	if (!isAnimation) {
			//以下是主要逻辑之一，注意加深理解
			//个人感觉这里还是用setter\getter比较好
			mImageView.clearAnimation();
			GameData mData = (GameData)mImageView.getTag();
			nullView.setImageBitmap(mData.bm);//能直接访问内部类的私有对象，但必须先实例化，没好好看书啊 	
			GameData nullData = (GameData)nullView.getTag();
			nullData.bm = mData.bm;
			nullData.px = mData.px;
			nullData.py = mData.py;
			setNullImageView(mImageView);
	    	if (isBegin) {
	    		checkGame();
	    	}
			return;
    	}
    	//创建一个动画，设置好方向，移动的距离
    	TranslateAnimation exchangeAnimation = null;
    	if (mImageView.getX() > nullView.getX()) {
    		//在下方，往上移动
    		exchangeAnimation = new TranslateAnimation(0.1f, -mImageView.getWidth(), 0.1f, 0.1f);
    	} else if (mImageView.getX() < nullView.getX()) {
    		//在上方，往下移
    		exchangeAnimation = new TranslateAnimation(0.1f, mImageView.getWidth(), 0.1f, 0.1f);
    	} else if (mImageView.getY() > nullView.getY()) {
    		//在右边，往左移动
    		exchangeAnimation = new TranslateAnimation(0.1f, 0.1f, 0.1f, -mImageView.getWidth());
    	} else if (mImageView.getY() < nullView.getY()) {
    		//再左边，往右移动
    		exchangeAnimation = new TranslateAnimation(0.1f, 0.1f, 0.1f, mImageView.getWidth());
    	}
    	//设置动画时长
    	exchangeAnimation.setDuration(50);
    	//设置动画结束之后是否停留
    	exchangeAnimation.setFillAfter(true);
    	//真正的数据交换
    	exchangeAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation arg0) {
				isAnimationRun = true;
			}
			
			@Override
			public void onAnimationRepeat(Animation arg0) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				isAnimationRun = false;
				//以下是主要逻辑之一，注意加深理解
				//个人感觉这里还是用setter\getter比较好
				mImageView.clearAnimation();
				GameData mData = (GameData)mImageView.getTag();
				nullView.setImageBitmap(mData.bm);//能直接访问内部类的私有对象，但必须先实例化，没好好看书啊 	
				GameData nullData = (GameData)nullView.getTag();
				nullData.bm = mData.bm;
				nullData.px = mData.px;
				nullData.py = mData.py;
				setNullImageView(mImageView);//原来位置的px,py还没交换！！
		    	if (isBegin) {
		    		checkGame();
		    	}
			}
		});
    	//执行动画
    	mImageView.startAnimation(exchangeAnimation);
	}
    
    //每个游戏小方块上要绑定的数据
    class	GameData {
    	//实际位置
    	private int x;
    	private int y;
    	//小方块对应的图片
    	private Bitmap bm;
    	//图片位置
    	private int px;
    	private int py;
    	public GameData(int x, int y, Bitmap bm) {
			super();
			this.x = x;
			this.y = y;
			this.bm = bm;
			this.px = x;
			this.py = y;
		}
    	//判断每个小方块位置是否正确
		public boolean isTrue() {
			if (this.x==this.px && this.y==this.py) {
				return true;
			}
			return false;
		}
    }
}


//细节问题，动画移动过程中不可操作
//昨天做的2D翻转的Demo也有这个问题


//手势不能再图片区域产生效果，涉及时间的分发机制，暂时还不是太理解


//以及如何处理图片与实际屏幕