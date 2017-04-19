package pratice.screenlock_v_1;

import java.util.List;

import pratice.screenlock_v_1.GestureLock.onDrawFinishedListener;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

public class TestActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_test);
		SharedPreferences sp = getSharedPreferences("password", TestActivity.MODE_PRIVATE);
		final String password = sp.getString("password", "");
		
		final GestureLock lock = (GestureLock) findViewById(R.id.LockView);
		lock.setOnDrawFinishedListener(new onDrawFinishedListener() {
			
			@Override
			public boolean onDrawFinished(List<Integer> passList) {
				StringBuilder sb = new StringBuilder();
				for (Integer i:passList) {
					sb.append(i);
				}
				if (sb.toString().equals(password)) {
					Toast.makeText(TestActivity.this, "√‹¬Î’˝»∑", Toast.LENGTH_SHORT).show();
					return true;
				} else {
					Toast.makeText(TestActivity.this, "√‹¬Î¥ÌŒÛ", Toast.LENGTH_SHORT).show();Log.d("error", sb.toString());
					return false;
				}
			}
		});
	}
}
