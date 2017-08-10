#include"Prev.h"
#include"Game.h"
int WINAPI WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
#ifdef _DEBUG
    #pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
    system("mode con lines=50 cols=100");
    system("color 0A");
#endif
    Direct::getSingleton();
    FBXManager::getSingleton();
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