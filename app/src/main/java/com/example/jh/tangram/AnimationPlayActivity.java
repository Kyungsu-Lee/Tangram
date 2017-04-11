package com.example.jh.tangram;

import android.app.Activity;
import android.app.Application;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.RelativeLayout;

/**
 * Created by lmasi on 2017. 3. 8..
 */

public class AnimationPlayActivity extends Activity {


    RelativeLayout main;
    ImageView fadeout;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.animationplayactivity);


        main = (RelativeLayout) findViewById(R.id.main);
        main.setBackgroundColor(Color.WHITE);

        int idx = getIntent().getIntExtra("idx", -1);

        if(idx == -1)
        {
            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.scenes1_01_sound, new Intent(AnimationPlayActivity.this, MainActivity.class).putExtra("idx", 1), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 1) {

            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.scenes1_02_sound, new Intent(AnimationPlayActivity.this, AnimationPlayActivity.class).putExtra("idx", 102), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 102)
        {
            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.scenes2_01_sound, new Intent(AnimationPlayActivity.this, MainActivity.class).putExtra("idx", 0), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 0) {

            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.scenes2_02_sound, new Intent(AnimationPlayActivity.this, AnimationPlayActivity.class).putExtra("idx", 103), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 103)
        {
            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.scenes3_01_sound, new Intent(AnimationPlayActivity.this, MainActivity.class).putExtra("idx", 2), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 2) {

            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.scenes3_02_sound, new Intent(AnimationPlayActivity.this, AnimationPlayActivity.class).putExtra("idx", 104), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 104)
        {
            AnimationPlayer animationPlayer = new AnimationPlayer(AnimationPlayActivity.this);
           // animationPlayer.startActivityAfterVideoWithIntent(R.raw.ending_sound, new Intent(AnimationPlayActivity.this, AnimationPlayActivity.class).putExtra("idx", 1000), this);
            animationPlayer.play(animationPlayer.UriParse(R.raw.ending_sound), new MediaPlayer.OnCompletionListener() {
                @Override
                public void onCompletion(MediaPlayer mp) {
                    Handler hd = new Handler()
                    {
                        int time = 0;

                        @Override
                        public void handleMessage(Message msg) {
                            super.handleMessage(msg);

                            if(time >=  400)
                                android.os.Process.killProcess(android.os.Process.myPid());

                            fadeout.setBackgroundColor(Color.argb(time > 256 ? 255 : time, 0, 0, 0));
                            time += 20;
                            sendEmptyMessageDelayed(0, 40);
                        }
                    };

                    hd.sendEmptyMessage(0);
                }
            });
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        fadeout = new ImageView(AnimationPlayActivity.this);
        fadeout.setLayoutParams(new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        fadeout.setBackgroundColor(Color.argb(0, 0, 0, 0));
        main.addView(fadeout);

    }
}
