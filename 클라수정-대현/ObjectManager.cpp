#include "ObjectManager.h"
ObjectManager::ObjectManager()
{
    _timer.setCooltime(0.05);
}
ObjectManager::~ObjectManager()
{
}
void ObjectManager::addObject(Object* object)
{
    _object.push_back(object);
}
void ObjectManager::updateCommon()
{
    for (int a = 0; a < _object.size(); a++)
    {
        _object[a]->update();
    }
}
void ObjectManager::updateCollision()
{
    D3DXVECTOR3 aPosBeforeUpdate;//�ڿ������� ȸ����Ű�� ���� ������Ʈ ���� a������Ʈ ��ǥ�� �޴´�. �갡 ������ �����浹�� �� ������Ʈ�� 180�� �ݴ�� ȸ���ϴ� ��찡 ���� �ǰ� ���ڿ������� ���̴� ��찡 �ֵ�.
    D3DXVECTOR3 aPos;
    D3DXVECTOR3 bPos;
    D3DXVECTOR3 aPosAfterUpdate; //������Ʈ ���� ��ǥ�� �޴´�.
    float distance;
    float sizeDistance;
    float collisionDistance;
    float rotation;
    for (int a = 0; a < _object.size(); a++)
    {
        aPosBeforeUpdate = _object[a]->getPos();
		if (true == _object[a]->_exist)
		{
			_object[a]->update();
			if(_object[a]->getID() != Object::ID_STATIC)
				for(int b=0;b<_object.size();b++)
				{
					if(a != b)
					{
						if(_object[a]->isCollision(*_object[b]))
						{
							aPos         = _object[a]->getPos();
							bPos         = _object[b]->getPos();
							distance     = sqrt(pow(bPos.x-aPos.x, 2)+pow(bPos.z-aPos.z, 2));
							sizeDistance = _object[a]->getSize()+_object[b]->getSize();
							if(distance<sizeDistance)
							{
								_object[a]->setPosX(bPos.x + sizeDistance*cos(atan2(aPos.z-bPos.z, aPos.x-bPos.x)));
								_object[a]->setPosZ(bPos.z + sizeDistance*sin(atan2(aPos.z-bPos.z, aPos.x-bPos.x)));
								aPosAfterUpdate = _object[a]->getPos();
								//�ڿ������� ȸ��2
								rotation = RADIAN_TO_DEGREE(atan2(aPosAfterUpdate.z-aPosBeforeUpdate.z, aPosAfterUpdate.x-aPosBeforeUpdate.x));
								_object[a]->setRotY(rotation);
							}
						}
					}//if(a!=b)
				}//for(int b=0;*/
		}
    }//for int a=0;
}
int  ObjectManager::getObjectAmount()
{
    return _object.size();
}
D3DXMATRIX ObjectManager::getWorldMatrix(int objectIndex)
{
    return _object[objectIndex]->getMtxWorld();
}