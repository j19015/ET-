/******************************************************************************
 *  LineTracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "LineTracer.h"
#include "stdio.h"
#include <time.h>
time_t time(time_t *t);
const float DELTA_T=0.0001;
const float KP=0.6;//0.48
const float KI=1.2;
const float KD=0.012;
static float diff[2]={0.0,0.0};
static float integral=0.0;
static time_t cl[2]={0,0};
static int aoto2 = 0;
/*
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param walker 走行
 */
LineTracer::LineTracer(const LineMonitor* lineMonitor,
                       Walker* walker)
    : mLineMonitor(lineMonitor),
      mWalker(walker),
      mIsInitialized(false) {
}

/**
 * ライントレースする
 */

void LineTracer::run_speaed(int spead1,int spead2){
    if (mIsInitialized == false) {
        mWalker->init();
        mIsInitialized = true;
    }

    float isOnLine = mLineMonitor->isOnLine();

    // 走行体の向きを計算する
    float direction = calcDirection(isOnLine);

    mWalker->setCommand(80.0, direction);

    // 走行を行う
    mWalker->run_speaed(spead1,spead2);
}
float LineTracer::math_limit(float pid,float range2,float range1){
    if (pid>range1)pid=range1;
    if(pid<range2)pid=range2;
    return pid;
}

float LineTracer::pid_sample(float sensor_val,float target_val)
{
    float p,i,d;
    diff[0] = diff[1];
    diff[1] = sensor_val - target_val;
    integral += (diff[1]+diff[0])*2*DELTA_T;
    p=  KP* diff[1];
    i = KI*integral;
    d = KD*(diff[1]-diff[0])/DELTA_T;
    //printf("math_limitは%f\n",math_limit(p, -100.0,100.0));
    return math_limit(p+d, -100.0,100.0);
}

/**
 * 走行体の向きを計算する
 * @param isOnLine true:ライン上/false:ライン外
 * @retval RIGHT  ライン上にある場合(右旋回指示)
 * @retval LEFT ライン外にある場合(左旋回指示)
 */
float LineTracer::calcDirection(float isOnLine) {
    return pid_sample(isOnLine*1.0,23.0);
}
void LineTracer::setter1(){
    integral = 0.0;
    diff[0]=0.0;
    diff[1]=0.0;
}

