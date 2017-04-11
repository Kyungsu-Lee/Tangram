package com.example.jh.tangram;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.media.MediaPlayer;
import android.net.Uri;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.MediaController;
import android.widget.RelativeLayout;
import android.widget.VideoView;

/**
 * Created by lmasi on 2017. 3. 8..
 */

public class AnimationPlayer extends RelativeLayout {

    private static String packageName;

    private VideoView videoView;
    private MediaController mediaController;

    public AnimationPlayer(Context context) {
        super(context);

        packageName = context.getPackageName();

        videoView = new VideoView(context);
        LayoutParams layoutParams = new LayoutParams(LayoutParams.MATCH_PARENT, (int)ScreenParameter.getScreen_y()/2);
        layoutParams.addRule(CENTER_IN_PARENT);
        videoView.setLayoutParams(layoutParams);
        this.addView(videoView);

        mediaController = new MediaController(context);
        mediaController.setAnchorView(videoView);
        videoView.setMediaController(mediaController);

        //for prohibit to touch video player
        ImageView block = new ImageView(context);
        block.setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));
        block.setOnTouchListener(new OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                return true;
            }
        });
        this.addView(block);
    }

    public void play(Uri uri, MediaPlayer.OnCompletionListener completionListener)
    {
        videoView.setOnCompletionListener(completionListener);
        videoView.setVideoURI(uri);
        videoView.start();
    }

    public void play(String uri, MediaPlayer.OnCompletionListener completionListener)
    {
        videoView.setOnCompletionListener(completionListener);
        videoView.setVideoURI(Uri.parse(uri));
        videoView.start();
    }

    public void startActivityAfterVideo(final Context context, Uri uri, final Class targetClass)
    {
        play(uri, new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                context.startActivity(new Intent(context, targetClass));
            }
        });
    }

    public void startActivityAfterVideo(final Context context, int raw, final Class targetClass)
    {
        play(UriParse(raw), new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                context.startActivity(new Intent(context, targetClass));
                ((Activity)context).finish();
            }
        });
    }

    public void startActivityAfterVideoWithIntent(int raw, final Intent intent, final Context context)
    {
        play(UriParse(raw), new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                ((Activity)context).startActivity(intent);
                ((Activity)context).finish();
                videoView.stopPlayback();
            }
        });
    }

    public String UriParse(int raw)
    {
        return "android.resource://" + packageName + "/"+ raw;
    }


/*  //////////// original methods /////////////


        VideoView videoView = (VideoView) findViewById(R.id.view);
        // 비디오뷰를 커스텀하기 위해서 미디어컨트롤러 객체 생성
        MediaController mediaController = new MediaController(getApplicationContext());
        // 비디오뷰에 연결
        mediaController.setAnchorView(videoView);
        // 안드로이드 res폴더에 raw폴더를 생성 후 재생할 동영상파일을 넣습니다.
        // 경로에 주의할 것
        // 실제 모바일에서 테스트 할 것
        // 위 두가지를 대충 넘겼다가 많은 시간을 허비했다. ㅜㅜ...
        Uri video = Uri.parse("android.resource://" + getPackageName()+ "/"+R.raw.scenes1_all);

        //비디오뷰의 컨트롤러를 미디어컨트롤로러 사용
        videoView.setMediaController(mediaController);

        //callback
        videoView.setOnCompletionListener(new MediaPlayer.OnCompletionListener() {
            @Override
            public void onCompletion(MediaPlayer mp) {
                startActivity(new Intent(AnimationPlayActivity.this, MainActivity.class));
            }
        });

        //비디오뷰에 재생할 동영상주소를 연결
        videoView.setVideoURI(video);
        //비디오뷰를 포커스하도록 지정
        //동영상 재생
        videoView.start();
*/
}
