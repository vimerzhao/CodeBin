package com.mygdx.game;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.MotionEvent;

import com.badlogic.gdx.backends.android.AndroidFragmentApplication;

public class AndroidLauncher extends FragmentActivity implements
        AndroidFragmentApplication.Callbacks {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_activity_demo);

        Demo1Fragment demoFragment = new Demo1Fragment();
        getSupportFragmentManager().beginTransaction().add(R.id.content_frame, demoFragment).commit();
    }



    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        return super.dispatchTouchEvent(event);
    }

    @Override
    public void exit() {
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }
}
