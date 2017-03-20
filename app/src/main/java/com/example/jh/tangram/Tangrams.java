package com.example.jh.tangram;

import android.content.Context;
import android.util.Log;
import android.widget.RelativeLayout;

/**
 * Created by lmasi on 2017. 3. 13..
 */

public class Tangrams extends RelativeLayout {

    private static final int BLOCK_NUM = 7;
    private static final int[][] BLOCK_SIZE = {
            {448, 448},
            {448, 224},
            {224, 221},
            {315, 315},
            {317, 161},
            {448, 448},
            {224, 224}
    };

    private  static final int[] img = {
            R.drawable.red_1,
            R.drawable.orange_1,
            R.drawable.yellow_1,
            R.drawable.green_1,
            R.drawable.sky_1,
            R.drawable.blue_1,
            R.drawable.pink_1
    };

    private float param_x;
    private float param_y;

    private TangramBlock[] blocks;

    public Tangrams(Context context) {
        super(context);

        param_x = (float)ScreenParameter.getScreenparam_x();
        param_y = (float)ScreenParameter.getScreenparam_y();

        param_x *= 0.9;
        param_y *= 0.9;

        Log.d("message", Double.toString(param_x));

        blocks = new TangramBlock[BLOCK_NUM];

        for(int i=0; i<blocks.length; i++) {
            blocks[i] = new TangramBlock(context, img[i], img[i] + 1);
            LayoutParams params = new LayoutParams((int)(param_x * BLOCK_SIZE[i][0]), (int)(param_y * BLOCK_SIZE[i][1]));
            blocks[i].setLayoutParams(params);
            this.addView(blocks[i]);
        }

    }

    public void makeBlock(int kind) // 0 : dog  1 : cat 2 : bird
    {
        if(kind == 1)   //cat
        {
            RelativeLayout.LayoutParams paramSS = new RelativeLayout.LayoutParams((int)(1010 * ScreenParameter.getScreenparam_x()), (int)(1200 * ScreenParameter.getScreenparam_y()));
            paramSS.addRule(RelativeLayout.CENTER_IN_PARENT);
            paramSS.setMargins((int)(300 * ScreenParameter.getScreenparam_x()), 0, 0, 0);
            this.setLayoutParams(paramSS);

            //blocks[0].setX(515 * param_x);
            //blocks[0].setY(1286 * param_y);
            LayoutParams p1 = new LayoutParams((int)(param_x * BLOCK_SIZE[0][0]), (int)(param_y * BLOCK_SIZE[0][1]));
            p1.setMargins((int)(11 * param_x), (int)(560 * param_y), 0, 0);
            blocks[0].setLayoutParams(p1);
            blocks[0].setRotation(45);

            //blocks[1].setX(401 * param_x);
            //blocks[1].setY(1001 * param_y);
            LayoutParams p2 = new LayoutParams((int)(param_x * BLOCK_SIZE[1][0]), (int)(param_y * BLOCK_SIZE[1][1]));
            p2.setMargins((int)(533 * param_x), (int)(1008 * param_y), 0, 0);
            blocks[1].setLayoutParams(p2);
            blocks[1].setScaleX(-1);

            //blocks[2].setX(1083 * param_x);
            //blocks[2].setY(1425 * param_y);
            LayoutParams p3 = new LayoutParams((int)(param_x * BLOCK_SIZE[2][0]), (int)(param_y * BLOCK_SIZE[2][1]));
            p3.setMargins((int)(123 * param_x), (int)(198 * param_y), 0, 0);
            blocks[2].setLayoutParams(p3);
            blocks[2].setRotation(45);

            blocks[3].setX(78 * param_x);
            blocks[3].setY(532 * param_y);
            blocks[3].setRotation(-45);

            blocks[4].setX(0 * param_x);
            blocks[4].setY(69 * param_y);
            blocks[4].setRotation(90);

            blocks[5].setX(104 * param_x);
            blocks[5].setY(784 * param_y);
            blocks[5].setRotation(-90);

            blocks[6].setX(282 * param_x);
            blocks[6].setY(40 * param_y);
            blocks[6].setRotation(360-225);
        }

        else if(kind == 0)   //dog
        {
            RelativeLayout.LayoutParams paramSS = new RelativeLayout.LayoutParams((int)(990 * ScreenParameter.getScreenparam_x()), (int)(763 * ScreenParameter.getScreenparam_y()));
            paramSS.addRule(RelativeLayout.CENTER_IN_PARENT);
            this.setLayoutParams(paramSS);

            blocks[0].setX(541 * param_x);
            blocks[0].setY(0 * param_y);

            blocks[1].setX(541 * param_x);
            blocks[1].setY(447 * param_y);

            blocks[2].setX(317 * param_x);
            blocks[2].setY(225 * param_y);

            blocks[3].setX(93 * param_x);
            blocks[3].setY(447 * param_y);

            blocks[4].setX(0 * param_x);
            blocks[4].setY(64 * param_y);

            blocks[5].setX(541 * param_x);
            blocks[5].setY(0 * param_y);

            blocks[6].setX(93 * param_x);
            blocks[6].setY(225 * param_y);
        }

        else if(kind == 2)   //dog
        {
            RelativeLayout.LayoutParams paramSS = new RelativeLayout.LayoutParams((int)(1220 * ScreenParameter.getScreenparam_x()), (int)(1320 * ScreenParameter.getScreenparam_y()));
            paramSS.addRule(RelativeLayout.CENTER_IN_PARENT);
            this.setLayoutParams(paramSS);

            blocks[0].setX(97 * param_x);
            blocks[0].setY(897 * param_y);
            blocks[0].setRotation(-90);

            blocks[1].setX(97 * param_x);
            blocks[1].setY(452 * param_y);
            blocks[1].setRotation(0);

            blocks[2].setX(545 * param_x);
            blocks[2].setY(671 * param_y);
            blocks[2].setRotation(0);

            blocks[3].setX(993 * param_x);
            blocks[3].setY(895 * param_y);
            blocks[3].setRotation(0);

            blocks[4].setX(333 * param_x);
            blocks[4].setY(653 * param_y);
            blocks[4].setRotation(45);

            blocks[5].setX(545 * param_x);
            blocks[5].setY(212 * param_y);
            blocks[5].setRotation(45);

            blocks[6].setX(769 * param_x);
            blocks[6].setY(670 * param_y);
            blocks[6].setRotation(90);
        }
    }

    public boolean changBlocks(int[] arr)
    {
        boolean flag = true;


        for(int i=0; i<BLOCK_NUM; i++)
        {
            flag &= isInBlock(arr, i);

            if(isInBlock(arr, i))
                blocks[i].changeImage();
            else if(!isInBlock(arr, i))
                blocks[i].originalImage();
        }

        return flag;
    }

    private boolean isInBlock(int[] arr, int n)
    {
        boolean flag = false;

        for(int i : arr)
            flag |= i == n;

        return flag;
    }
}
