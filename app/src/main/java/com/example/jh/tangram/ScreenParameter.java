package com.example.jh.tangram;

/**
 * Created by lmasi on 2016-08-10.
 */
public class ScreenParameter {

    //default screen size
    private static double DEFAULTSIZE_Y = 2028;
    private static double DEFAULTSIZE_X = 1536;

    //screenparameter to be used
    private static double  screenparam_x = 1;
    private static double  screenparam_y = 1;
    private static double  screen_x = 2560;
    private static double  screen_y = 1440;



    public static double getDefaultsizeX() {
        return DEFAULTSIZE_X;
    }

    public static double getDefaultsizeY() {
        return DEFAULTSIZE_Y;
    }

    public static double getScreen_x() {
        return screen_x;
    }

    public static double getScreen_y() {
        return screen_y;
    }

    public static double getScreenparam_x() {
        return screenparam_x;
    }

    public static double getScreenparam_y() {
        return screenparam_y;
    }


    public static void setScreen_x(double screen_x) {
        ScreenParameter.screen_x = screen_x;
    }

    public static void setScreen_y(double screen_y) {
        ScreenParameter.screen_y = screen_y;
    }

    public static void setScreenparam_x(double screenparam_x) {
        ScreenParameter.screenparam_x = screenparam_x;
    }

    public static void setScreenparam_y(double screenparam_y) {
        ScreenParameter.screenparam_y = screenparam_y;
    }


}
