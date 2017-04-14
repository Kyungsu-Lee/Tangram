#include "Tangram2.h"

extern "C" {
/*	Function: Recognize tangram block
	Input:	camera frame - size: VGA(480x640), state: upside down image
			ANS structure : goal
	output: vector<int>present found block index
			complete: set when satisfy answer	*/
//void TANGRAM(Mat &frame, ANS &answer, vector<int> &foundIdx, int &complete, int& init, Mat& lambda, Mat& pre)
void TANGRAM(Mat &frame, ANS &answer, vector<int> &foundIdx, int &complete) {
    static int init = 0;
    static Mat pre = frame.clone();
    static Mat lambda=Mat::zeros(frame.size(),frame.type());
    char msg[30]={'\0'};

    BLOCK blocks[7];
    int progEn = go;

    if (init++ == 0) {
        //readAnswer();
        getLamda(frame, lambda);
        //initialize(frame, lambda);
        //sprintf(msg, "/sdcard/Documents/init%d.jpg", init);
        //imwrite(msg, frame);
    }
    else {
        //sprintf(msg, "/sdcard/Documents/init%d.jpg", init);
        //imwrite(msg, frame);
        foundIdx.clear();
        complete = 0;

        Mat cutFrame;
        cutUnnecessaryPart(frame, cutFrame);

        progEn = isDiff(pre, cutFrame);
        pre = cutFrame.clone();
        if (progEn != go) {
            putText(frame, "NONE", Point(30, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255));
            return;
        }

        /* warping frame with lamda founded during initialization */
        Mat warpFrame;
        warpPerspective(cutFrame, warpFrame, lambda, cutFrame.size());
        //flip(warp, warp, 0);
        //imshow("warp", warpFrame);
        //imwrite("/sdcard/Documents/warp.jpg", warpFrame);

        vector<vector<Point>> edgeResult;
        progEn=findBlockbyEdge(warpFrame, edgeResult);
        if (progEn != go){
            putText(frame, "NONE", Point(30, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255));
            return;
        }

        getBlock(warpFrame, edgeResult, blocks);

        checkFound(blocks, answer, foundIdx);
        if (foundIdx.empty()) {
            putText(frame, "NONE", Point(30, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255));
            return;
        }

        /*for (int t = 0; t < foundIdx.size(); t++) {
            char msg[30] = {'\0'};
            Mat tmp=Mat::zeros(frame.size(), frame.type());
            //sprintf(msg, "%d", blocks[foundIdx[t]].color);
            //putText(frame, msg, blocks[foundIdx[t]].cg, FONT_HERSHEY_SIMPLEX, 1,Scalar(255, 255, 255));
            circle(tmp, blocks[foundIdx[t]].cg, 3, Scalar(255,255,255));
            sprintf(msg, "/sdcard/Documents/blk%d.jpg", blocks[foundIdx[t]].color);
            imwrite(msg, tmp);
        }*/
        if (foundIdx.size() == 7) {
            complete = 1;
        }
        return;

    }
}

/* initialize program variables: answer-matching, lambda-warpPerspectivetransform, pre-get diffrence with present input frame*/
/*void initialize(Mat& src, Mat& lambda)
{
    //readAnswer();
    getLamda(src, lambda);
}*/

/* get lambda matrix to perform warpPerspective transform */
void getLamda(Mat &src, Mat &lambda) {
    Point2f play_point0(WARP_POINT0_X, WARP_POINT0_Y);
    Point2f play_point1(WARP_POINT1_X, WARP_POINT1_Y);
    Point2f play_point2(WARP_POINT2_X, WARP_POINT2_Y);
    Point2f play_point3(WARP_POINT3_X, WARP_POINT3_Y);

    float val_y0 = WARP_POINT0_Y + (WARP_POINT2_X - WARP_POINT3_X) * 1.2;
    float val_y1 = WARP_POINT1_Y + (WARP_POINT2_X - WARP_POINT3_X) * 1.2;

    // Input Quadilateral or Image plane coordinates
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    Point2f inputQuad[4];
    inputQuad[0] = play_point0;
    inputQuad[1] = play_point1;
    inputQuad[2] = play_point2;
    inputQuad[3] = play_point3;

    // Output Quadilateral or World plane coordinates
    // The 4 points that select quadilateral on the input , from top-left in clockwise order
    Point2f outputQuad[4];
    outputQuad[0] = Point(WARP_POINT3_X, WARP_POINT0_Y);
    outputQuad[1] = Point(WARP_POINT2_X, WARP_POINT1_Y);
    outputQuad[2] = Point(WARP_POINT2_X, val_y1);
    outputQuad[3] = Point(WARP_POINT3_X, val_y0);

    lambda = Mat::zeros(src.cols, src.rows, src.type());

    // Get the Perspective Transform Matrix i.e. lambda
    lambda = getPerspectiveTransform(inputQuad, outputQuad);

    //putText(src, "lambdaOK", Point(100, 300), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255) ,3);
    //imwrite("/sdcard/Documents/lambda.jpg", src);
}

/* read answer file and store in answer variables*/
void readAnswer(vector<ANS>& answer) {
    FILE *fp;
    fp = fopen("/sdcard/Documents/answer.txt", "r");
    if (fp == NULL)
        return;
    char ch;
    char name[30];

    while (1) {
        ANS ans;
        //char name[30] = {'\0'};

        fscanf(fp, "%s", ans.name);
        if (feof(fp))
            break;

        for (int i = 0; i < 21; i++) {
            fscanf(fp, "%d", &ans.dist[i]);
        }
        for (int i = 0; i < 7; i++) {
            fscanf(fp, "%d", &ans.angle[i]);
        }
        ch = fgetc(fp);
        if (strlen(ans.name) > 1)
            answer.push_back(ans);
        ch = fgetc(fp);
    }

    fclose(fp);
}

/*  cut unnecessary part caused by camera mirror
	cut image and fill cutted part of image to make size same
	input: Mat source image
	output: Mat cutted image whose size is same with input*/
void cutUnnecessaryPart(Mat &src, Mat &dst) {
    Mat cut;
    Rect cutRect(0, TOP_POINT_Y, src.cols, BOTTOM_POINT_Y);
    cut = src(cutRect).clone();
    //imshow("cut", cut);
    //waitKey(0);
    //dst = Mat::zeros(Size(src.cols, src.rows), src.type());
    copyMakeBorder(cut, dst, TOP_POINT_Y, src.rows-BOTTOM_POINT_Y-TOP_POINT_Y , 0, 0, BORDER_CONSTANT, Scalar(0, 0, 0));
}

/*	To make process efficient, stopping process when present frame is similar with previous
	input: previous frame, present frame
	output: integer value (stop=0, go=1)	*/
int isDiff(Mat &prev, Mat &frame) {
    Mat gray, preGray, diff;
    cvtColor(frame, gray, CV_BGR2GRAY);
    cvtColor(prev, preGray, CV_BGR2GRAY);

    absdiff(gray, preGray, diff);

    threshold(diff, diff, 25, 255, THRESH_BINARY);
    Scalar total = sum(diff);
    if (total[0] > 0.2 * (diff.cols * diff.rows)) {
        return stop;
    }
    else
        return go;
}

int checkPoly(vector<Point>& poly)
{
    vector<float> corner;

    ////chech present contour
    //Mat z = Mat::zeros(Size(FRAME_WIDTH, FRAME_HEIGHT), CV_8UC3);
    //vector<vector<Point>> cont;
    //cont.push_back(poly);
    //drawContours(z, cont, 0, Scalar(255, 255, 255));
    //imshow("poly", z);
    //waitKey(1);


    for (int i = 0; i < poly.size(); i++){
        corner.push_back(getDistance(poly[i], poly[(i + 1) % poly.size()]));
    }
    sort(corner.begin(), corner.end());
    if (corner[0] < 20)	//가장 짧은 변의 길이가 30픽셀 이상이어야 한다.
        return stop;

    vector<float>cornerRate;
    for (int i = 0; i < corner.size(); i++){
        cornerRate.push_back(corner[i] / corner[0]);
    }

    float margin = 0.56;
    if (corner.size() == 3){	//직각삼각형 - 1:1:1.4
        if (((cornerRate[0]>=(1 - margin)) && (cornerRate[0]<=(1 +margin))) &&
            ((cornerRate[1]>=(1 - margin)) && (cornerRate[1]<=(1 + margin))) &&
            ((cornerRate[2]>=(1.4 - margin)) && (cornerRate[2]<=(1.4 + margin))))
            return go;
        else return stop;
    }
    else if (corner.size() == 4){
        if (((cornerRate[0] >= (1 - margin)) && (cornerRate[0] <= (1 + margin))) &&
            ((cornerRate[1] >= (1 - margin)) && (cornerRate[1] <= (1 + margin))) &&
            ((cornerRate[2] >= (1 - margin)) && (cornerRate[2] <= (1 + margin))) &&
            ((cornerRate[3] >= (1 - margin)) && (cornerRate[3] <= (1 + margin))))//정사각형 - 1:1:1:1
            return go;

        else if (((cornerRate[0] >= (1 - margin)) && (cornerRate[0] <= (1 + margin))) &&
                 ((cornerRate[1] >= (1 - margin)) && (cornerRate[1] <= (1 + margin))) &&
                 ((cornerRate[2] >= (1.4 - margin)) && (cornerRate[2] <= (1.4 + margin))) &&
                 ((cornerRate[3] >= (1.4 - margin)) && (cornerRate[3] <= (1.4 + margin))))		//평행사변 - 1:1:1.4:1.4
            return go;

        else return stop;
    }
    else return stop;
}

/* find block by only edge- 1. 각 변의 비율, 2. 긴 변의 길이*/
int findBlockbyEdge(Mat& src, vector<vector<Point>>& result)
{
    int en = go;

    Mat blured, edge;
    blur(src, blured, Size(3, 3));
    Canny(src, edge, 30, 60, 3);
    dilate(edge, edge, Mat(), Point(-1, -1), 2);

    vector<vector<Point>> contours;
    findContours(edge, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    if (contours.size() < 1)
        return stop;

    Mat cont = src.clone();
    char msg[50]={'\0'};

    //get first block contours according to corner rate
    for (int i = 0; i < contours.size(); i++){
        vector<Point> poly;
        approxPolyDP(contours[i], poly, 12, true);
        if (poly.size() != 3 && poly.size() != 4)
            continue;

        drawContours(cont, contours, i, Scalar(255, 255, 255), 1);
        en=checkPoly(poly);
        if (en == go){
            drawContours(cont, contours, i, Scalar(0, 255, 255), 2);
            result.push_back(poly);
            sprintf(msg, "/sdcard/Documents/contour%d.jpg", result.size());
            //imwrite(msg, cont);
        }
        else
            continue;
    }

    if (result.size() == 0)
        return stop;

    return go;
}


double getDistance(const Point &p1, const Point &p2) {

    double distance;

    // 피타고라스의 정리
    // pow(x,2) x의 2승,  sqrt() 제곱근
    distance = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));

    return distance;
}

/* get center of gravity of block*/
int getBlockCenter(BLOCK &blk) {
    if (blk.vertex.empty())
        return stop;

    int cg_x = 0;
    int cg_y = 0;
    for (int i = 0; i < blk.vertex.size(); i++) {
        cg_x += blk.vertex[i].x;
        cg_y += blk.vertex[i].y;
    }
    cg_x = cg_x / blk.vertex.size();
    cg_y = cg_y / blk.vertex.size();

    if (cg_x == 0 || cg_y == 0 || cg_x > FRAME_WIDTH || cg_y > FRAME_HEIGHT)
        return stop;

    blk.cg.x = cg_x;
    blk.cg.y = cg_y;
    return go;
}

/* get angle of longest line of block*/
int getBlockAngle(BLOCK& blk)
{
    if (blk.vertex.empty() || blk.cg == Point(0, 0))
        return stop;

    //블럭이 삼각형인 경우 외접원과 중심좌표의 각도를 블럭의 각도로 사용한다.
    if (blk.vertex.size() == 3){
        Point2f center(0, 0);
        float radius;

        minEnclosingCircle(blk.vertex, center, radius);
        blk.angle = cvFastArctan((blk.cg.y - center.y), (blk.cg.x - center.x));
    }
    else{	//블럭이 사각형인 경우 가장 긴 변(대각선)이 x축과 이루는 각도를 사용한다.
        float maxDis = 0;
        vector<Point2i> maxSide;
        int maxIdx0 = -1;
        int maxIdx1 = -1;

        //find longest line
        for (int i = 0; i < blk.vertex.size(); i++) {
            for (int j = i + 1; j < blk.vertex.size(); j++) {
                float dis = getDistance(blk.vertex[i], blk.vertex[j]);
                if (dis > maxDis) {
                    maxDis = dis;
                    maxIdx0 = i;
                    maxIdx1 = j;
                }
            }
        }

        if (maxIdx0 != -1 && maxIdx1 != -1) {
            maxSide.push_back(blk.vertex[maxIdx0]);
            maxSide.push_back(blk.vertex[maxIdx1]);
        }
        else
            return stop;
        //정사각형(노란색)인 경우
        if (blk.color == yellow) {
            int ang = (int)cvFastArctan((maxSide[1].y - maxSide[0].y),
                                        (maxSide[1].x - maxSide[0].x));
            blk.angle = ang % 90;
        }
        else if (blk.color == orange) { //평행사변형인 경우
            int idx = -1;
            for (int j = 0; j < blk.vertex.size(); j++)
                if (maxSide[0] == blk.vertex[j]) {
                    idx = j;
                }
            //평행사변형에서 가장 긴 대각선을 이루는 두꼭지점중 한점을 기준으로, 다음꼭지점과 이전꼭지점의 길이로 flip을 확인한다.
            //이때 꼭지점은 도형에서 반시계방향으로 순서가 정해진다고 가정한다.
            int dis1 = (int)getDistance(blk.vertex[idx], blk.vertex[(idx + 1) % 4]);
            int dis2 = (int)getDistance(blk.vertex[(idx + 1) % 4], blk.vertex[(idx+2)%4]);

            blk.angle = (int)cvFastArctan((maxSide[1].y - maxSide[0].y), (maxSide[1].x - maxSide[0].x));

            if (dis1 < dis2)
                blk.angle = blk.angle;
            else
                blk.angle = -1 * blk.angle;
        }
        else
            return stop;
    }

    return go;
}


/*	decide block color by hue value of center of block in source imgae
input: Mat source image, BLOCK having cg information
output: defined blk color	*/
int decideBlockColor(Mat& src, BLOCK& blk)
{
    Mat blured;
    blur(src, blured, Size(3, 3));

    /*//chech present contour
    Mat z = Mat::zeros(Size(FRAME_WIDTH, FRAME_HEIGHT), CV_8UC3);
    vector<vector<Point>> cont;
    cont.push_back(blk.vertex);
    drawContours(z, cont, 0, Scalar(255, 255, 255));*/

    int h = blured.at<uchar>(blk.cg);

    if (blk.vertex.size() == 4){

        //한모서리와 다른 모서리의 각도를 구하여, 각도가 90도이면 정사각형, 45보다 작으면 평행사변형이라한다.
        int ang1 = ((int)cvFastArctan((blk.vertex[1].y-blk.vertex[0].y), (blk.vertex[1].x-blk.vertex[0].x)));
        int ang2 = ((int)cvFastArctan((blk.vertex[2].y - blk.vertex[1].y), (blk.vertex[2].x - blk.vertex[1].x)));
        int ang = abs(ang2 - ang1)%90;
        if ((ang >= 70 || ang<=15) && h<30)
            blk.color = yellow;
        else if ((ang <= 65 && ang >= 35) && h < 50)
            blk.color = orange;
        else
            return stop;
    }
    else if (blk.vertex.size() == 3){
        //get largest line
        //int maxLineIdx = -1;
        int maxLength = 0;
        for (int i = 0; i < blk.vertex.size(); i++){
            int length = (int)getDistance(blk.vertex[i], blk.vertex[(i + 1) % blk.vertex.size()]);
            if (maxLength < length){
                //maxLineIdx = i;
                maxLength = length;
            }
        }

        int color=-1;
        if (maxLength > 0 && maxLength < 80 && color==-1){
            if (h < 100 && h>50) color = sky;
            else if (h >= 110) color = purple;
        }
        if (maxLength > 100 && color==-1){
            if (h < 30) color = red;
            else if (h > 50) color = blue;
            //else return stop;
        }
        if (maxLength > 50 && color==-1){
            if (h > 20 && h < 100) color = green;
            //else return stop;
        }
        if (color != -1)
            blk.color = color;
        else
            return stop;
    }
    else return stop;

    return go;
}

/*	get one block from source imag
input: Mat image
output: Block values store to block array	*/
void getBlock(Mat& src, vector<vector<Point>>&edgeResult, BLOCK block[7])
{
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);
    vector<Mat> hsv_ch;
    split(hsv, hsv_ch);

    for (int j = 0; j < edgeResult.size(); j++){
        BLOCK blk;
        int en = go;

        blk.vertex = edgeResult[j];

        //get block parameter: center of gravity and angle using block vertexes
        en = getBlockCenter(blk);
        if (en == stop)
            continue;
        en = decideBlockColor(hsv_ch[0], blk);
        if (en == stop)
            continue;
        en = getBlockAngle(blk);
        if (en == stop)
            continue;

        //find index to store blk to block array
        int blkIdx = -1;
        for (int i = 0; i < 7; i++){
            if (i == blk.color){
                blkIdx = blk.color;
                break;
            }
        }
        if (blkIdx == -1)
            continue;

        //copy all information of block
        block[blkIdx].vertex = blk.vertex;
        block[blkIdx].cg = blk.cg;
        block[blkIdx].color = blk.color;
        block[blkIdx].angle = blk.angle;
        block[blkIdx].found = 1;	//flag
    }
    return;
}


/* 모든 블럭의 거리 계산. 찾아진 블럭이 아니면 거리를 0, 정답 거리와 비교하여 일치하는 경우 인덱스 저장 */
void matchBlockDistance(BLOCK block[7], ANS &answer, vector<int> &foundIdx) {
    vector<int> tempIdx;

    int margin = 30;
    int t = 0;    //answer dist index
    //모든 블럭의 거리 계산 찾아진 블럭이 아니면 거리를 0, 이를 정답 거리와 비교하여 일치하는 경우 인덱스 저장
    for (int i = 0; i < 7; i++) {
        for (int j = i + 1; j < 7; j++) {
            int dist = 0;
            if (block[i].found != 1 || block[j].found != 1)
                dist = 0;
            else {
                dist = (int) getDistance(block[i].cg, block[j].cg);
            }
            if ((dist > answer.dist[t] - margin) && (dist < answer.dist[t] + margin)) {
                //	int exist = 0;
                //	for (int k = 0; k < tempIdx.size(); k++){
                //		if (tempIdx[k] == i || tempIdx[k] == j)
                //			exist = 1;
                //	}
                //	if (!exist){
                tempIdx.push_back(i);
                tempIdx.push_back(j);
                //	}
                //}
            }
            t++;
        }
    }

    if (tempIdx.empty())
        return;

    //저장된 인덱스를 sorting, 중복되지 않는 인덱스만 저장한다.
    sort(tempIdx.begin(), tempIdx.end());
    int storeIdx = -1;
    for (int i = 0; i < tempIdx.size(); i++) {
        if (storeIdx != tempIdx[i]) {
            foundIdx.push_back(tempIdx[i]);
            storeIdx = tempIdx[i];
        }
    }
    return;
}

int matchBlockAngle(BLOCK block[7], ANS &answer, int &idx) {
    int angmargin = 30;
    int match = 0;

    int angle = block[idx].angle;

    if (block[idx].color == orange) {
        if ((angle > answer.angle[idx] - angmargin && angle < answer.angle[idx] + angmargin) ||
            (angle + 360 > answer.angle[idx] - angmargin &&
             angle + 360 < answer.angle[idx] + angmargin) ||
            (-angle + 360 > answer.angle[idx] - angmargin &&
             -angle + 360 < answer.angle[idx] + angmargin)) {
            match = 1;
        }
    }
    else if (block[idx].color == yellow) {
        if (((angle > answer.angle[idx] - angmargin) && (angle < answer.angle[idx] + angmargin)) ||
            ((angle + 90 > answer.angle[idx] - angmargin) &&
             (angle + 90 < answer.angle[idx] + angmargin)) ||
            ((angle - 90 > answer.angle[idx] - angmargin) &&
             (angle - 90 < answer.angle[idx] + angmargin))) {
            match = 1;
        }
    }
    else {
        if ((angle > answer.angle[idx] - angmargin && angle < answer.angle[idx] + angmargin) ||
            (angle + 360 > answer.angle[idx] - angmargin &&
             angle + 360 < answer.angle[idx] + angmargin) ||
            (-angle + 360 > answer.angle[idx] - angmargin &&
             -angle + 360 < answer.angle[idx] + angmargin)) {
            match = 1;
        }
    }
    return match;
}

/* Check what blocks are founded
input: BLOCK array
output: founded block index vector*/
void checkFound(BLOCK blocks[7], ANS& answer, vector<int>& foundIdx)
{
    //현재 찾은 블럭이 무엇인지 확인
    int numFoundBlock = 0;
    int blkIdx = -1; //어떤 블럭을 찾았는지 저장. 두개 이상이면 필요없는 변수
    for (int i = 0; i < 7; i++){
        if (blocks[i].found == 1){
            blkIdx = i;
            numFoundBlock++;
        }
    }
    //찾은 블럭이 없거나 하나만 찾은 경우 끝냄
    if (numFoundBlock <2) //수정 1에서 2로
        return;

    //찾은 블럭의 거리를 계산하고 정답과 일치하는 블럭의 인덱스를 저장한다.
    vector<int> preFoundIdx;
    matchBlockDistance(blocks, answer, preFoundIdx);

    //찾아진 블럭의 각도 체크 regarding to answer
    for (int i = 0; i < preFoundIdx.size(); i++){
        int match = matchBlockAngle(blocks, answer, preFoundIdx[i]);
        if (match == 1){
            foundIdx.push_back(preFoundIdx[i]);
        }
    }
}
}