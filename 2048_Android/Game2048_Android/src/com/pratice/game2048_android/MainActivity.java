package com.pratice.game2048_android;

import android.app.Activity;
import android.os.Bundle;
import android.view.Menu;
//import android.view.MenuItem;
//import android.view.TextureView;
import android.widget.TextView;


public class MainActivity extends Activity {
	private int score;
	private TextView gameScore;
	private static MainActivity mainActivity;
	public MainActivity() {
		mainActivity = this;
	}
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        gameScore = (TextView)findViewById(R.id.gameScore);
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }
    public static MainActivity getMainActivity() {
		return mainActivity;
	}
    public void clearScore() {
		score = 0;
		showScore();
	}
    public void showScore() {
		gameScore.setText(score+"");
	}
    // ¼Æ·Ö
    public void addScore(int s) {
		score += s;
		showScore();
	}
}
