#pragma once
#include"prev.h"
struct DBData {
	D3DXVECTOR3 pos;
	int exp;
	int maxexp;
	int hp;
	bool connected;
};


class DataBase
{
public:
	DataBase();
	~DataBase();
};

