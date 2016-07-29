package zhaoyu1995.aigobang;
import java.util.ArrayList;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Point;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Parcelable;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;



public class CheckerBoard extends View {
	private int checkerBoardWidth;//棋盘宽度
	private float unitWidth;//每一小格的宽度
	private static final int CHECKERBOARD_SIZE = 15;//棋盘大小，15*15
	private Bitmap whitePiece;//棋子载体
	private Bitmap blackPiece;
	private int pieceRadio;
	private float proportion = (float) (9.0 / 10.0);//棋子相对于单元格的缩放大小
	//落子记录
	boolean isBlack = true;
	private ArrayList<Point> whiteList = new ArrayList<Point>();
	private ArrayList<Point> blackList = new ArrayList<Point>();
	private Paint paint = new Paint();	
	private AI aiPlayer = new AI();//实例化一个AI
	private int gameState = 0;
	private int whiteWins = 0;
	private int blackWins = 0;
	private boolean isDialogOver = true;
	private Point lastPoint;
	private MediaPlayer player = MediaPlayer.create(getContext(), R.raw.voice);//果然要MP3
	private boolean AIvsAI = false;
	public void setAIvsAI(boolean b) {
		AIvsAI = b;
	}
	public CheckerBoard(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	public CheckerBoard(Context context, AttributeSet attrs) {
		super(context, attrs);
		
		//setBackgroundColor(0x44ff0000);//调试使用，使布局可见
		
		//初始化
		init();
	}
	public void init() {
		lastPoint = null;
		//设置画笔颜色
		paint.setColor(0x99000000);//黑色半透明
		paint.setAntiAlias(true);//抗锯齿
		paint.setDither(true);
		paint.setStyle(Paint.Style.STROKE);
		//宽度
		
		//举一反三！！！
		//初始化棋子
		blackPiece = BitmapFactory.decodeResource(getResources(), R.drawable.piece1);
		whitePiece = BitmapFactory.decodeResource(getResources(), R.drawable.piece2);
	}

	public CheckerBoard(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
	}
	//自定义View的测量问题，不太懂
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
		checkerBoardWidth = w;
		unitWidth = checkerBoardWidth*1.0f/CHECKERBOARD_SIZE;
		pieceRadio = (int)(unitWidth*proportion);
		//定制棋子大小啊
		blackPiece = Bitmap.createScaledBitmap(blackPiece, pieceRadio, pieceRadio, false);
		whitePiece = Bitmap.createScaledBitmap(whitePiece, pieceRadio, pieceRadio, false);
	}
	//绘制棋盘
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		drawBoard(canvas);
		
		//利用重载，保留了教程中的函数，便于查找自己编码时发生的错误
		drawPiece(canvas, true);
		
		player.start();
		
		//AI落子应该在玩家落子并完成棋子绘制后进行
		aiPlay();
		gameState = aiPlayer.isGmaeOver();
		checkGameState();
	}
	public void checkGameState() {
		if (0 == gameState) {
			return;
		}
		if (!isDialogOver) {//不明白为什么总是弹出两次，这里先如此用一个变量暂时处理
			return;
		}
		AlertDialog.Builder dialog = new AlertDialog.Builder(getContext());
		dialog.setTitle("游戏结束");
		if (1 == gameState) {
			blackWins++;
			dialog.setMessage("黑棋胜!总比分：黑"+blackWins+":"+whiteWins+"白");
		} else if (2 == gameState){
			whiteWins++;
			dialog.setMessage("白棋胜!总比分：黑"+blackWins+":"+whiteWins+"白");
		} else if (3 == gameState){
			dialog.setMessage("平局!总比分：黑"+blackWins+":"+whiteWins+"白");
		}
		dialog.setCancelable(false);
		dialog.setPositiveButton("再来一局", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				setAIvsAI(false);
				reStart();
				isDialogOver = true;
			}
		});
		dialog.setNegativeButton("取消", new DialogInterface.OnClickListener() {
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				setAIvsAI(false);
				isDialogOver = true;
				
			}
		});
		isDialogOver = false;
		dialog.show();
	}
	public void aiRePlay() {
		if (AIvsAI){
			if (isBlack){//其实一定是黑子，为了安全起见还是坚持一下
				Point point = aiPlayer.getAnswer();
				lastPoint = point;
				blackList.add(point);
				aiPlayer.updateWinCount(point, 1);
				isBlack = !isBlack;	
				invalidate();//刷新？？没有这个是不行的
			} else {
				Toast.makeText(getContext(), "有错误", Toast.LENGTH_LONG).show();//调试使用,可以不删除
			}
			
		}
	}
	public void aiPlay() {
		if (isBlack) {
			return;
		}
		Point point = aiPlayer.getAnswer();
		lastPoint = point;
		whiteList.add(point);
		aiPlayer.updateWinCount(point, 2);
		isBlack = !isBlack;	
		invalidate();//刷新？？没有这个是不行的
	}
	public void drawPiece(Canvas canvas) {
		for (int i = 0; i < whiteList.size(); i++) {
			Point point = whiteList.get(i);
			canvas.drawBitmap(whitePiece, 
					(point.x+(1-proportion)/2)*unitWidth, 
					(point.y+(1-proportion)/2)*unitWidth, null);
		}
		for (int i = 0; i < blackList.size(); i++) {
			Point point = blackList.get(i);
			canvas.drawBitmap(blackPiece, 
					(point.x+(1-proportion)/2)*unitWidth, 
					(point.y+(1-proportion)/2)*unitWidth, null);
		}
	}
	public void drawPiece(Canvas canvas, boolean b) {
		int[][] board = aiPlayer.getBoard();
		for (int i = 0; i < board.length; i++) {
			for (int j = 0; j < board[0].length; j++) {
				if (1 ==  board[i][j]) {
					canvas.drawBitmap(blackPiece, 
							(float)(j+(1-proportion)/2.0)*unitWidth, 
							(float)(i+(1-proportion)/2.0)*unitWidth, null);
				} else if (2 == board[i][j]) {
					canvas.drawBitmap(whitePiece, 
							(float)(j+(1-proportion)/2.0)*unitWidth, 
							(float)(i+(1-proportion)/2.0)*unitWidth, null);
				}

			}
		}
		
		//为提高实用性，最后落子的地方做标记
		Paint tempPaint = new Paint();
		tempPaint.setARGB(255, 255 ,0, 0);
		tempPaint.setAntiAlias(true);
		tempPaint.setStrokeWidth(2);
		if (lastPoint != null && isBlack){
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth, (lastPoint.y+(1-proportion)/2)*unitWidth,
					(lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio/3, (lastPoint.y+(1-proportion)/2)*unitWidth, tempPaint);
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth, (lastPoint.y+(1-proportion)/2)*unitWidth,
					(lastPoint.x+(1-proportion)/2)*unitWidth, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio/3, tempPaint);
			
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio, (lastPoint.y+(1-proportion)/2)*unitWidth,
					(lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio-pieceRadio/3, (lastPoint.y+(1-proportion)/2)*unitWidth, tempPaint);
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio, (lastPoint.y+(1-proportion)/2)*unitWidth,
					(lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio/3, tempPaint);
			
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio,
					(lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio/3, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio, tempPaint);
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio,
					(lastPoint.x+(1-proportion)/2)*unitWidth, (lastPoint.y+(1-proportion)/2)*unitWidth-pieceRadio/3+pieceRadio, tempPaint);
			
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio,
					(lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio-pieceRadio/3, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio, tempPaint);
			canvas.drawLine((lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio,
					(lastPoint.x+(1-proportion)/2)*unitWidth+pieceRadio, (lastPoint.y+(1-proportion)/2)*unitWidth+pieceRadio-pieceRadio/3, tempPaint);
		}
	    
	}
	//使用一次，初始化时绘制棋盘
	public void drawBoard(Canvas canvas) {
		//获取变量
		int boardW = checkerBoardWidth;
		float unitH = unitWidth;
		for (int i = 0; i < CHECKERBOARD_SIZE; i++) {
			int startX = (int)(unitH/2);
			int endX = (int)(boardW - unitH/2);
			int y = (int)((0.5+i)*unitH);
			canvas.drawLine(startX, y, endX, y, paint);
			canvas.drawLine(y, startX, y, endX, paint);
		}
	}
	private Point getValidPoint(int x, int y) {
		//注意用的是哪个坐标系
		return new Point((int)(x/unitWidth), (int)(y/unitWidth));
	}
	//处理点击事件
	@SuppressLint("ClickableViewAccessibility")
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		
		if (AIvsAI){
			aiRePlay();//机机博弈时这个函数代替点击事件
			return false;
		}
		
		//游戏结束不允许落子
		if (0 != gameState) {
			return false;
		}
		int action = event.getAction();
		if (MotionEvent.ACTION_UP == action) {
			int x = (int) event.getX();
			int y = (int) event.getY();
			Point point = getValidPoint(x, y);
			//判断该位置是否有棋子
			if (whiteList.contains(point) || blackList.contains(point)) {
				return false;
			}
			if (isBlack) {
				blackList.add(point);
				lastPoint = point;
				aiPlayer.updateWinCount(point, 1);
				isBlack = !isBlack;
				invalidate();
			}
			
			//为改为AI，注释点下面的
//			else {
//				whiteList.add(point);
//				aiPlayer.updateWinCount(point, 2);
//			}
			
			
			
		}
		//必须返回true，表明能处理此事件
		return true;
	}
	
	/*
	 * 添加了ID仍然不能存储，这目前还未解决，先学习一个这个知识点再回来解决/
	 * 临时处理方法是设置为竖屏
	 */
	//棋盘存储
	private static final String INSTANCE = "instance";
	private static final String INSTANCE_GAME_STATE = "instance_game_state";
	private static final String INSTANCE_WHITE_ARRAY = "instance_white_array";
	private static final String INSTANCE_BLACK_ARRAY = "instance_black_array";
	@Override
	protected Parcelable onSaveInstanceState() {
		Bundle bundle = new Bundle();
		//系统默认的不要忘记存储
		bundle.putParcelable(INSTANCE, super.onSaveInstanceState());
		bundle.putInt(INSTANCE_GAME_STATE, gameState);
		bundle.putParcelableArrayList(INSTANCE_WHITE_ARRAY, whiteList);
		bundle.putParcelableArrayList(INSTANCE_BLACK_ARRAY, blackList);
		return bundle;
	}
	@Override
	protected void onRestoreInstanceState(Parcelable state) {
		if (state instanceof Bundle) {
			Bundle bundle = (Bundle)state;
			gameState = bundle.getInt(INSTANCE_GAME_STATE);
			whiteList = bundle.getParcelableArrayList(INSTANCE_WHITE_ARRAY);
			blackList = bundle.getParcelableArrayList(INSTANCE_BLACK_ARRAY);
			//这一句不能忘记
			super.onRestoreInstanceState(bundle.getParcelable(INSTANCE));
			return;
		}
		super.onRestoreInstanceState(state);
	}
	public void reStart() {
		whiteList.clear();
		blackList.clear();
		gameState = 0;
		lastPoint = null;
		aiPlayer.init();
		invalidate();
	}
}
