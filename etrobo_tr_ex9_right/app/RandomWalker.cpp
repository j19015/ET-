/******************************************************************************
 *  RandomWalker.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/02/07
 *  Implementation of the Class RandomWalker
 *  Author: Kenya Yabe
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include <stdlib.h>
#include "Clock.h"
#include "etroboc_ext.h"
#include "RandomWalker.h"

// 定数宣言
const int RandomWalker::MIN_TIME = 5000 * 1000;    // 切り替え時間の最小値
const int RandomWalker::MAX_TIME = 15000 * 1000;   // 切り替え時間の最大値
int i = 0;
int yellow_bind = 0;
int get_c;
int get_b;

/**
 * コンストラクタ
 * @param lineTracer      ライントレーサ
 * @param scenarioTracer  シナリオトレーサ
 * @param starter         スタータ  
 * @param simpleTimer     タイマ
 * @param walker
 */
RandomWalker::RandomWalker(LineTracer* lineTracer,
                           ScenarioTracer* scenarioTracer,
                           const Starter* starter,
                           SimpleTimer* simpleTimer,
                           Walker* walker)
    : mWalker(walker),
      mLineTracer(lineTracer),
      mScenarioTracer(scenarioTracer),
      mStarter(starter),
      mSimpleTimer(simpleTimer),
      mState(UNDEFINED) {
    ev3api::Clock* clock = new ev3api::Clock();

    srand(clock->now());  // 乱数をリセットする

    delete clock;
}

/**
 * ランダム走行する
 */
void RandomWalker::aft_goal(){
    get_b = ev3_motor_get_counts(EV3_PORT_B);
    get_c = ev3_motor_get_counts(EV3_PORT_C);
    printf("bの値は%d,cの値は%d\n",get_b,get_c);//bの値は1713,cの値は1776
    if(ev3_color_sensor_get_color(EV3_PORT_2)==3){//みどりちゃｎ
            i = 2;
    }
    switch(i){

        case 1://黄色踏んだ時
            ev3_motor_reset_counts(EV3_PORT_C);
            ev3_motor_reset_counts(EV3_PORT_B);
            mWalker -> go();
            if(ev3_color_sensor_get_color(EV3_PORT_2)==6){
                if(yellow_bind < 23)yellow_bind++;
                else i=5;
            }
            break;

        case 2://緑踏んで黒見つけたとき
            mWalker -> roll();
            if(ev3_color_sensor_get_color(EV3_PORT_2)==1){
                i=3;
                }
            break;

        case 3://緑踏んで黒見つけて車輪が1070行ったとき
                printf("壁との距離は%d\n",ev3_ultrasonic_sensor_get_distance(EV3_PORT_3));
                if(ev3_ultrasonic_sensor_get_distance(EV3_PORT_3) <100 ) {
                    mWalker -> stop();
                    i=4;
                }
                else mLineTracer -> run_speaed(20,20);
            break;

        case 4://赤のあと
            mWalker -> go2();
            get_c = ev3_motor_get_counts(EV3_PORT_C);
            printf("壁との距離は%d\n",ev3_ultrasonic_sensor_get_distance(EV3_PORT_3));
            if(ev3_ultrasonic_sensor_get_distance(EV3_PORT_3) <= 29){
                    mWalker -> stop();
                    ETRoboc_notifyCompletedToSimulator();
                }	
            break;

        case 5://
            mLineTracer -> run_speaed(20,20);
            break;

        default:
            if(ev3_color_sensor_get_color(EV3_PORT_2)== COLOR_YELLOW ){
                i=1;
            }
            else {
                mLineTracer->run_speaed(80,100);
            }
    }
    printf("%d\n",i);
}