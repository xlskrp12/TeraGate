#include"Prev.h"
#include"Game.h"
int __stdcall WinMain(HINSTANCE instance, HINSTANCE prevInstance, PSTR cmdLine, int cmdShow)
{
//��â ����ִ� ģ��
#ifdef _DEBUG
    //#pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
    //system("mode con lines=1000 cols=120");
    //system("color 0A");
    //HWND hConsole;
    //hConsole = GetConsoleWindow();
    //SetWindowPos(hConsole, NULL, 0, 0, 0, 0, SWP_NOZORDER/*hWndInsertAfter���ڹ���*/|SWP_NOSIZE/*cx cy���ڹ���*/);
#endif
    //���ϴ� �׽�Ʈ��
    #pragma comment(linker,"/entry:WinMainCRTStartup /subsystem:console")
    system("mode con lines=1000 cols=120");
    system("color 0A");
    HWND hConsole;
    hConsole = GetConsoleWindow();
    SetWindowPos(hConsole, NULL, 0, 0, 0, 0, SWP_NOZORDER/*hWndInsertAfter���ڹ���*/|SWP_NOSIZE/*cx cy���ڹ���*/);
    //��

    Direct::getSingleton();
    FBXManager::getSingleton();
    Game::getSingletone()->init();
    //��޸� üũ(���� ���ص� ��)
    {
        MEMORYSTATUSEX memoryStatusEx;
        memoryStatusEx.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memoryStatusEx);
        DWORDLONG totalVirtualMemory = memoryStatusEx.ullTotalPageFile;

        DWORDLONG virtualMemoryUsed = memoryStatusEx.ullTotalPageFile - memoryStatusEx.ullAvailPageFile;
        //�ñ��ϸ� ���
        //printf("�����μ��� ��������������������������������������\n");
        //printf("������ �޸� ��뷮 : %d\n", virtualMemoryUsed);
        //printf("������ �޸� ��뷮 : %d\n");
        //printf("������������������������������������������������������������\n");
    }
    Game::getSingletone()->run();
    Game::getSingletone()->release();
    return 0;
}