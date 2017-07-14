package com.mygdx.game;

import android.graphics.PixelFormat;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;

import com.badlogic.gdx.backends.android.AndroidApplicationConfiguration;
import com.badlogic.gdx.backends.android.AndroidFragmentApplication;


public class Demo1Fragment extends AndroidFragmentApplication {
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        super.onCreateView(inflater, container, savedInstanceState);
        // return initializeForView(new Another3DLoader());
        AndroidApplicationConfiguration config = new AndroidApplicationConfiguration();
        // 这一句必须有,不能删除
        config.a = config.r = config.g = config.b = 8;

        View view = initializeForView(new Basic3DLoader(), config);
        if (graphics.getView() instanceof SurfaceView) {
            SurfaceView surfaceView = (SurfaceView) graphics.getView();
            surfaceView.getHolder().setFormat(PixelFormat.TRANSPARENT);
            surfaceView.setZOrderOnTop(true);
        }
        return view;
    }
}
