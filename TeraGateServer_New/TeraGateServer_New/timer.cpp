#include"timer.h"

Timer _minionTimer[4];
//미니언 생성용 타이머
void Timer::MinionTimerSet()
{
	for (int i = 0; i<4; i++)
	{
		_minionTimer[i].setCooltime(8);
		_minionTimer[i].setDelay(i);
	}
}
