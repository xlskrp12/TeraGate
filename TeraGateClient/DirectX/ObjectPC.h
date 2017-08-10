#pragma once
#include"Object.h"
//#include"Server.h"
class ObjectPC : public Object
{
public:
    ObjectPC();
    ~ObjectPC();
    void update();
	//---------------------------------------------------------------
	//Server *_server = new Server;
	//---------------------------------------------------------------
};

