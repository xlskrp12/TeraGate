#include"object.h"

const float OBJECT::GATE_RADIUS = 3000;
const D3DXVECTOR3 OBJECT::GATE_C_POS = D3DXVECTOR3(0, 0, 0);
const D3DXVECTOR3 OBJECT::GATE_L_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(210)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(210)));
const D3DXVECTOR3 OBJECT::GATE_LN_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(170)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(170)));
const D3DXVECTOR3 OBJECT::GATE_LR_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(250)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(250)));
const D3DXVECTOR3 OBJECT::GATE_LC_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(210)) / 2, 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(210)) / 2);
const D3DXVECTOR3 OBJECT::GATE_R_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(-30)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(-30)));
const D3DXVECTOR3 OBJECT::GATE_RL_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(-70)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(-70)));
const D3DXVECTOR3 OBJECT::GATE_RN_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(10)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(10)));
const D3DXVECTOR3 OBJECT::GATE_RC_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(-30)) / 2, 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(-30)) / 2);
const D3DXVECTOR3 OBJECT::GATE_N_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(90)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(90)));
const D3DXVECTOR3 OBJECT::GATE_NL_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(130)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(130)));
const D3DXVECTOR3 OBJECT::GATE_NR_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(50)), 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(50)));
const D3DXVECTOR3 OBJECT::GATE_NC_POS = D3DXVECTOR3(GATE_RADIUS*cos(DEGREE_TO_RADIAN(90)) / 2, 0, GATE_RADIUS*sin(DEGREE_TO_RADIAN(90)) / 2);

OBJECT::OBJECT()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_pos.z = 0;
	m_rot.x = 0;
	m_rot.y = 0;
	m_rot.z = 0;
	m_size = 0;
	m_sightSize = 0;
	m_mapsize = 3150;
	m_movement = 10;
}

void OBJECT::setPos(D3DXVECTOR3 pos)
{ 
	m_pos = pos;
}
void OBJECT::setPos(float x, float y, float z) 
{ 
	m_pos = { x, y, z };
};
void OBJECT::setPosX(float x) 
{ 
	m_pos.x = x;
}
void OBJECT::setPosY(float y) 
{ 
	m_pos.y = y;
}
void OBJECT::setPosZ(float z) 
{ 
	m_pos.z = z;
}
void OBJECT::setRot(D3DXVECTOR3 rot) 
{ 
	m_rot = rot;
}
void OBJECT::setRot(float x, float y, float z) 
{
	m_rot = { x, y, z };
}
void OBJECT::setRotX(float x) 
{ 
	m_rot.x = x;
}
void OBJECT::setRotY(float y) 
{ 
	m_rot.y = y;
}
void OBJECT::setRotZ(float z) 
{ 
	m_rot.z = z;
}

// 이동속도 세팅, 확인용
void OBJECT::setMovement(float movement)
{
	m_movement = movement;
}

float OBJECT::getMovement()
{
	return m_movement;
}


// 위치 ,rot 확인용
D3DXVECTOR3 OBJECT::getPos()
{
	return m_pos;
}
float OBJECT::getPosX()
{
	return m_pos.x;
}
float OBJECT::getPosY()
{
	return m_pos.y;
}
float OBJECT::getPosZ()
{
	return m_pos.z;
}
D3DXVECTOR3 OBJECT::getRot()
{
	return m_rot;
}
float OBJECT::getRotX()
{
	return m_rot.x;
}
float OBJECT::getRotY()
{
	return m_rot.y;
}
float OBJECT::getRotZ()
{
	return m_rot.z;
}

//team 세팅용
void OBJECT::setTeam(int teamId)
{
	m_teamId = teamId;
}

int OBJECT::getTeamId()
{
	return m_teamId;
}

// hp 세팅 및 확인용
void OBJECT::setHP(int HP)
{
	m_hp = HP;
}

void OBJECT::setMaxHP(int maxHP)
{
	m_maxHp = maxHP;
}

float OBJECT::getHP()
{
	return m_hp;
}

float OBJECT::getMaxHP()
{
	return m_maxHp;
}


// 시야 확인용
bool OBJECT::isInSight(OBJECT& object)
{
	if (fabs(object.m_pos.x - m_pos.x) < object.m_size + m_sightSize)
		if (fabs(object.m_pos.y - m_pos.y) < object.m_size + m_sightSize)
			if (fabs(object.m_pos.z - m_pos.z) < object.m_size + m_sightSize)
				if (pow(m_pos.x - object.m_pos.x, 2) + pow(m_pos.y - object.m_pos.y, 2) + pow(m_pos.z - object.m_pos.z, 2) < pow(m_sightSize + object.m_size, 2))
				{
					return true;
				}
	return false;
}

void OBJECT::collisionCheck(int id)
{

}