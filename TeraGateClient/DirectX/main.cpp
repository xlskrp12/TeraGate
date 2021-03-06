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
    //메모리 체크
    {
        MEMORYSTATUSEX memoryStatusEx;
        memoryStatusEx.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memoryStatusEx);
        DWORDLONG totalVirtualMemory = memoryStatusEx.ullTotalPageFile;

        DWORDLONG virtualMemoryUsed = memoryStatusEx.ullTotalPageFile - memoryStatusEx.ullAvailPageFile;

        //printf("┌프로세스 정보─────────────────\n");
        //printf("│가상 메모리 사용량 : %d\n", virtualMemoryUsed);
        //printf("│물리 메모리 사용량 : %d\n");
        //printf("└─────────────────────────────\n");
    }
    Game::getSingletone()->run();
    Game::getSingletone()->release();
    return 0;
}