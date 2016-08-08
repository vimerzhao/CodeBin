package com.example.snake;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Point;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.SeekBar;
import android.widget.SeekBar.OnSeekBarChangeListener;
import android.widget.TextView;
import android.widget.Toast;


public class MainActivity extends Activity {
	
	private GameView gameView;
	private Button mUpButton;
	private Button mDownButton;
	private Button mLeftButton;
	private Button mRightButton;
	private Button mPlayStopButton;
	private SeekBar mSpeedControlBar;
	private TextView mScoreView;
	private Button mAIButton;
	private Button mExitButton;
	
	public static final int D_UP = 100;
	public static final int D_DOWN = 200;
	public static final int D_LEFT = 300;
	public static final int D_RIGHT = 400;
	
	private float speed;
	private float oldSpeed;
	
	private boolean isRun;
	private boolean isAiPlay;
	
	private AiPlayer aiPlayer;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        gameView = (GameView) findViewById(R.id.gameView);
		mUpButton = (Button) findViewById(R.id.btn_up);
		mDownButton = (Button) findViewById(R.id.btn_down);
		mLeftButton = (Button) findViewById(R.id.btn_left);
		mRightButton = (Button) findViewById(R.id.btn_right);
		mPlayStopButton = (Button) findViewById(R.id.btn_play_stop);        
        mSpeedControlBar = (SeekBar) findViewById(R.id.speed_control);
        mScoreView = (TextView) findViewById(R.id.score);
        mAIButton = (Button) findViewById(R.id.btn_ai);
        mExitButton = (Button) findViewById(R.id.btn_exit);
        oldSpeed = 300;
        timer.schedule(timerTask, 500, (long)oldSpeed);
        aiPlayer = new AiPlayer(gameView.getViewCount());
        //初始化
        initControlView();
        startGame();
    }
    public void startGame() {
    	mPlayStopButton.setText("开始");
    	mAIButton.setText("求助AI");
    	mScoreView.setText("");
    	initGame();
    	gameView.restart();
	}
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

	public void initControlView() {
				
		//设置监听器
		mUpButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (isRun && !isAiPlay) {
					gameView.setDirection(D_UP);
				}
							
			}
		});
		mDownButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (isRun && !isAiPlay){
					gameView.setDirection(D_DOWN);
				}
			}
		});
		mLeftButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (isRun && !isAiPlay) {
					gameView.setDirection(D_LEFT);				
				}
			}
		});
		mRightButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (isRun && !isAiPlay) {
					gameView.setDirection(D_RIGHT);	
				}
				
			}
		});
		mPlayStopButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (isRun == false) {
					isRun = true;
					mPlayStopButton.setText("暂停");
				} else {
					isRun = false;
					mPlayStopButton.setText("开始");
				}
			}
		});
		
		mSpeedControlBar.setOnSeekBarChangeListener(new OnSeekBarChangeListener() {
			
			@Override
			public void onStopTrackingTouch(SeekBar seekBar) {
				changeTime();
			}
			
			@Override
			public void onStartTrackingTouch(SeekBar arg0) {
				// TODO Auto-generated method stub
				
			}
			
			@Override
			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
				speed = 450-progress*4;
			}
		});
		
		mExitButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				finish();
			}
		});
		
		mAIButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (isAiPlay) {
					mAIButton.setText("求助AI");
					isAiPlay = false;
				} else {
					mAIButton.setText("退出AI");
					isAiPlay = true;
				}
			}
		});
		
	}
	
	public void changeTime() {
		Toast.makeText(MainActivity.this, "Run", Toast.LENGTH_SHORT).show();
		if (speed != oldSpeed) {
			timer.cancel();
			timer = new Timer();
			TimerTask timerTask = new TimerTask() {
				@Override
				public void run() {
					if (isRun) {
	
						Message message = new Message();
						handler.sendMessage(message);
					}
				}
			};
			timer.schedule(timerTask, 10, (long)speed);
			oldSpeed = speed;
		}
	}
	public void initGame() {
		speed = 500;
		isRun = false;
		isAiPlay =  false;

	}
	final Handler handler = new Handler(){
		
		public void handleMessage(Message msg) {
			if (isAiPlay) {
				aiPlayer.setData(gameView.getSnakeList(), gameView.getDirection(), gameView.getApple());
				gameView.setDirection(aiPlayer.getDirection());
			}
			msg.what = gameView.getDirection();
			switch (msg.what) {
			case D_UP:
				MoveUp();
				break;
			case D_DOWN:
				MoveDown();
				break;
			case D_LEFT:
				MoveLeft();	
				break;
			case D_RIGHT:
				MoveRight();
				break;
			default:
				break;
			}
			//gameView.setApple(createApple());
			mScoreView.setText(""+gameView.getScore());
			checkGame();
			gameView.invalidate();
			//Log.d("handle", "RUN");
			super.handleMessage(msg);
		}
	};
	
	Timer timer = new Timer();
	TimerTask timerTask = new TimerTask() {
		@Override
		public void run() {
			if (isRun) {
				Message message = new Message();
				handler.sendMessage(message);
			}
		}
	};


	public Point createApple() {
		boolean flag = true;
		int x = 0, y = 0;
		List<Point> snake = gameView.getSnakeList();
		while (flag) {
			x = (int) Math.ceil(Math.random()*(gameView.getViewCount()-2));
			y = (int) Math.ceil(Math.random()*(gameView.getViewCount()-2));
			flag = false;
			for (Point point:snake) {
				if (point.x==x && point.y == y) {
					flag = true;
					break;
				}
			}
		}
		
		return new Point(x, y);
	}

    @Override
    protected void onDestroy() {
        if (timer != null) {// 停止timer
            timer.cancel();
            timer = null;
        }
        super.onDestroy();
    }

    public int getDirection(float start_x, float start_y, float end_x, float end_y) {
    	boolean isLeftOrRight = Math.abs(start_x-end_x) > Math.abs(start_y-end_y)?true:false;
    	if (isLeftOrRight) {
    		boolean isLeft = start_x-end_x>0?true:false;
    		if (isLeft) {
    			return D_LEFT;
    		} else {
    			return D_RIGHT;
    		}
    	} else {
    		boolean isUp = start_y-end_y>0?true:false;
    		if (isUp) {
    			return D_UP;
    		} else {
    			return D_DOWN;
    		}
    	}
	}
    public void MoveUp() {
    	Point headPoint = gameView.getHead();
    	Point newHead = new Point(headPoint.x, headPoint.y-1);
    	Point Apple = gameView.getApple();
    	if (!newHead.equals(Apple)) {
    		gameView.removeTail();
    	} else {
    		gameView.setApple(createApple());
		}
    	gameView.addHead(newHead); 
	}
	public void MoveDown() {
    	Point headPoint = gameView.getHead();
    	Point newHead = new Point(headPoint.x, headPoint.y+1);
    	Point Apple = gameView.getApple();
    	if (!newHead.equals(Apple)) {
    		gameView.removeTail();
    	} else {
			gameView.setApple(createApple());
		}
    	gameView.addHead(newHead); 
    }
	public void MoveLeft() {
    	Point headPoint = gameView.getHead();
    	Point newHead = new Point(headPoint.x-1, headPoint.y);
    	Point Apple = gameView.getApple();
    	if (!newHead.equals(Apple)) {
    		gameView.removeTail();
    	} else {
    		gameView.setApple(createApple());
		}
    	gameView.addHead(newHead); 
	}
	public void MoveRight() {
    	Point headPoint = gameView.getHead();
    	Point newHead = new Point(headPoint.x+1, headPoint.y);
    	Point Apple = gameView.getApple();
    	if (!newHead.equals(Apple)) {
    		gameView.removeTail();
    	} else {
    		gameView.setApple(createApple());
		}
    	gameView.addHead(newHead); 
	}
 
	public void checkGame() {
		Point headPoint = gameView.getHead();
    	//撞墙
    	if (headPoint.x <= 0 || headPoint.y <= 0 || headPoint.x >= gameView.getViewCount() -1
    			|| headPoint.y >= gameView.getViewCount()-1 ) {
    		isRun = false;
    		pushDialog();
    	}
    	
    	//撞自己
    	List<Point> snakeList = gameView.getSnakeList();
    	for (int i = 1; i < snakeList.size(); i++) {
    		if (headPoint.equals(snakeList.get(i))) {
    			isRun = false;
    			pushDialog();
    		}
    	}
  
	}
	public void pushDialog() {
		AlertDialog.Builder dialog= new AlertDialog.Builder(MainActivity.this);
		dialog.setTitle("Game Over");
		dialog.setMessage("Replay once again");
		dialog.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
				startGame();
			}
		});
		dialog.setNegativeButton("No", new DialogInterface.OnClickListener() {
			
			@Override
			public void onClick(DialogInterface arg0, int arg1) {
			}
		});
		dialog.show();
	}
}
