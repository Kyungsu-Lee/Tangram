package com.example.jh.tangram;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.widget.ImageView;

/**
 * Created by lmasi on 2017. 3. 8..
 */

public class TangramBlock extends ImageView{

    private Drawable originImage;
    private Drawable changedImage;

    private Drawable currentImage;

    private boolean isOriginalImageFlag;

    private TangramBlock(Context context) {
        super(context);
    }

    public TangramBlock(Context context, int originImage, int changedImage)
    {
        super(context);

        this.isOriginalImageFlag = true;

        this.originImage = getResources().getDrawable(originImage);
        this.changedImage = getResources().getDrawable(changedImage);
        this.currentImage = getResources().getDrawable(originImage);

        this.setBackground(this.originImage);
    }

    private void changeImage(Drawable image)
    {
        this.isOriginalImageFlag = image.equals(originImage);

        this.currentImage = image;
        this.setBackground(image);
    }

    public void changeImage()
    {
        Handler handler = new Handler(Looper.getMainLooper())
        {
            @Override
            public void handleMessage(Message msg) {
                changeImage(changedImage);
            }
        };
        handler.sendEmptyMessage(0);

    }

    public void originalImage()
    {
        Handler handler = new Handler(Looper.getMainLooper())
        {
            @Override
            public void handleMessage(Message msg) {
                changeImage(originImage);
            }
        };
        handler.sendEmptyMessage(0);

    }

    public Drawable getCurrentImage()
    {
        return this.currentImage;
    }

    public boolean isOriginalImage()
    {
        return this.isOriginalImageFlag;
    }
}
