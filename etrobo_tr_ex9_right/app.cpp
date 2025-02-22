﻿/******************************************************************************
 *  app.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Task main_task
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "app.h"
#include "RandomWalker.h"
#include "SonarSensor.h"
#include "etroboc_ext.h"

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle=0;

// using宣言
using ev3api::ColorSensor;
using ev3api::TouchSensor;
using ev3api::SonarSensor;
using ev3api::Motor;
using ev3api::Clock;

// Device objects
// オブジェクトを静的に確保する
SonarSensor gSonarSensor(PORT_3);
ColorSensor gColorSensor(PORT_2);
TouchSensor gTouchSensor(PORT_1);
Motor       gLeftWheel(PORT_C);
Motor       gRightWheel(PORT_B);
Clock       gClock;

// オブジェクトの定義
static Walker          *gWalker;
static LineMonitor     *gLineMonitor;
static Starter         *gStarter;
static SimpleTimer     *gScenarioTimer;
static SimpleTimer     *gWalkerTimer;
static LineTracer      *gLineTracer;
static Scenario        *gScenario;
static ScenarioTracer  *gScenarioTracer;
static RandomWalker    *gRandomWalker;
static SimpleTimer     *gSimpleTimer;
static bool judge = true;

// scene object
static Scene gScenes[] = {
    { TURN_LEFT,   1250 * 1000, 0 },  // 左旋回1.25秒
    { GO_STRAIGHT, 5000 * 1000, 0 },  // 直進5秒
    { TURN_LEFT,   1250 * 1000, 0 },  // 左旋回1.25秒
    { GO_STRAIGHT, 2500 * 1000, 0 }   // 直進2.5秒
};

/**
 * EV3システム生成
 */
static void user_system_create() {
    // [TODO] タッチセンサの初期化に2msのdelayがあるため、ここで待つ
    tslp_tsk(2U * 1000U);

    // オブジェクトの作成
    gWalker          = new Walker(gLeftWheel,
                                  gRightWheel);
    gStarter         = new Starter(gTouchSensor);
    gLineMonitor     = new LineMonitor(gColorSensor);
    gScenarioTimer   = new SimpleTimer(gClock);
    gWalkerTimer     = new SimpleTimer(gClock);
    gLineTracer      = new LineTracer(gLineMonitor, gWalker);
    gScenario        = new Scenario(0);
    gScenarioTracer  = new ScenarioTracer(gWalker,
                                          gScenario,
                                          gScenarioTimer);
    gRandomWalker    = new RandomWalker(gLineTracer,
                                        gScenarioTracer,
                                        gStarter,
                                        gWalkerTimer,
                                        gWalker);
    gSimpleTimer     = new SimpleTimer(gClock);

    // シナリオを構築する
    for (uint32_t i = 0; i < (sizeof(gScenes)/sizeof(gScenes[0])); i++) {
        gScenario->add(&gScenes[i]);
    }
    // 初期化完了通知
    ev3_led_set_color(LED_ORANGE);
}

/**
 * EV3システム破棄
 */
static void user_system_destroy() {
    gLeftWheel.reset();
    gRightWheel.reset();

    delete gRandomWalker;
    delete gScenarioTracer;
    delete gScenario;
    delete gLineTracer;
    delete gWalkerTimer;
    delete gScenarioTimer;
    delete gLineMonitor;
    delete gStarter;
    delete gWalker;
    delete gSimpleTimer;
}

/**
 * メインタスク
 */
void main_task(intptr_t unused) {
    user_system_create();  // センサやモータの初期化処理

    // 周期ハンドラ開始
    sta_cyc(CYC_TRACER);

    slp_tsk();  // バックボタンが押されるまで待つ

    // 周期ハンドラ停止
    stp_cyc(CYC_TRACER);

    user_system_destroy();  // 終了処理

    ext_tsk();
}

/**
 * ライントレースタスク
 */
void tracer_task(intptr_t exinf) {
        gRandomWalker -> aft_goal();
}


