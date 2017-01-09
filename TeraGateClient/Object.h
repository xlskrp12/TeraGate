#pragma once
#include<Windows.h>
#include"Timer.h"
// 2d리소스를 인스턴스 포인터로 받았듯이 3d리소스도 포인터로 받아두자 { _model = Sprite::getInstance(); _sound = SoundManager::getInstance(); }
typedef class Object
{
public:
    //▼ 메소드
    Object();
    void   setPosition(double x, double y, double z) { _x = x; _y = y; _z = z; }
    bool   isPlayAble() { return _flagPlayAble; }
    void   setFlagDelete(bool flag) { _flagDelete = flag; }
    bool   getFlagDelete() { return _flagDelete; }
    // 인터페이스
    virtual void changeState(int state) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void debug() = 0;
    virtual void proc() = 0;
protected:
    //▼ 상수
    double _x, _y, _z;
    bool   _flagPlayAble = false;         // 플레이어블 플래그
    bool   _flagDelete = false;         // 이 값이 true면 오브젝트 매니저에서 삭제하게 해당 객체를 삭제하게 된다.
}Object;