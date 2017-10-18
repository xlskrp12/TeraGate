#include"packet.h"

class Timer
{
public:
	Timer() {
		_start = clock();
		_end = clock();
	}
	void reset() {
		_start = clock();
		_end = clock();
	}
	bool isOn() {
		_end = clock();
		_time = difftime(_end, _start) / CLOCKS_PER_SEC;
		if (TIME_PER_ON <= _time)
		{
			reset();
			return true;
		}
		else
			return false;
	}
	void setDelay(double delay)
	{
		_delay = delay;
	}
	void setCooltime(double cooltime)
	{
		_cooltime = cooltime;
	}

public:
	// 시작시간, 최종시간
	clock_t _start, _end;
	clock_t _delay;
	// 실행시간 = 최종시간 - 시작시간
	double _time;
	double _renderCount;
	const double TIME_PER_ON = 1.0f / 60.0f;
	double _cooltime;
};