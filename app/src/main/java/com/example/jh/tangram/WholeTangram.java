package com.example.jh.tangram;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.widget.ImageView;

/**
 * Created by lmasi on 2017. 3. 15..
 */

public class WholeTangram extends ImageView {

    Drawable originImage;
    Drawable changedImage;

    public WholeTangram(Context context, int originImage, int changedImage) {
        super(context);

        this.originImage = getResources().getDrawable(originImage);
        this.changedImage = getResources().getDrawable(changedImage);

        this.setBackground(this.originImage);
    }

    public void changeImage()
    {
        this.setBackground(this.changedImage);
    }

    public void toOriginalImage()
    {
        this.setBackground(this.originImage);
    }
}
