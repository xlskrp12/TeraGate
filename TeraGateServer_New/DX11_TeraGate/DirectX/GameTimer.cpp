#include "GameTimer.h"
GameTimer::GameTimer()
{
    _runtimeStart = clock();
    _runtimeEnd   = clock();
    _runtime      = difftime(_runtimeStart, _runtimeEnd) / CLOCKS_PER_SEC;
    reset();
}
void GameTimer::setResetTimeCycle(double time)
{
    _resetTimeCycle = time;
}
void GameTimer::reset()
{
    _start          = clock();
    _end            = clock();
    _time           = difftime(_end, _start) / CLOCKS_PER_SEC;
    _renderCount    = 0;
    _debugCount     = 0;
    _updateCount    = 0;
    _resetTimeCycle = 100000;
}
void GameTimer::update()
{
    _updateCount++;
}
double GameTimer::getRuntime()
{
    _runtimeEnd  = clock();
    _runtime     = difftime(_runtimeEnd, _runtimeStart) / CLOCKS_PER_SEC;
    return _runtime;
}
double GameTimer::getTime()
{
    // GetTime()은 mTime값을 업데이트하고 반환한다.
    // 반환값은 무시하고 Update()함수 대용으로 사용할 수도 있다.
    _end  = clock();
    _time = difftime(_end, _start) / CLOCKS_PER_SEC;

    if(_resetTimeCycle < _time)
        reset();

    return _time;
}
double GameTimer::getUpdateCount()
{
    return _updateCount;
}
double GameTimer::getRenderFPS()
{
    return _renderCount / _time;
}
double GameTimer::getDebugFPS()
{
    return _debugCount / _time;
}
double GameTimer::getLPS()
{
    return _updateCount / _time;
}
// 리턴값으로 업데이트 허용여부를 결정한다.
bool GameTimer::isGoingUpdate()
{
    if (getUpdateCount() < getTime() * MAX_UPDATE_PER_SEC)
        return true;
    else
        return false;
}
void GameTimer::render()//(const HDC hDC, int x, int y)
{
    this->getTime();
    _renderCount++;
    //// Release전용 디버그 문자열 출력
    //char string[100];
    //sprintf_s(string, sizeof(string), "▼ Timer::render()");
    //print(hDC, x, y, string);
    //sprintf_s(string, sizeof(string), "_renderCount : %.0f", _renderCount);
    //print(hDC, x + 20, y + 20, string);
    //sprintf_s(string, sizeof(string), "_updateCount : %.0f", _updateCount);
    //print(hDC, x + 20, y + 40, string);
    //sprintf_s(string, sizeof(string), "_time : %.3f", _time);
    //print(hDC, x + 20, y + 60, string);
    //sprintf_s(string, sizeof(string), "FPS : %.3f", _renderCount / _time);
    //print(hDC, x + 20, y + 80, string);
    //sprintf_s(string, sizeof(string), "LPS : %.3f", _updateCount / _time);
    //print(hDC, x + 20, y + 100, string);
}
void GameTimer::debug()//(const HDC hDC, int x, int y)
{
    //this->getTime();
    //_debugCount++;
    //// 디버그 문자열 출력
    //char string[100];
    //sprintf_s(string, sizeof(string), "▼ Timer::debug()");
    //print(hDC, x, y, string);
    //sprintf_s(string, sizeof(string), "_debugCount : %.0f", _debugCount);
    //print(hDC, x + 20, y + 20, string);
    //sprintf_s(string, sizeof(string), "_updateCount : %.0f", _updateCount);
    //print(hDC, x + 20, y + 40, string);
    //sprintf_s(string, sizeof(string), "_runtime : %.3f", _time);
    //print(hDC, x + 20, y + 60, string);
    //sprintf_s(string, sizeof(string), "FPS : %.3f", _debugCount / _time);
    //print(hDC, x + 20, y + 80, string);
    //sprintf_s(string, sizeof(string), "LPS : %.3f", _updateCount / _time);
    //print(hDC, x + 20, y + 100, string);
}