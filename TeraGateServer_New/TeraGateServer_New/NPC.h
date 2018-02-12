#pragma once
#include"object.h"

class NPC : public OBJECT
{
public:
	void NPCRespawn(int id);
	void SetDes(OBJECT obj);
	void npcAttack(OBJECT obj);

	void MoveNPC();

private:

	
};

