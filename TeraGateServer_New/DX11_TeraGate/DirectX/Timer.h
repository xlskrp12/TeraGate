#pragma once
#include"Prev.h"
class Timer
{
public:
    Timer();
    void reset();
    bool isOn();
    void setCooltime(double timePerOn);
    void setDelay(double delay);
public:
    // 시작시간, 최종시간
    clock_t _start, _end;
    clock_t _delay;
    // 실행시간 = 최종시간 - 시작시간
    double _time;
    double _renderCount;
    double _cooltime;
};