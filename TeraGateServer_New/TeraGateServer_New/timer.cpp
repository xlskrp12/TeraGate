#include"timer.h"

Timer _minionTimer[4];
//�̴Ͼ� ������ Ÿ�̸�
void Timer::MinionTimerSet()
{
	for (int i = 0; i<4; i++)
	{
		_minionTimer[i].setCooltime(8);
		_minionTimer[i].setDelay(i);
	}
}
