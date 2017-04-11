package com.example.jh.tangram;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.graphics.Point;
import android.os.Bundle;
import android.text.Layout;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.RelativeLayout;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

/**
 * Created by lmasi on 2017. 3. 19..
 */

public class HomeActivity extends Activity {

    RelativeLayout main;
    ImageView btn_play;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        super.onCreate(savedInstanceState);
        setContentView(R.layout.homeactivity);

        ScreenParameter.setScreenparam_x(ScreenSize().x / ScreenParameter.getDefaultsizeX());
        ScreenParameter.setScreenparam_y(ScreenSize().y / ScreenParameter.getDefaultsizeY());
        ScreenParameter.setScreen_x(ScreenSize().x);
        ScreenParameter.setScreen_y(ScreenSize().y);


        main = (RelativeLayout)findViewById(R.id.main);
        main.setBackground(getResources().getDrawable(R.drawable.main_bg));


        btn_play = new ImageView(getApplicationContext());
        RelativeLayout.LayoutParams params = new RelativeLayout.LayoutParams((int)(317 * ScreenParameter.getScreenparam_x()), (int)(317 * ScreenParameter.getScreenparam_y()));
        params.setMargins((int)(624 * ScreenParameter.getScreenparam_x()), (int)(531 * ScreenParameter.getScreenparam_y()), 0, 0);
        btn_play.setLayoutParams(params);
        btn_play.setBackground(getResources().getDrawable(R.drawable.btn_play));
        main.addView(btn_play);
        btn_play.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                if(event.getAction() == MotionEvent.ACTION_UP)
                {
                    startActivity(new Intent(HomeActivity.this, AnimationPlayActivity.class).putExtra("idx", -1));
                    finish();
                }

                return true;
            }
        });

    }


    public Point ScreenSize() { //현재 스크린의 사이즈를 가져오는 메서드. 정형화된 틀이다.

        Display display = getWindowManager().getDefaultDisplay();
        DisplayMetrics metrics = new DisplayMetrics();
        display.getMetrics(metrics);
        int width = metrics.widthPixels;
        int height = metrics.heightPixels;

        Point size = new Point(width, height);

        return size;

    }
}
