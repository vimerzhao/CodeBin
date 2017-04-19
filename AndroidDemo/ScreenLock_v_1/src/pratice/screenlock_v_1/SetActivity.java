package pratice.screenlock_v_1;

import java.util.ArrayList;
import java.util.List;

import pratice.screenlock_v_1.GestureLock.onDrawFinishedListener;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class SetActivity extends Activity {
	private List<Integer> passList = new ArrayList<Integer>();
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_set);
		final GestureLock lock = (GestureLock) findViewById(R.id.LockView);
		Button btn_reset = (Button) findViewById(R.id.reset);
		Button btn_save = (Button) findViewById(R.id.save);
		
		//这一块驾还是不太熟练，接口 监听器
		lock.setOnDrawFinishedListener(new onDrawFinishedListener() {
			
			@Override
			public boolean onDrawFinished(List<Integer> passList) {
				if (passList.size() < 3) {
					Toast.makeText(SetActivity.this, "密码不能少于3个点", Toast.LENGTH_SHORT).show();
					return false;
				} else {
					SetActivity.this.passList = passList;
					return true;
				}
			}
		});
		
		
		btn_reset.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				lock.resetPoint();
			}
		});
		
		btn_save.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if (passList != null) {
					StringBuilder str = new StringBuilder();
					for (Integer i:passList) {
						str.append(i);
					}
					//这一段初次接触
					SharedPreferences sp = SetActivity.this.getSharedPreferences("password", SetActivity.MODE_PRIVATE);
					SharedPreferences.Editor editor = sp.edit();
					editor.putString("password", str.toString());
					editor.commit();
					
					Toast.makeText(SetActivity.this, "保存完成", Toast.LENGTH_SHORT).show();
				}
				
			}
		});
	}
}
