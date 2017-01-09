#pragma once
#include"Object.h"
//▼ ObjectManager 클래스 요약
//  오브젝트의 생성, 삭제, 조작, 상호작용 등 관리하는 클래스.
//
//▼ 오브젝트를 생성하는 경우
//  1. 인게임 씬의 초기화로 오브젝트를 생성한다.
//  2. 오브젝트가 다른 오브젝트를 생성한다.
//
//▼ ObjectManager가 싱글톤이어야 하는 이유
//  인게임 씬뿐만 아니라 오브젝트로부터의 접근을 가능하게하기 위해서.
class ObjectManager
{
    //▼ 싱글톤
private:
    ObjectManager();
public:
    static ObjectManager* mInstance;
    static ObjectManager* getInstance();
    //▼ 메소드
public:
    ~ObjectManager();
    void add(Object* object);
    void deleteObject();
    void setAllDeleteFlagTrue();
    void proc(HWND, UINT, WPARAM, LPARAM);
    void update();
    void render();
    void debug();
    void order();
    void updateDamage();
    int  getQuantity();

    //▼ 상수
    const static int MAX_OBJECT_QUANTITY = 1000;// 오브젝트 최대 생성 개수
protected:
    //▼ 변수
    Object* _object[MAX_OBJECT_QUANTITY];// Object포인터의 배열. STL을 배우기 전까지는 괜히 포인터 배열 자체를 동적할당하지 말자.
    int     _objectQuantity = 0;         // 생성한 오브젝트 수량.    
};