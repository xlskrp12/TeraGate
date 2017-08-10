#include "ObjectPC.h"
ObjectPC::ObjectPC()
{
}
ObjectPC::~ObjectPC()
{
}
void ObjectPC::update()
{
    //// 중력, 점프 관련
    static float forceY       = 0.f;
    static float forceJump    = 5.0f;
    static float gravity      = 0.20f;
    static float lowestHeight = 0.f;
    static float baseHeight   = 0.f;
    if (_pos.y <= baseHeight)
        forceY = 0.f;
    forceY = forceY - gravity;
    if (GetKeyState(VK_NUMPAD9) & 0x8000)
    {
        if (_pos.y == baseHeight)
            forceY = forceY + forceJump;
    }
    _pos.y = _pos.y + forceY;
    if (_pos.y < baseHeight)
        _pos.y = baseHeight;
    // 이동관련
    static float movement     = 2.0f;
    static float rotationUnit = 0.2f;
    if (GetKeyState(VK_NUMPAD7) & 0x8000)
        movement = 5.f;
    else
        movement = 2.f;


    // 복합키, 단독키 입력시 방향처리
    // ↗
    /*if ((GetKeyState(VK_NUMPAD8) & 0x8000) && (GetKeyState(VK_NUMPAD6) & 0x8000))
        _rot.y = 45;
    // ↖
    else if ((GetKeyState(VK_NUMPAD8) & 0x8000) && (GetKeyState(VK_NUMPAD4) & 0x8000))
        _rot.y = -45;
    // ↘
    else if ((GetKeyState(VK_NUMPAD2) & 0x8000) && (GetKeyState(VK_NUMPAD6) & 0x8000))
        _rot.y = 135;
    // ↙
    else if ((GetKeyState(VK_NUMPAD2) & 0x8000) && (GetKeyState(VK_NUMPAD4) & 0x8000))
        _rot.y = -135;

    else if (GetKeyState(VK_NUMPAD8) & 0x8000)
        _rot.y = 0;
    else if (GetKeyState(VK_NUMPAD2) & 0x8000)
        _rot.y = -180;
    else if (GetKeyState(VK_NUMPAD4) & 0x8000)
        _rot.y = -90;
    else if (GetKeyState(VK_NUMPAD6) & 0x8000)
        _rot.y = 90;*/
    // 어쨌든 이동키를 하나라도 누른다면 이동
    if ((GetKeyState(VK_NUMPAD8) & 0x8000) || (GetKeyState(VK_NUMPAD6) & 0x8000) || (GetKeyState(VK_NUMPAD2) & 0x8000) || (GetKeyState(VK_NUMPAD4) & 0x8000))
    {


        // 맵 사이즈로 나가게 된다면?
        if (pow(1000, 2) < pow(_pos.x + movement * sin(DEGREE_TO_RADIAN(_rot.y)), 2) + pow(_pos.z + movement * cos(DEGREE_TO_RADIAN(_rot.y)), 2))
        {

        }
        else
        {
          /* _pos.x += movement * sin(DEGREE_TO_RADIAN(_rot.y));
            _pos.z += movement * cos(DEGREE_TO_RADIAN(_rot.y));*/
			_rot.y = _ServerDef->player.roty;
			_pos.x = _ServerDef->player.x;
			_pos.z = _ServerDef->player.z;
			//std::cout << _pos.x << " , " << _pos.z << std::endl;

			//---------------------------------------------------------------
			/*cs_packet_down *my_packet = reinterpret_cast<cs_packet_down *>(_server->send_buffer);
			my_packet->size = sizeof(my_packet);
			_server->send_wsabuf.len = sizeof(my_packet);

			DWORD iobyte;

			int retval = WSASend(_server->g_mysocket, &_server->send_wsabuf, 1, &iobyte, 0, NULL, NULL);
			if (retval)
			{
				int errCode = WSAGetLastError();
				printf("Error while sending packet [%d]", errCode);
			}
			*/
			//---------------------------------------------------------------
        }


    }
}