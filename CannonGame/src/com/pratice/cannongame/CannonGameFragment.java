package com.pratice.cannongame;

import android.app.Fragment;
import android.media.AudioManager;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class CannonGameFragment extends Fragment {
	private CannonView cannonView;
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		super.onCreateView(inflater, container, savedInstanceState);
		View view = inflater.inflate(R.layout.fragment_game, container, false);
		cannonView = (CannonView) view.findViewById(R.id.cannonView);
		return view;
	}
	@Override
	public void onActivityCreated(Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		//允许音量键设置游戏音量
		getActivity().setVolumeControlStream(AudioManager.STREAM_MUSIC);
	}
	@Override
	public void onPause() {
		super.onPause();
		cannonView.stopGame();
	}
	@Override
	public void onDestroy() {
		super.onDestroy();
		cannonView.releaseResources();
	}
	
}
