#pragma once
#include "prev.h"




enum {
	// C �߾�
	// L  ������ ���ΰ���Ʈ
	// LC ���������� �߾� �� ����Ʈ
	// LN ���������� ���Ǿ� ��(���� ��) �� ����Ʈ
	// LR ���������� ������ �� �� ����Ʈ
	GATE_C,
	GATE_L,
	GATE_LN,
	GATE_LR,
	GATE_LC,
	GATE_R,
	GATE_RN,
	GATE_RL,
	GATE_RC,
	GATE_N,
	GATE_NL,
	GATE_NR,
	GATE_NC,
	GATE_TOTAL
};

enum {
	TEAM_L,// �� ����Ʈ (������)
	TEAM_R,// �� ����Ʈ (��������)
	TEAM_N,// �� ���Ǿ� (����)
	TEAM_TOTAL
};

enum STATE {
	STATE_IDLE,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_FLASH
};

enum Frame
{
	MAGICIAN_IDLE_A = 0,
	MAGICIAN_IDLE_Z = 20,
	MAGICIAN_MOVE_A = 21,
	MAGICIAN_MOVE_Z = 41,
	MAGICIAN_ATTACK_A = 45,//������ 42�ε� �ʹ� ����ؼ� ª�� ������.
	MAGICIAN_ATTACK_Z = 55,//������ 62�ε� �ʹ� ����ؼ� ª�� ������.
	MAGICIAN_FLASH_A = 63,
	MAGICIAN_FLASH_Z = 74,
	MAGICIAN_TOTAL = 74,


	MINION_MOVE_A = 0,
	MINION_MOVE_Z = 9,
	MINION_ATTACK_A = 10,
	MINION_ATTACK_Z = 39,
	MINION_TOTAL = 40,
	FRAME_TOTAL = 100
};