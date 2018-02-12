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
    // ���۽ð�, �����ð�
    clock_t _start, _end;
    clock_t _delay;
    // ����ð� = �����ð� - ���۽ð�
    double _time;
    double _renderCount;
    double _cooltime;
};