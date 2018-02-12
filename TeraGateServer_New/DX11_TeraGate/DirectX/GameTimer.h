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
    // �������� ���� Ŭ��, ��Ÿ��
    clock_t _runtimeStart, _runtimeEnd;
    double  _runtime;


    // ���۽ð�, �����ð�
    clock_t _start, _end;
    // ����ð� = �����ð� - ���۽ð�
    double _time;
    double _resetTimeCycle;
    double _renderCount;
    double _debugCount;
    double _updateCount;
    const static int MAX_UPDATE_PER_SEC = 60;
};