package pratice.screenlock_v_1;

import java.util.ArrayList;
import java.util.List;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

public class GestureLock extends View{
	private Point[][] points = new Point[3][3];
	//按钮资源
	private Bitmap errorPoint;
	private Bitmap normalPoint;
	private Bitmap pressedPoint;
	private Paint paint = new Paint(Paint.ANTI_ALIAS_FLAG);
	private float pointR;
	private float mouseX, mouseY;			//手指位置
	private boolean isPaint;				//是否在绘制状态
	private ArrayList<Point> pointList = new ArrayList<Point>();
	
	private ArrayList<Integer> passList = new ArrayList<Integer>();
	private onDrawFinishedListener listener;
	
	
	public GestureLock(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
	}

	public GestureLock(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}

	public GestureLock(Context context, AttributeSet attrs, int defStyleAttr) {
		super(context, attrs, defStyleAttr);
		// TODO Auto-generated constructor stub
	}
	
	@Override
	protected void onSizeChanged(int w, int h, int oldw, int oldh) {
		// TODO Auto-generated method stub
		super.onSizeChanged(w, h, oldw, oldh);
		init();
	}
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		drawPoints(canvas);
		if (pointList.size() > 0) {
			for (int i = 0; i < pointList.size()-1; i++) {
				drawLine(canvas, pointList.get(i), pointList.get(i+1));
			}
			if (isPaint) {
				drawLine(canvas, pointList.get(pointList.size()-1), new Point(mouseX, mouseY));
			}
		}
		
	}
	public void init() {
		int width = getWidth();
		int height = getHeight();
		int offset = Math.abs(height-width)/2;//计算偏移量
		int offsetX, offsetY;
		int unitWidth;
		if (width>height) {
			offsetX = offset;
			offsetY = 0;
			unitWidth = height/4;
		} else {
			offsetX = 0;
			offsetY = offset;
			unitWidth = width/4;
		}
		
		pointR = (float) (unitWidth*3.0/4.0);
		
		points[0][0] = new Point(offsetX + unitWidth, offsetY + unitWidth);
		points[0][1] = new Point(offsetX + unitWidth*2, offsetY + unitWidth);
		points[0][2] = new Point(offsetX + unitWidth*3, offsetY + unitWidth);
		
		points[1][0] = new Point(offsetX + unitWidth, offsetY + unitWidth*2);
		points[1][1] = new Point(offsetX + unitWidth*2, offsetY + unitWidth*2);
		points[1][2] = new Point(offsetX + unitWidth*3, offsetY + unitWidth*2);
		
		points[2][0] = new Point(offsetX + unitWidth, offsetY + unitWidth*3);
		points[2][1] = new Point(offsetX + unitWidth*2, offsetY + unitWidth*3);
		points[2][2] = new Point(offsetX + unitWidth*3, offsetY + unitWidth*3);
		
		//获取资源
		errorPoint = BitmapFactory.decodeResource(getResources(), R.drawable.error);
		normalPoint = BitmapFactory.decodeResource(getResources(), R.drawable.normal);
		pressedPoint = BitmapFactory.decodeResource(getResources(), R.drawable.press);
		//调整图片大小
		normalPoint= Bitmap.createScaledBitmap(normalPoint, (int)pointR, (int)pointR, false);
		pressedPoint = Bitmap.createScaledBitmap(pressedPoint, (int)pointR, (int)pointR, false);
		errorPoint = Bitmap.createScaledBitmap(errorPoint, (int)pointR, (int)pointR, false);
		//注意这个才是半径createScaledBitmap指定的是宽和高
		pointR = pointR/2;
		
		
		isPaint = false;
		pointList.clear();
	}
	
	private void drawPoints(Canvas canvas){
		for (int i = 0; i < points.length; i++) {
			for (int j = 0; j < points[0].length; j++) {
				if (Point.STATE_ERROR == points[i][j].state) {
					//!!!!这个类名取得不好，与系统重叠
					canvas.drawBitmap(errorPoint, points[i][j].x-pointR, points[i][j].y-pointR, paint);
					
				} else if (Point.STATE_NORMAL == points[i][j].state) {
					canvas.drawBitmap(normalPoint, points[i][j].x-pointR, points[i][j].y-pointR, paint);					
				} else {
					//Press
					canvas.drawBitmap(pressedPoint, points[i][j].x-pointR, points[i][j].y-pointR, paint);					
				}
			}
		}
	}
	
	//处理触摸事件，典型
	@SuppressLint("ClickableViewAccessibility") @Override
	public boolean onTouchEvent(MotionEvent event) {
		mouseX = event.getX();
		mouseY = event.getY();
		
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			resetPoint();
			Point curPoint = getSelectedPoint();
			if (null != curPoint) {
				isPaint = true;
				points[(int)curPoint.x][(int)curPoint.y].state = Point.STATE_PRESSED;
				pointList.add(points[(int)curPoint.x][(int)curPoint.y]);
				passList.add((int)curPoint.x*3+(int)curPoint.y);
			}
			break;			
		case MotionEvent.ACTION_MOVE:
			if (isPaint) {
				curPoint = getSelectedPoint();
				if (null != curPoint) {
					if (!pointList.contains(points[(int)curPoint.x][(int)curPoint.y])) {
						points[(int)curPoint.x][(int)curPoint.y].state = Point.STATE_PRESSED;
						pointList.add(points[(int)curPoint.x][(int)curPoint.y]);
						passList.add((int)curPoint.x*3+(int)curPoint.y);
					}
				}
			}
			break;
			
		case MotionEvent.ACTION_UP:
			boolean unlock = false;
			if (null!=listener && isPaint) {//图案解锁
				unlock = listener.onDrawFinished(passList);
			}
			if (!unlock) {
				for (Point point:pointList) {
					point.state = Point.STATE_ERROR;
				}
			}
			isPaint = false;
			break;

		default:
			break;
		}
		
		this.postInvalidate();
		
		return true;
	}
	
	private Point getSelectedPoint() {
		Point mousePoint = new Point(mouseX, mouseY);
		for (int i = 0; i < points.length; i++) {
			for (int j = 0; j < points[0].length; j++) {
				if (points[i][j].getDistance(mousePoint) < pointR) {
					Point ansPoint = new Point(i, j);
					return ansPoint;
				}
			}
		}
		return null;
	}
	
	private void drawLine(Canvas canvas, Point start, Point end) {
		Paint linePaint = new Paint();
		linePaint.setStrokeWidth(pointR/3);//设置画笔宽度
		if (Point.STATE_PRESSED == start.state) {
			linePaint.setARGB(255, 185, 160, 58);
			linePaint.setAntiAlias(true);
			
			canvas.drawLine(start.x, start.y, end.x, end.y, linePaint);
		} else if (Point.STATE_ERROR == start.state) {
			linePaint.setARGB(255, 243, 1, 0);
			canvas.drawLine(start.x, start.y, end.x, end.y, linePaint);
		}
	}
	
	public void resetPoint() {
		passList.clear();
		pointList.clear();
		for (int i = 0; i < points.length; i++) {
			for (int j = 0; j < points[0].length; j++) {
				points[i][j].state = Point.STATE_NORMAL;	
			}
		}
		this.postInvalidate();
	}
	
	
	//接口
	public interface onDrawFinishedListener {
		boolean onDrawFinished(List<Integer> passList);
	}
	
	public void setOnDrawFinishedListener(onDrawFinishedListener listener) {
		this.listener = listener;
	}
}
