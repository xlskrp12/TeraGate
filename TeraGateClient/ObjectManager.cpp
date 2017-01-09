#include"ObjectManager.h"
//▼ 싱글톤
ObjectManager* ObjectManager::mInstance = NULL;
ObjectManager* ObjectManager::getInstance()
{
    if (mInstance == NULL)
        mInstance = new ObjectManager;
    return mInstance;
}
ObjectManager::ObjectManager()
{
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        _object[i] = NULL;
}
//▼ 메소드
ObjectManager::~ObjectManager()
{
    delete[] _object;
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        _object[i] = NULL;
}
// 사용방법 예제
// mObjectManager.add(new ObjectThresh);
void ObjectManager::add(Object* object)
{
    // 오브젝트 최대 생성수 미만이라면
    if (_objectQuantity < MAX_OBJECT_QUANTITY)
    {
        // 오브젝트를 포인터배열에 넣고
        // 오브젝트 개수를 1 가산한다.
        _object[_objectQuantity] = object;
        _objectQuantity++;
    }
}
/*
오브젝트의 플래그-딜리트를 체크해 메모리를 해제하고 NULL값을 넣는다.
오브젝트 수량을 0으로 초기화 한다.
인덱스가 비어있지 않다면 오브젝트 수량을 카운트한다.
*/
void ObjectManager::deleteObject()
{
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        if (_object[i] != NULL && _object[i]->getFlagDelete())
        {
            delete _object[i];
            _object[i] = NULL;
        }
    _objectQuantity = 0;
    for (int a = 0; a<MAX_OBJECT_QUANTITY; a++)
        if (_object[a] != NULL)
            _objectQuantity++;
}
// 모든 오브젝트의 플래그-딜리트를 참으로 만든다.
// 다음 deleteObject()가 호출될 때 모든 오브젝트가 삭제되게 된다.
void ObjectManager::setAllDeleteFlagTrue()
{
    for (int i = 0; i < MAX_OBJECT_QUANTITY; i++)
        if (_object[i] != NULL)
            _object[i]->setFlagDelete(TRUE);
}
void ObjectManager::proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    for (int i = 0; i < _objectQuantity; i++)
    {
//        오브젝트에게 주어지는 이벤트 처리
//        _object[i]->proc(hWnd, message, wParam, lParam);
    }
}
void ObjectManager::update()
{
    // ObjectManager 클래스 구조상
    // update() 내에서의 함수호출 순서는 꼭 지켜주자.
    // 1. 모든 오브젝트를 업데이트해 삭제 플래그를 갱신한다.
    // 2. 삭제 플래그가 참인 오브젝트를 삭제한다.
    // 3. 성능최적화를 위해 오브젝트 배열을 전위정렬한다.
    for (int i = 0; i<MAX_OBJECT_QUANTITY; i++)
        if (_object[i] != NULL)
            _object[i]->update();
    deleteObject();
    order();
}
void ObjectManager::render()
{
    for (int i = 0; i<MAX_OBJECT_QUANTITY; i++)
    {
        if (_object[i] != NULL)
            _object[i]->render();
    }
}
void ObjectManager::debug()
{
}
void ObjectManager::order()
{
    /*
    정렬작업 수행시 Big(O)에서 최적화가 필요하지만..
    기능상의 문제는 없다.

    비어있는 인덱스 값을 체크해 전위 인덱스로 끌어온다.
    ex) 구동 전 인덱스
    mObject[0] = NULL;
    mObject[1] = 제드;
    ex) 구동 후 인덱스
    mObject[0] = 제드;
    mObject[1] = NULL;
    */
    for (int a = 0; a<MAX_OBJECT_QUANTITY; a++)
        for (int b = 0; b < MAX_OBJECT_QUANTITY - 1; b++)
        {
            if (_object[b] == NULL && _object[b + 1] != NULL)
            {
                _object[b] = _object[b + 1];
                _object[b + 1] = NULL;
            }
        }
}
void ObjectManager::updateDamage()
{
}
int  ObjectManager::getQuantity()
{
    return _objectQuantity;
}