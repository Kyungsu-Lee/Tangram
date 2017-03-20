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
        //imshow("present frame", frame);

        Mat cutFrame;
        cutUnnecessaryPart(frame, cutFrame);
        //imshow("cutframe", cutFrame);

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
        imwrite("/sdcard/Documents/warp.jpg", warpFrame);

        vector<Mat> coloredBlk;
        progEn = findBlockbyColor(warpFrame, coloredBlk);
        if (progEn != go) {
            putText(frame, "NONE", Point(30, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255));
            return;
        }

        for (int i = 0; i < coloredBlk.size(); i++) {
            getBlock(coloredBlk[i], blocks);

        }
       /* if (coloredBlk.size() != 0)
            for (int i = 0; i < coloredBlk.size(); i++) {
                char msg[50];
                sprintf(msg, "/sdcard/Documents/blk%d.jpg", i);
                imwrite(msg, coloredBlk[i]);
            }*/

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
   // fp = fopen("/data/user/0/com.example.jh.tangram/files/answer.txt", "r");
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
    if (total[0] > 0.15 * (diff.cols * diff.rows)) {
        return stop;
    }
    else
        return go;
}

/*	find each block by watershed algorithm
	input: source image
	output: vector<Mat> found image vector*/
int findBlockbyColor(Mat &src, vector<Mat> &result) {

    Mat blured, edge;
    blur(src, blured, Size(3, 3));
    Canny(blured, edge, 30, 60, 3);
    dilate(edge, edge, Mat(), Point(-1, -1), 2);
    //erode(edge, edge, Mat(), Point(-1, -1), 1);
    //dilate(edge, edge, Mat(), Point(-1, -1), 2);
    //imwrite("/sdcard/Documents/colorCanny.jpg", edge);

    //make markers
    vector<vector<Point> > contours;
    findContours(edge, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    if (contours.size() <= 0)
        return stop;
    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(edge.size(), CV_32SC1);
    // Draw the foreground markers
    int num = 0;
    for (size_t i = 0; i < contours.size(); i++) {
        Rect r;
        r = boundingRect(contours[i]);
        if (r.area() > 50 && r.area() < edge.cols / 2 * edge.rows / 2) {
            num++;
            drawContours(markers, contours, static_cast<int>(i), Scalar::all(num), -1);
        }
    }
    if (num == 0) // || num > 10
        return stop;
    //imshow("conto mark", markers*10000);
    //waitKey(0);

    Mat mark = markers.clone();
    mark.convertTo(mark, CV_8UC1);

    // Draw the background marker
    //circle(markers, Point(1, 1), 3, Scalar::all(255),  -1);
    //circle(markers, Point(1, markers.rows - 1), 3, Scalar::all(255), -1);
    //circle(markers, Point(markers.cols - 1, 1), 3, Scalar::all(255), -1);
    //circle(markers, Point(markers.cols - 1, markers.rows - 1), 3, Scalar::all(255), -1);

    watershed(src, markers);
    //Generate result image
    vector<Vec3b> colors;
    for (int i = 0; i < num; i++) {
        colors.push_back(pickColor(src, mark, i + 1));
        if ((colors[i][0] + colors[i][1] + colors[i][2]) / 3 < 10)
            colors[i] = Vec3b(0, 0, 0);
    }

    // Create the result image
    // Fill labeled objects stored colors
    for (int k = 0; k < num; k++) {
        Mat dst = Mat::zeros(markers.size(), CV_8UC3);
        for (int i = 0; i < markers.rows; i++) {
            for (int j = 0; j < markers.cols; j++) {
                if (markers.at<int>(i, j) == k + 1)
                    dst.at<Vec3b>(i, j) = colors[k];
            }
        }
        result.push_back(dst);
    }
    if (result.size() <= 0) {
        return stop;
    }
    return go;
}

/*  pick color from original color image regarding to value of data matrix
	input:  color and data matrix(these should be same size)
			integer value of data matrix which indicate wanted position
	output: bgr color value as Vec3b type */
Vec3b pickColor(Mat &src_color, Mat &src_data, int value) {
    vector<Mat> ch(3);
    split(src_color, ch);

    int x = 0;
    int y = 0;
    for (int i = 0; i < src_data.cols; i++) {
        int j = 0;
        for (j = 0; j < src_data.rows; j++) {
            if (src_data.at<uchar>(j, i) == value) {
                x = j;
                y = i;
                break;
            }
        }
        if (j != src_data.rows)
            break;
    }

    int b = ch[0].at<uchar>(x, y);
    int g = ch[1].at<uchar>(x, y);
    int r = ch[2].at<uchar>(x, y);

    return (Vec3b((uchar) b, (uchar) g, (uchar) r));
}

int getPolygon(Mat &src, vector<Point> &vertex) {
    Mat canny;
    Canny(src, canny, 35, 60);
    if (canny.total()==0)
        return stop;
    //imshow("canny", canny);
    dilate(canny, canny, Mat());

    vector<vector<Point>> contours;
    vector<Point> poly;
    findContours(canny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    if (contours.size() <= 0)
        return stop;
    for (size_t i = 0; i < contours.size(); i++) {
        if (contourArea(contours[i]) <= 0 || contourArea(contours[i]) > src.cols / 2 * src.rows / 2)
            return stop;
        approxPolyDP(contours[i], poly, 15, true);
        if (poly.size() != 3 && poly.size() != 4)
            return stop;
    }

    vertex = poly;
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
int getBlockAngle(BLOCK &blk) {
    if (blk.vertex.empty() || blk.cg == Point(0, 0))
        return stop;

    //블럭이 삼각형인 경우 외접원과 중심좌표의 각도를 블럭의 각도로 사용한다.
    if (blk.vertex.size() == 3) {
        Point2f center(0, 0);
        float radius;

        minEnclosingCircle(blk.vertex, center, radius);
        blk.angle = cvFastArctan((blk.cg.y - center.y), (blk.cg.x - center.x));
    }
    else {    //블럭이 사각형인 경우 가장 긴 변(대각선)이 x축과 이루는 각도를 사용한다.
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
            int ang = (int) cvFastArctan((maxSide[1].y - maxSide[0].y),
                                         (maxSide[1].x - maxSide[0].x));
            blk.angle = ang % 90;
        }
        else if (blk.color == 1) { //평행사변형인 경우
            int idx = -1;
            for (int j = 0; j < blk.vertex.size(); j++)
                if (maxSide[0] == blk.vertex[j]) {
                    idx = j;
                }
            //평행사변형에서 가장 긴 대각선을 이루는 두꼭지점중 한점을 기준으로, 다음꼭지점과 이전꼭지점의 길이로 flip을 확인한다.
            //이때 꼭지점은 도형에서 반시계방향으로 순서가 정해진다고 가정한다.
            int dis1 = (int) getDistance(blk.vertex[idx], blk.vertex[(idx + 1) % 4]);
            int dis2 = (int) getDistance(blk.vertex[idx], blk.vertex[(idx + 3) % 4]);

            blk.angle = (int) cvFastArctan((maxSide[1].y - maxSide[0].y),
                                           (maxSide[1].x - maxSide[0].x));

            if (dis1 > dis2)
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
int decideBlockColor(Mat &src, BLOCK &blk) {
    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    vector<Mat> hsv_ch;
    split(hsv, hsv_ch);

    int h = hsv_ch[0].at<uchar>(blk.cg);
    //int s = hsv_ch[1].at<uchar>(blk.cg);

    if (blk.vertex.size() == 4){
        int dist = abs((int)(getDistance(blk.vertex[0], blk.vertex[1]) - getDistance(blk.vertex[1], blk.vertex[2])));
        if (dist < 15 && h < 50 && h>5)
            blk.color = yellow;
        else if (dist >= 15 && h < 30 && h>5)
            blk.color = orange;
        else
            return stop;
    }
    else if (blk.vertex.size() == 3){
        //get largest line
        int maxLineIdx = -1;
        int maxLength = 0;
        for (int i = 0; i < blk.vertex.size(); i++){
            int length = getDistance(blk.vertex[i], blk.vertex[(i + 1) % blk.vertex.size()]);
            if (maxLength < length){
                maxLineIdx = i;
                maxLength = length;
            }
        }

        if (maxLength > 0 && maxLength < 95){
            if (h < 100 && h>50) blk.color = sky;
            else if (h >= 110) blk.color = purple;
            else return stop;
        }
        else if (maxLength >= 120 && maxLength< 180){
            if (h < 20 || h>170) blk.color = red;
            else if (h > 45) blk.color = blue;
            else return stop;
        }
        else if (maxLength > 95 && maxLength<120){
            if (h > 20 && h < 100) blk.color = green;
            else return stop;
        }
        else
            return stop;
    }
    else return stop;

    char msg[50];
    Mat t=src.clone();
    sprintf(msg, "/sdcard/Documents/block%d.jpg", blk.color);
    imwrite(msg, t);

    return go;
}

/*	get one block from source imag
	input: Mat image
	output: Block values store to block array	*/
void getBlock(Mat &src, BLOCK block[7]) {
    BLOCK blk;

    int en = go;
    vector<Point> vtx;
    en = getPolygon(src, vtx);
    if (en == stop)
        return;
    else
        blk.vertex = vtx; //block is found

    //get block parameter: center of gravity and angle using block vertexes
    en = getBlockCenter(blk);
    if (en == stop)
        return;
    en = decideBlockColor(src, blk);
    if (en == stop)
        return;
    en = getBlockAngle(blk);
    if (en == stop)
        return;

    //find index to store blk to block array
    int blkIdx = -1;
    for (int i = 0; i < 7; i++) {
        if (i == blk.color) {
            blkIdx = blk.color;
            break;
        }
    }
    if (blkIdx == -1)
        return;

    //copy all information of block
    block[blkIdx].vertex = blk.vertex;
    block[blkIdx].cg = blk.cg;
    block[blkIdx].color = blk.color;
    block[blkIdx].angle = blk.angle;
    block[blkIdx].found = 1;    //flag to check block is found

    return;
}

/* 모든 블럭의 거리 계산. 찾아진 블럭이 아니면 거리를 0, 정답 거리와 비교하여 일치하는 경우 인덱스 저장 */
void matchBlockDistance(BLOCK block[7], ANS &answer, vector<int> &foundIdx) {
    vector<int> tempIdx;

    int margin = 20;
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
        //if (tempIdx[i] != tempIdx[i + 1]){
        //	foundIdx.push_back(tempIdx[i]);
        //	if (i == tempIdx.size() - 2)
        //		foundIdx.push_back(tempIdx[i + 1]);
        //}
    }
    return;
}

int matchBlockAngle(BLOCK block[7], ANS &answer, int &idx) {
    int angmargin = 15;
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
void checkFound(BLOCK block[7], ANS &answer, vector<int> &foundIdx) {
    //현재 찾은 블럭이 무엇인지 확인
    int numFoundBlock = 0;
    vector<int> blkIdx ; //어떤 블럭을 찾았는지 저장. 두개 이상이면 필요없는 변수
    for (int i = 0; i < 7; i++) {
        if (block[i].found == 1) {
            blkIdx.push_back(i);
            numFoundBlock++;
        }
    }
    //찾은 블럭이 없으면 끝냄. 또는 하나만 찾은 경우 해당 블럭만 저장후 끝냄
    if (numFoundBlock == 0)
        return;
    else if (numFoundBlock == 1) {
        foundIdx.push_back(blkIdx[0]);
        //char msg[50];
        //Mat t=Mat::zeros(Size(FRAME_WIDTH, FRAME_HEIGHT), CV_8UC3);
        //sprintf(msg, "/sdcard/Documents/One%d.jpg", blkIdx[0]);
        //imwrite(msg,t);
        return;
    }

    //찾은 블럭의 거리를 계산하고 정답과 일치하는 블럭의 인덱스를 저장한다.
    vector<int> preFoundIdx;
    matchBlockDistance(block, answer, preFoundIdx);

    //블럭이 여러개 있지만 모두 거리가 맞지 않는 경우, 가장 숫자가 작은 블럭 하나의 인덱스만 저장하고 끝낸다.
    if (numFoundBlock != 0 && preFoundIdx.size() == 0) {
        foundIdx.push_back(blkIdx[0]);
        return;
    }

    //찾아진 블럭의 각도 체크 regarding to answer
    for (int i = 0; i < preFoundIdx.size(); i++) {
        int match = matchBlockAngle(block, answer, preFoundIdx[i]);
        if (match == 1) {
            foundIdx.push_back(preFoundIdx[i]);
        }
    }
}
}