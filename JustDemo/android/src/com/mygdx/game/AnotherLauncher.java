package com.mygdx.game;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.MotionEvent;

import com.badlogic.gdx.backends.android.AndroidFragmentApplication;

public class AnotherLauncher extends FragmentActivity implements
        AndroidFragmentApplication.Callbacks {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.fragment_activity_demo);

        Demo2Fragment demoFragment = new Demo2Fragment();
        getSupportFragmentManager().beginTransaction().add(R.id.content_frame, demoFragment).commit();
    }



    @Override
    public boolean dispatchTouchEvent(MotionEvent event) {
        return super.dispatchTouchEvent(event);
    }

    @Override
    public void exit() {

    }
}

