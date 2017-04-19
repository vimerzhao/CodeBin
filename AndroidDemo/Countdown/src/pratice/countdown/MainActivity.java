package pratice.countdown;

import java.util.Timer;
import java.util.TimerTask;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;


public class MainActivity extends Activity implements OnClickListener{
	
	private EditText inputTime;
	private Button getTime, startTime, stopTime;
	private	TextView showTime;
	private int i;
	private Timer timer;
	private TimerTask timerTask;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        init();
    }
    private void init() {
		inputTime = (EditText) findViewById(R.id.input_time);
		getTime = (Button) findViewById(R.id.get_time);
		startTime= (Button) findViewById(R.id.begin);
		stopTime = (Button) findViewById(R.id.stop);
		showTime = (TextView) findViewById(R.id.show_time);
		getTime.setOnClickListener(this);
		startTime.setOnClickListener(this);
		stopTime.setOnClickListener(this);
	}
    @Override
    public void onClick(View v) {
    	switch (v.getId()) {
		case R.id.get_time:
				showTime.setText(inputTime.getText().toString());
				i = Integer.parseInt(inputTime.getText().toString());
			break;
		case R.id.begin:
			start();
			break;
		case R.id.stop:
			stop();
			break;
		default:
			break;
		}
    	
    }
    private Handler mHandler = new Handler(){
    	public void handleMessage(Message msg) {
			showTime.setText(msg.arg1 + "");
			start();
		};
    };
    private void start() {
    	timer = new Timer();
    	timerTask = new TimerTask() {
			
			@Override
			public void run() {
				i--;
				Message message = mHandler.obtainMessage();
				message.arg1 = i;
				mHandler.sendMessage(message);	
				
			}
		};
		timer.schedule(timerTask, 1000);
    }
    private void stop() {
    	timer.cancel();
    }
}
