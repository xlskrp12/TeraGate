#pragma once
#include<time.h>    // clock_t   사용을 위해 선언
typedef class Timer
{
public:
    Timer();
    void   reset();
    void   updateLogic();
    double getTime();
    double getUpdateCount();
    double getRenderFPS();
    double getDebugFPS();
    double getLPS();
    bool   isGoingUpdate();
//    void   render(const HDC hDC, int x, int y);
//    void   debug(const HDC hDC, int x, int y);
protected:
    // 시작시간, 최종시간
    clock_t _start, _end;
    // 실행시간 = 최종시간 - 시작시간
    double _time;
    double _renderCount;
    double _debugCount;
    double _updateCount;
    const static int MAX_UPDATE_PER_SEC = 100;
}Timer;