#pragma once
#include"Prev.h"
class GameTimer
{
public:
    GameTimer();
    void   setResetTimeCycle(double time);
    void   reset();
    void   update();
    double getRuntime();
    double getTime();
    double getUpdateCount();
    double getRenderFPS();
    double getDebugFPS();
    double getLPS();
    bool   isGoingUpdate();
    void   render();//const HDC hDC, int x, int y);
    void   debug();//const HDC hDC, int x, int y);
protected:
    // 리셋하지 않을 클락, 런타임
    clock_t _runtimeStart, _runtimeEnd;
    double  _runtime;


    // 시작시간, 최종시간
    clock_t _start, _end;
    // 실행시간 = 최종시간 - 시작시간
    double _time;
    double _resetTimeCycle;
    double _renderCount;
    double _debugCount;
    double _updateCount;
    const static int MAX_UPDATE_PER_SEC = 60;
};