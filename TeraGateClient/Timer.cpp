#include"Timer.h"
Timer::Timer()
{
    reset();
}
void Timer::reset()
{
    _start       = clock();
    _end         = clock();
    _time        = difftime(_end, _start) / CLOCKS_PER_SEC;
    _renderCount = 0;
    _debugCount  = 0;
    _updateCount = 0;
}
void Timer::updateLogic()
{
    _updateCount++;
}
double Timer::getTime()
{
    // getTime()은 _time값을 업데이트하고 반환한다.
    // 반환값은 무시하고 updateLogic()함수 대신 사용할 수도 있다.
    _end  = clock();
    _time = difftime(_end, _start) / CLOCKS_PER_SEC;
    return _time;
}
double Timer::getUpdateCount()
{
    return _updateCount;
}
double Timer::getRenderFPS()
{
    return _renderCount / _time;
}
double Timer::getDebugFPS()
{
    return _debugCount / _time;
}
double Timer::getLPS()
{
    return _updateCount / _time;
}
// 리턴값으로 업데이트 허용여부를 결정한다.
bool Timer::isGoingUpdate()
{
    if (getUpdateCount() < getTime() * MAX_UPDATE_PER_SEC)
        return true;
    else
        return false;
}
/*
void Timer::render(const HDC hDC, int x, int y)
{
    this->getTime();
    _renderCount++;
    // Release전용 디버그 문자열 출력
    char string[100];
    sprintf_s(string, sizeof(string), "▼ Timer::render()");
    print(hDC, x, y, string);
    sprintf_s(string, sizeof(string), "_renderCount : %.0f", _renderCount);
    print(hDC, x + 20, y + 20, string);
    sprintf_s(string, sizeof(string), "_updateCount : %.0f", _updateCount);
    print(hDC, x + 20, y + 40, string);
    sprintf_s(string, sizeof(string), "_time : %.3f", _time);
    print(hDC, x + 20, y + 60, string);
    sprintf_s(string, sizeof(string), "FPS : %.3f", _renderCount / _time);
    print(hDC, x + 20, y + 80, string);
    sprintf_s(string, sizeof(string), "LPS : %.3f", _updateCount / _time);
    print(hDC, x + 20, y + 100, string);
}
void Timer::debug(const HDC hDC, int x, int y)
{
    this->getTime();
    _debugCount++;
    // 디버그 문자열 출력
    char string[100];
    sprintf_s(string, sizeof(string), "▼ Timer::debug()");
    print(hDC, x, y, string);
    sprintf_s(string, sizeof(string), "_debugCount : %.0f", _debugCount);
    print(hDC, x + 20, y + 20, string);
    sprintf_s(string, sizeof(string), "_updateCount : %.0f", _updateCount);
    print(hDC, x + 20, y + 40, string);
    sprintf_s(string, sizeof(string), "_runtime : %.3f", _time);
    print(hDC, x + 20, y + 60, string);
    sprintf_s(string, sizeof(string), "FPS : %.3f", _debugCount / _time);
    print(hDC, x + 20, y + 80, string);
    sprintf_s(string, sizeof(string), "LPS : %.3f", _updateCount / _time);
    print(hDC, x + 20, y + 100, string);
}
*/