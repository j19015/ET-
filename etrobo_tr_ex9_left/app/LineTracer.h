/******************************************************************************
 *  LineTracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineTracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_LINETRACER_H_
#define EV3_APP_LINETRACER_H_

#include "LineMonitor.h"
#include "Walker.h"

class LineTracer {
public:
    LineTracer(const LineMonitor* lineMonitor,
               Walker* walker);

    //void run();
    //void run2();
    void run_speaed(int,int);
    void setter1();
    float math_limit(float pid,float range2,float range1);
    float pid_sample(float sensor_val,float target_val);

private:
    const LineMonitor* mLineMonitor;
    Walker* mWalker;
    bool mIsInitialized;
    float calcDirection(float isOnLine);
};

#endif  // EV3_APP_LINETRACER_H_
