#pragma once
#include"prev.h"


class OBJECT
{
public:
	const static float GATE_RADIUS;
	const static D3DXVECTOR3 GATE_C_POS;
	const static D3DXVECTOR3 GATE_L_POS;
	const static D3DXVECTOR3 GATE_LN_POS;
	const static D3DXVECTOR3 GATE_LR_POS;
	const static D3DXVECTOR3 GATE_LC_POS;
	const static D3DXVECTOR3 GATE_R_POS;
	const static D3DXVECTOR3 GATE_RL_POS;
	const static D3DXVECTOR3 GATE_RN_POS;
	const static D3DXVECTOR3 GATE_RC_POS;
	const static D3DXVECTOR3 GATE_N_POS;
	const static D3DXVECTOR3 GATE_NL_POS;
	const static D3DXVECTOR3 GATE_NR_POS;
	const static D3DXVECTOR3 GATE_NC_POS;

	OBJECT();
	//~OBJECT();

	void setPos(D3DXVECTOR3 pos);
	void setDes(D3DXVECTOR3 des);
	void setPos(float x, float y, float z);
	void setPosX(float x);
	void setPosY(float y);
	void setPosZ(float z);
	void setRot(D3DXVECTOR3 rot);
	void setRot(float x, float y, float z);
	void setRotX(float x);
	void setRotY(float y);
	void setRotZ(float z);

	void setMovement(float movement);
	float getMovement();
	
	D3DXVECTOR3 getPos();
	float getPosX();
	float getPosY();
	float getPosZ();
	D3DXVECTOR3 getRot();
	float getRotX();
	float getRotY();
	float getRotZ();

	//HP
	void setHP(int HP);
	void setMaxHP(int maxHP);
	float getHP();
	float getMaxHP();

	//Team
	void setTeam(int teamId);
	int getTeamId();

	//충돌체크
	//float getSize();
	//void  setSize(float size);

	//bool viewRange(int a, int b);
	bool isInSight(OBJECT& object);
	
	void collisionCheck(int id);

private:

protected:
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_des;
	D3DXVECTOR3 m_rot;

	float m_hp;
	float m_maxHp;
	
	int m_teamId;
	int m_gateId;
	bool m_isActive;

	//충돌체크용
	float m_size;
	float m_sightSize;

	float m_mapsize;
	float m_movement;

	//애니메이션
	int m_state;
	int m_frame;

	float m_attack;
};