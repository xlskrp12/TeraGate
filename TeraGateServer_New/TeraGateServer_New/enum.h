#pragma once
#include "prev.h"




enum {
	// C 중앙
	// L  왼쪽팀 메인게이트
	// LC 왼쪽팀에서 중앙 길 게이트
	// LN 왼쪽팀에서 엔피씨 팀(위쪽 팀) 길 게이트
	// LR 왼쪽팀에서 오른쪽 팀 길 게이트
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
	TEAM_L,// 팀 레프트 (왼쪽팀)
	TEAM_R,// 팀 라이트 (오른쪽팀)
	TEAM_N,// 팀 엔피씨 (윗팀)
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
	MAGICIAN_ATTACK_A = 45,//원래는 42인데 너무 답답해서 짧게 설정함.
	MAGICIAN_ATTACK_Z = 55,//원래는 62인데 너무 답답해서 짧게 설정함.
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