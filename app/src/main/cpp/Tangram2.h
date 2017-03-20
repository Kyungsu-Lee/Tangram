//
// Created by JH on 2017-03-15.
//

#ifndef TANGRAM_TANGRAM2_H
#define TANGRAM_TANGRAM2_H

#endif //TANGRAM_TANGRAM2_H

#include <jni.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <cmath>
#include <ctime>
#include <vector>

using namespace cv;
using namespace std;

extern "C" {

#include <iostream>
#include <fstream>
#include <cstdio>
#include <opencv2/opencv.hpp>
#include <string.h>
#include <cmath>
#include <ctime>
#include <vector>

using namespace cv;
using namespace std;

#define FRAME_WIDTH 480
#define FRAME_HEIGHT 640

/* BLOCK COLOR*/
#define red 0
#define orange 1
#define yellow 2
#define green 3
#define sky 4
#define blue 5
#define purple 6

/* WARPING POINT */
#define WARP_POINT0_X 15
#define WARP_POINT0_Y 120
#define WARP_POINT1_X 460
#define WARP_POINT1_Y 120
#define WARP_POINT2_X 370
#define WARP_POINT2_Y 440
#define WARP_POINT3_X 100
#define WARP_POINT3_Y 440
#define TOP_POINT_Y 100
#define BOTTOM_POINT_Y 410

/*decide if process go on or stop*/
#define go 1
#define stop 0


/* contains informations of each block
:vertexs, center of gravity, angle with x-axis, color of block, distances from other block */
typedef struct BLOCK{
    vector<Point> vertex;		//maximum vertexs of blocks = 4
    Point cg = Point(0, 0);	//point of center of gravity
    int angle = 0.0;		//angle with x-axis
    int color = -1;			//identity of block
    int found = 0;
}BLOCK;


/* Answer structure*/
typedef struct ANS{
    char name[30];
    int dist[21];
    int angle[7];
}ANS;

/* variables should be initialized once at first*/
//vector<ANS> answer;

void readAnswer(vector<ANS>& answer);
void TANGRAM(Mat& frame, ANS& answer, vector<int>& foundIdx, int& complete);
//void TANGRAM(Mat& frame, ANS& answer, vector<int>& foundIdx, int& complete, int& init, Mat& lambda, Mat& pre);

//void initialize(Mat& src, Mat& lambda);
void getLamda(Mat& src, Mat& lambda);

void cutUnnecessaryPart(Mat& src, Mat& dst);
int isDiff(Mat& prev, Mat& frame);

int findBlockbyColor(Mat& ROI, vector<Mat> &result);
Vec3b pickColor(Mat &src_color, Mat &src_data, int value);

/* Find Block and its parameter*/
void getBlock(Mat& src, BLOCK block[7]);
int getPolygon(Mat& src, vector<Point>& vertex);
int getBlockCenter(BLOCK& blk);
int getBlockAngle(BLOCK& blk);
double getDistance(const Point &p1, const Point &p2);
int decideBlockColor(Mat& src, BLOCK& blk);

/* Match blocks's information with answer*/
void checkFound(BLOCK block[7], ANS& answer, vector<int>& foundIdx);
void matchBlockDistance(BLOCK block[7], ANS& answer, vector<int>& foundIdx);
int matchBlockAngle(BLOCK block[7], ANS& answer, int& idx);

}