#include "Timer.h"
Timer::Timer()
{
    _start     = clock();
    _end       = clock();
    _delay     = 0;
    _cooltime  = 1;
}
void Timer::reset()
{
    _start     = clock();
    _end       = clock();
}
bool Timer::isOn()
{
    _end  = clock();
    _time = difftime(_end, _start) / CLOCKS_PER_SEC;
    if(_delay + _cooltime <= _time)
    {
        _delay = 0;
        reset();
        return true;
    }
    else
        return false;
}
void Timer::setDelay(double delay)
{
    _delay = delay;
}
void Timer::setCooltime(double cooltime)
{
    _cooltime = cooltime;
}