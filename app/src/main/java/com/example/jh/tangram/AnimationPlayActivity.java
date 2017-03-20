package com.example.jh.tangram;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Color;
import android.os.Bundle;
import android.view.ViewGroup;
import android.widget.RelativeLayout;

/**
 * Created by lmasi on 2017. 3. 8..
 */

public class AnimationPlayActivity extends Activity {


    RelativeLayout main;


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
            animationPlayer.startActivityAfterVideoWithIntent(R.raw.ending_sound, new Intent(AnimationPlayActivity.this, AnimationPlayActivity.class).putExtra("idx", 1000), this);
            RelativeLayout.LayoutParams videoParams = new RelativeLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
            videoParams.addRule(RelativeLayout.CENTER_IN_PARENT);
            animationPlayer.setLayoutParams(videoParams);
            main.addView(animationPlayer);
        }

        else if(idx == 1000)
            finish();
    }
}
