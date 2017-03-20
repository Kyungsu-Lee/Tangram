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
                    startActivity(new Intent(HomeActivity.this, AnimationPlayActivity.class));
                    finish();
                }

                return true;
            }
        });


        Log.d("aaa", getFilesDir().getAbsolutePath());

        String dirPath = "getFilesDir().getAbsolutePath()";
        File file = new File(dirPath);

        if( !file.exists() ) {
            file.mkdirs();
        }

        String testStr = "dog\n" +
                "95\n" +
                "109\n" +
                "180\n" +
                "167\n" +
                "53\n" +
                "153\n" +
                "91\n" +
                "133\n" +
                "167\n" +
                "58\n" +
                "126\n" +
                "73\n" +
                "76\n" +
                "62\n" +
                "44\n" +
                "84\n" +
                "129\n" +
                "38\n" +
                "132\n" +
                "52\n" +
                "106\n" +
                "224\n" +
                "-153\n" +
                "41\n" +
                "316\n" +
                "270\n" +
                "45\n" +
                "138\n" +
                "\n" +
                "cat\n" +
                "128\n" +
                "107\n" +
                "51\n" +
                "147\n" +
                "69\n" +
                "142\n" +
                "222\n" +
                "179\n" +
                "264\n" +
                "90\n" +
                "243\n" +
                "84\n" +
                "42\n" +
                "177\n" +
                "43\n" +
                "116\n" +
                "108\n" +
                "126\n" +
                "216\n" +
                "47\n" +
                "211\n" +
                "2\n" +
                "-158\n" +
                "88\n" +
                "183\n" +
                "9\n" +
                "43\n" +
                "180\n" +
                "\n" +
                "bird\n" +
                "110\n" +
                "107\n" +
                "196\n" +
                "84\n" +
                "182\n" +
                "145\n" +
                "92\n" +
                "207\n" +
                "55\n" +
                "91\n" +
                "136\n" +
                "114\n" +
                "42\n" +
                "98\n" +
                "44\n" +
                "156\n" +
                "181\n" +
                "71\n" +
                "99\n" +
                "87\n" +
                "117\n" +
                "222\n" +
                "29\n" +
                "42\n" +
                "222\n" +
                "315\n" +
                "185\n" +
                "136\n" +
                "\n" +
                "bird\n" +
                "92\n" +
                "98\n" +
                "78\n" +
                "103\n" +
                "52\n" +
                "63\n" +
                "190\n" +
                "37\n" +
                "194\n" +
                "126\n" +
                "81\n" +
                "169\n" +
                "39\n" +
                "76\n" +
                "148\n" +
                "180\n" +
                "98\n" +
                "93\n" +
                "100\n" +
                "137\n" +
                "115\n" +
                "85\n" +
                "-16\n" +
                "86\n" +
                "266\n" +
                "353\n" +
                "264\n" +
                "272\n";
        File savefile = new File(dirPath+"/answer.txt");
        try{
            FileOutputStream fos = new FileOutputStream(savefile);
            fos.write(testStr.getBytes());
            fos.close();
        } catch(IOException e){}


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
