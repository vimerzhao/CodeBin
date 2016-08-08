package com.example.snake;

import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Point;
import android.util.AttributeSet;
import android.view.View;
import android.widget.Toast;

public class GameView extends View {
	private float unitSize;
	private static final int VIEW_COUNT = 22;//良好的代码可以只改变该只，就可得设置不同大小的格子，有利于代码拓展
	private int[][] markLevel = new int[VIEW_COUNT][VIEW_COUNT];
	private Bitmap mBlockBitmap;
	
	private Bitmap mAppleBitmap;
	private Bitmap mUpTailBitmap;
	private Bitmap mDownTailBitmap;
	private Bitmap mLeftTailBitmap;
	private Bitmap mRightTailBitmap;
	
	private Bitmap mUpHeadBitmap;
	private Bitmap mDownHeadBitmap;
	private Bitmap mLeftHeadBitmap;
	private Bitmap mRightHeadBitmap;
	
	private Bitmap mHorizontalBodyBitmap;
	private Bitmap mVerticalBodyBitmap;

	private Bitmap mLeftTopBitmap;
	private Bitmap mRightTopBitmap;
	private Bitmap mLeftBottonBitmap;
	private Bitmap mRightBottonBitmap;
	
	
	
	private static final int EMPTY = 0;
	private static final int BLOCK = 1;
	
	
	private List<Point> snakeList = new ArrayList<Point>();
	private Point mApplePoint;
	private int direction;
	
	public GameView(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
	}

	public GameView(Context context, AttributeSet attrs) {
		super(context, attrs);
		setBackgroundResource(R.drawable.bg_game);
		
	}

	public GameView(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
		int widthSize = MeasureSpec.getSize(widthMeasureSpec);
		int widthMode = MeasureSpec.getMode(widthMeasureSpec);
		int heightSize = MeasureSpec.getSize(heightMeasureSpec);
		int heightMode = MeasureSpec.getMode(heightMeasureSpec);
		int width = Math.min(widthSize, heightSize);
		//增加代码健壮程度
		if (MeasureSpec.UNSPECIFIED == widthMode) {
			width = heightSize;
		} else if (MeasureSpec.UNSPECIFIED == heightMode) {
			width = widthSize;
		}		
		setMeasuredDimension(width, width);
	}

	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		super.onSizeChanged(w, h, oldw, oldh);
		unitSize = w*1.0f/VIEW_COUNT;

		initView();
		initGame();
	}
	
	public void initGame() {
		//初始化游戏数据
		snakeList.clear();
		snakeList.add(new Point(3, 2));
		snakeList.add(new Point(2, 2));
		snakeList.add(new Point(2, 1));
		snakeList.add(new Point(1, 1));
		snakeList.add(0, new Point(3,3));
		direction = MainActivity.D_DOWN;
		
		mApplePoint = new Point(3, 9);//注意空指针
	}
	public void initView() {
		//1
		for (int i = 0; i < markLevel.length; i++) {
			for (int j = 0; j < markLevel[0].length; j++) {
				if (i==0 || i==VIEW_COUNT-1 || j==0 || j==VIEW_COUNT-1) {
					markLevel[i][j] = BLOCK;
				} else {
					markLevel[i][j] = EMPTY;
				}
			}
		}
		//2
		mBlockBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.block);
		mBlockBitmap = Bitmap.createScaledBitmap(mBlockBitmap, (int)unitSize, (int)unitSize, false);
		
		//3 切割图片		
		Bitmap resBitmap = BitmapFactory.decodeResource(getResources(), R.drawable.res);
		int unitHeight = resBitmap.getHeight()/4;
		//resBitmap的坐标--调整坐标
		mAppleBitmap = Bitmap.createBitmap(resBitmap, 0, unitHeight*3, unitHeight, unitHeight);
		mAppleBitmap = Bitmap.createScaledBitmap(mAppleBitmap, (int)unitSize, (int)unitSize, false);
		
		
		mUpTailBitmap = Bitmap.createBitmap(resBitmap, unitHeight*4, unitHeight*3, unitHeight, unitHeight);
		mUpTailBitmap = Bitmap.createScaledBitmap(mUpTailBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mDownTailBitmap = Bitmap.createBitmap(resBitmap, unitHeight*3, unitHeight*2, unitHeight, unitHeight);
		mDownTailBitmap = Bitmap.createScaledBitmap(mDownTailBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mLeftTailBitmap = Bitmap.createBitmap(resBitmap, unitHeight*4, unitHeight*2, unitHeight, unitHeight);
		mLeftTailBitmap = Bitmap.createScaledBitmap(mLeftTailBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mRightTailBitmap = Bitmap.createBitmap(resBitmap, unitHeight*3, unitHeight*3, unitHeight, unitHeight);
		mRightTailBitmap = Bitmap.createScaledBitmap(mRightTailBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		
		mHorizontalBodyBitmap = Bitmap.createBitmap(resBitmap, unitHeight, 0, unitHeight, unitHeight);
		mHorizontalBodyBitmap = Bitmap.createScaledBitmap(mHorizontalBodyBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mVerticalBodyBitmap = Bitmap.createBitmap(resBitmap, unitHeight*2, unitHeight, unitHeight, unitHeight);
		mVerticalBodyBitmap = Bitmap.createScaledBitmap(mVerticalBodyBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mUpHeadBitmap = Bitmap.createBitmap(resBitmap, 3*unitHeight, 0, unitHeight, unitHeight);
		mUpHeadBitmap = Bitmap.createScaledBitmap(mUpHeadBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mDownHeadBitmap = Bitmap.createBitmap(resBitmap, 4*unitHeight, unitHeight, unitHeight, unitHeight);
		mDownHeadBitmap = Bitmap.createScaledBitmap(mDownHeadBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mLeftHeadBitmap = Bitmap.createBitmap(resBitmap, 3*unitHeight, unitHeight, unitHeight, unitHeight);
		mLeftHeadBitmap = Bitmap.createScaledBitmap(mLeftHeadBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mRightHeadBitmap = Bitmap.createBitmap(resBitmap, 4*unitHeight, 0, unitHeight, unitHeight);
		mRightHeadBitmap = Bitmap.createScaledBitmap(mRightHeadBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		
		
		mLeftTopBitmap = Bitmap.createBitmap(resBitmap, 0, 0, unitHeight, unitHeight);
		mLeftTopBitmap = Bitmap.createScaledBitmap(mLeftTopBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);;
		
		mRightTopBitmap = Bitmap.createBitmap(resBitmap, 2*unitHeight, 0, unitHeight, unitHeight);
		mRightTopBitmap = Bitmap.createScaledBitmap(mRightTopBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mLeftBottonBitmap = Bitmap.createBitmap(resBitmap, 0, unitHeight, unitHeight, unitHeight);
		mLeftBottonBitmap = Bitmap.createScaledBitmap(mLeftBottonBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		mRightBottonBitmap = Bitmap.createBitmap(resBitmap, 2*unitHeight, 2*unitHeight, unitHeight, unitHeight);
		mRightBottonBitmap = Bitmap.createScaledBitmap(mRightBottonBitmap, (int)Math.ceil(unitSize), (int)Math.ceil(unitSize), false);
		
		
	}
	public void drawLevel(Canvas canvas) {
		for (int i = 0; i < markLevel.length; i++) {
			for (int j = 0; j < markLevel[0].length; j++) {
				if (BLOCK == markLevel[i][j]) {
					canvas.drawBitmap(mBlockBitmap, j*unitSize, i*unitSize, null);
				}
			}
		}
	}
	
	public void drawSnake(Canvas canvas) {
		if (snakeList.size() < 3) {
			return;
		}
		Bitmap tempBitmap = null;
		//蛇头
		switch (direction) {
		case MainActivity.D_UP:
			tempBitmap = mUpHeadBitmap;
			break;
		case MainActivity.D_DOWN:
			tempBitmap = mDownHeadBitmap;
			break;
		case MainActivity.D_LEFT:
			tempBitmap = mLeftHeadBitmap;
			break;
		case MainActivity.D_RIGHT:
			tempBitmap = mRightHeadBitmap;
			break;
		default:
			break;
		}
		canvas.drawBitmap(tempBitmap, snakeList.get(0).x*unitSize, snakeList.get(0).y*unitSize, null);
		
		//蛇身
		int i = 1;
		for (i = 1; i < snakeList.size()-1; i++) {
			if (isVertical(i)) {
				tempBitmap = mVerticalBodyBitmap;
			} else if (isHorizontal(i)) {
				tempBitmap = mHorizontalBodyBitmap;
			} else if (isRightBottom(i)) {
				tempBitmap = mRightBottonBitmap;
			} else if (isRightTop(i)) {
				tempBitmap = mRightTopBitmap;
			} else if (isLeftBottom(i)) {
				tempBitmap = mLeftBottonBitmap;
			} else if (isLeftTop(i)) {
				tempBitmap = mLeftTopBitmap;
			} else {
				Toast.makeText(getContext(), "Error_snake_body", Toast.LENGTH_SHORT).show();
			}
			canvas.drawBitmap(tempBitmap, snakeList.get(i).x*unitSize, snakeList.get(i).y*unitSize, null);
		}
		//i = size()-1
		//蛇尾
		Point tailPoint = snakeList.get(i);
		Point beforeTailPoint = snakeList.get(i-1);
		if (tailPoint.x==beforeTailPoint.x && (tailPoint.y-beforeTailPoint.y)==-1) {
			tempBitmap = mUpTailBitmap;
		} else if (tailPoint.x==beforeTailPoint.x && (tailPoint.y-beforeTailPoint.y)==1) {
			tempBitmap = mDownTailBitmap;
		} else if (tailPoint.y==beforeTailPoint.y && (tailPoint.x-beforeTailPoint.x)==-1) {
			tempBitmap = mLeftTailBitmap;
		} else {
			tempBitmap = mRightTailBitmap;
		}
		canvas.drawBitmap(tempBitmap, snakeList.get(i).x*unitSize, snakeList.get(i).y*unitSize, null);
		
	}
	
	public void drawApple(Canvas canvas) {
		canvas.drawBitmap(mAppleBitmap, mApplePoint.x*unitSize, mApplePoint.y*unitSize, null);
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		//画墙 画蛇 画食物
		drawLevel(canvas);
		drawSnake(canvas);
		drawApple(canvas);
		//更新数组
	}
	
	public boolean isVertical(int i) {
		Point beforePoint = snakeList.get(i-1);
		Point curPoint	  = snakeList.get(i);
		Point afterPoint  = snakeList.get(i+1);
		if (i > 0 && i < snakeList.size()-1) {
			/////////////////////////////////////////////////////////////////////////
			if (beforePoint.x==curPoint.x && curPoint.x==afterPoint.x) {
				return true;
			} else {
				return false;
			}
			/////////////////////////////////////////////////////////////////////////
		} else {
			Toast.makeText(getContext(), "Error_judge", Toast.LENGTH_SHORT).show();
			return false;
		}
	}	
	public boolean isHorizontal(int i) {
		Point beforePoint = snakeList.get(i-1);
		Point curPoint	  = snakeList.get(i);
		Point afterPoint  = snakeList.get(i+1);
		if (i > 0 && i < snakeList.size()-1) {
			///////////////////////////////////////////////////////////////////////////
			if (beforePoint.y==curPoint.y && curPoint.y==afterPoint.y) {
				return true;
			} else {
				return false;
			}
			/////////////////////////////////////////////////////////////////////////
		} else {
			Toast.makeText(getContext(), "Error_judge", Toast.LENGTH_SHORT).show();
			return false;
		}
	}
	public boolean isLeftBottom(int i) {
		Point beforePoint = snakeList.get(i-1);
		Point curPoint	  = snakeList.get(i);
		Point afterPoint  = snakeList.get(i+1);
		if (i > 0 && i < snakeList.size()-1) {
			////////////////////////////////////////////////////////////////////////
			//考虑两个方向
			//1
			if (beforePoint.x==curPoint.x && (beforePoint.y-curPoint.y)==-1 &&
					afterPoint.y==curPoint.y && (afterPoint.x-curPoint.x)==1) {
				return true;
				
			} else if (afterPoint.x==curPoint.x && (afterPoint.y-curPoint.y)==-1 &&
					beforePoint.y==curPoint.y && (beforePoint.x-curPoint.x)==1) {//2
				return true;
			} else {
				return false;
			}
			///////////////////////////////////////////////////////////////////////
		} else {
			Toast.makeText(getContext(), "Error_judge", Toast.LENGTH_SHORT).show();
			return false;
		}
	}
	public boolean isLeftTop(int i) {
		Point beforePoint = snakeList.get(i-1);
		Point curPoint	  = snakeList.get(i);
		Point afterPoint  = snakeList.get(i+1);
		if (i > 0 && i < snakeList.size()-1) {
			////////////////////////////////////////////////////////////////////////
			//考虑两个方向
			//1
			if (beforePoint.x==curPoint.x && (beforePoint.y-curPoint.y)==1 &&
					afterPoint.y==curPoint.y && (afterPoint.x-curPoint.x)==1) {
				return true;
				
			} else if (afterPoint.x==curPoint.x && (afterPoint.y-curPoint.y)==1 &&
					beforePoint.y==curPoint.y && (beforePoint.x-curPoint.x)==1) {//2
				return true;
			} else {
				return false;
			}			///////////////////////////////////////////////////////////////////////
		} else {
			Toast.makeText(getContext(), "Error_judge", Toast.LENGTH_SHORT).show();
			return false;
		}	
	}
	public boolean isRightBottom(int i) {
		Point beforePoint = snakeList.get(i-1);
		Point curPoint	  = snakeList.get(i);
		Point afterPoint  = snakeList.get(i+1);
		if (i > 0 && i < snakeList.size()-1) {
			////////////////////////////////////////////////////////////////////////
			//考虑两个方向
			//1
			if (beforePoint.x==curPoint.x && (beforePoint.y-curPoint.y)==-1 &&
					afterPoint.y==curPoint.y && (afterPoint.x-curPoint.x)==-1) {
				return true;
				
			} else if (afterPoint.x==curPoint.x && (afterPoint.y-curPoint.y)==-1 &&
					beforePoint.y==curPoint.y && (beforePoint.x-curPoint.x)==-1) {//2
				return true;
			} else {
				return false;
			}
			///////////////////////////////////////////////////////////////////////
			
		} else {
			Toast.makeText(getContext(), "Error_judge", Toast.LENGTH_SHORT).show();
			return false;
		}	
	}
	public boolean isRightTop(int i) {
		Point beforePoint = snakeList.get(i-1);
		Point curPoint	  = snakeList.get(i);
		Point afterPoint  = snakeList.get(i+1);
		if (i > 0 && i < snakeList.size()-1) {
			////////////////////////////////////////////////////////////////////////
			//考虑两个方向
			//1
			if (beforePoint.x==curPoint.x && (beforePoint.y-curPoint.y)==1 &&
					afterPoint.y==curPoint.y && (afterPoint.x-curPoint.x)==-1) {
				return true;
				
			} else if (afterPoint.x==curPoint.x && (afterPoint.y-curPoint.y)==1 &&
					beforePoint.y==curPoint.y && (beforePoint.x-curPoint.x)==-1) {//2
				return true;
			} else {
				return false;
			}
			///////////////////////////////////////////////////////////////////////
		} else {
			Toast.makeText(getContext(), "Error_judge", Toast.LENGTH_SHORT).show();
			return false;
		}
	}
//获取抽象出一个特征值更高逼格
	
	public int getViewCount() {
		return VIEW_COUNT;
	}
	
	public List<Point> getSnakeList() {
		return snakeList;
	}
	
	public void setDirection(int direction) {
		if ((this.direction+direction)==(MainActivity.D_DOWN+MainActivity.D_UP) ||
				(this.direction+direction)==(MainActivity.D_LEFT+MainActivity.D_RIGHT)) {
			return;
		}
		this.direction = direction;
	}
	public int getDirection() {
		return direction;
	}
	
	public void removeTail() {
		//尾巴减去
		int len = snakeList.size();
		snakeList.remove(len - 1);
	}
	public Point getHead() {
		return snakeList.get(0);
	}
	public void addHead(Point newHead) {
		snakeList.add(0, newHead);
	}
	public Point getApple() {
		return mApplePoint;
	}
	public void setApple(Point newApple) {
		mApplePoint = newApple;
	}
	public void clearGameView() {
		snakeList.clear();
		initGame();
	}
	public int getScore() {
		return snakeList.size();
	}
	public void restart() {
		initGame();
		invalidate();
	}
}
