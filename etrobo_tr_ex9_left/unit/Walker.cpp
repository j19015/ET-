/******************************************************************************
 *  Walker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class Walker
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Walker.h"
#include "LineMonitor.h"
#include "LineTracer.h"
// 定数宣言
const int Walker::LOW    = 30;    // 低速
const int Walker::NORMAL = 50;    // 通常
const int Walker::HIGH   = 70;    // 高速

const int Walker::RIGHT  = 0;     // 左方向
const int Walker::LEFT   = 1;     // 右方向

/**
 * コンストラクタ
 * @param leftWheel  左モータ
 * @param rightWheel 右モータ
 */
Walker::Walker(ev3api::Motor& leftWheel,
                                 ev3api::Motor& rightWheel)
    : mLeftWheel(leftWheel),
      mRightWheel(rightWheel),
      mForward(HIGH)/*,
      mTurn(RIGHT) */{
}


void Walker::run_speaed(int spead1,int spead2){
    float rightPWM=spead1;
    float leftPWM=spead1;

    if(mTurn<0){
        mTurn*=-1;
        rightPWM+=mTurn*4.0;
        leftPWM-=mTurn*4.0;
    }
    else if(mTurn>0){
        leftPWM+=mTurn*4.0;
        rightPWM-=mTurn*4.0;
    }
    else{
        rightPWM=spead2;
        leftPWM=spead2;
    }

    mRightWheel.setPWM(leftPWM);
    mLeftWheel.setPWM(rightPWM);
}

/**
 * 走行に必要なものをリセットする
 */
void Walker::init() {
    // モータをリセットする
    mLeftWheel.reset();
    mRightWheel.reset();
}
void Walker::go(){
    int rightPWM = 50;
    int leftPWM = 50;
    mLeftWheel.setPWM(leftPWM);
    mRightWheel.setPWM(rightPWM);
}
void Walker::go2(){
    int rightPWM = 8;
    int leftPWM = 8;
    mLeftWheel.setPWM(leftPWM);
    mRightWheel.setPWM(rightPWM);
}
void Walker::stop(){
    int rightPWM = 0;
    int leftPWM = 0;
    mLeftWheel.setPWM(leftPWM);
    mRightWheel.setPWM(rightPWM);
}
void Walker::roll(){
    int rightPWM = 0;
    int leftPWM = 30;
    mLeftWheel.setPWM(leftPWM);
    mRightWheel.setPWM(rightPWM);
}
/**
 * PWM値を設定する
 * @param forward 前進値
 * @param turn    旋回方向
 */
void Walker::setCommand(int forward, float turn) {
    mForward = forward;
    mTurn    = turn;
}

float Walker::math_limit(float pid,float range2,float range1){
    if (pid>range1)pid=range1;
    if(pid<range2)pid=range2;
    return pid;
}
