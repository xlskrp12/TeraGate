#include"Prev.h"
#include"Game.h"
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
    Game::getSingletone()->init();
    //�޸� üũ
    {
        MEMORYSTATUSEX memoryStatusEx;
        memoryStatusEx.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memoryStatusEx);
        DWORDLONG totalVirtualMemory = memoryStatusEx.ullTotalPageFile;

        DWORDLONG virtualMemoryUsed = memoryStatusEx.ullTotalPageFile - memoryStatusEx.ullAvailPageFile;

        //printf("�����μ��� ��������������������������������������\n");
        //printf("������ �޸� ��뷮 : %d\n", virtualMemoryUsed);
        //printf("������ �޸� ��뷮 : %d\n");
        //printf("������������������������������������������������������������\n");
    }
    Game::getSingletone()->run();
    Game::getSingletone()->release();
    return 0;
}