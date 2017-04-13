#include "Tangram2.h"

using namespace cv;

extern "C" {

static vector<int> foundIdx;   //현재 프레임에서 찾은 블럭의 인덱스 (header파일의 BLOCK_COLOR 참고)

JNIEXPORT jint JNICALL
Java_com_example_jh_tangram_MainActivity_convertNativeLib(JNIEnv*, jobject, jlong addrInput, jlong addrResult, jint idx) {

    Mat &img_input = *(Mat *) addrInput;
    Mat &img_result = *(Mat *) addrResult;

    Mat frame_org; //original frame
    cvtColor(img_input, frame_org, CV_RGBA2BGR);
    resize(frame_org, frame_org, Size(640,480));
    transpose(frame_org, frame_org);
    flip(frame_org, frame_org, -1);

   //
    // imwrite("/sdcard/Documents/frame.jpg", frame_org);

    static int complete = 0;    //블럭이 모두 맞춰지면 1. (현재 그림 인식 종료 가능)
    static int init=0;          //초기화 한번 진행

    int ansIdx=idx;        //맞춰야 하는 블럭 index. (개:0, 고양이:1, 새:2)
    static vector<ANS> answer;   //answer.txt 파일에서 정답 정보를 읽어와 저장하는 벡터


    //static Mat pre = frame_org.clone();     //화면 변화 체크
    //static Mat lambda=Mat::zeros(frame_org.size(),frame_org.type());    //화면 warping 파라미터

    if(init==0) {
        readAnswer(answer);
        init++;
    }
    else {
         TANGRAM(frame_org, answer[ansIdx], foundIdx, complete);
         //TANGRAM(frame_org, answer[ansIdx], foundIdx, complete, init, lambda, pre);
    }

/*
    if(foundIdx.size()!=0){
        char msg[30]={'\0'};
        for(int i=0; i<foundIdx.size(); i++){
            sprintf(msg, "%d", foundIdx[i]);
            putText(img_input, msg, Point(30+i*100, 100), FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 255, 255) ,3);
        }
    }
    if(complete==1){
        putText(img_input, answer[ansIdx].name, Point(600, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 255, 255) ,3);
    }
    */

    return 0;
}

JNIEXPORT jintArray JNICALL Java_com_example_jh_tangram_MainActivity_getResult(JNIEnv *env, jobject jobj){

    jintArray result;
    result = (env)->NewIntArray(foundIdx.size());
    if (result == NULL) {
        return NULL; /* out of memory error thrown */
    }
    int i;
    // fill a temp structure to use to populate the java int array
    jint fill[256];
    for (i = 0; i < foundIdx.size(); i++) {
        fill[i] = foundIdx[i]; // put whatever logic you want to populate the values here.
    }
    // move from the temp structure to the java structure
    (env)->SetIntArrayRegion( result, 0, foundIdx.size(), fill);

    return result;

}

}
