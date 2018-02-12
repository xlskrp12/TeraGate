/*
▶  컴파일러한테 dll 위치 명시해주기
컴파일러에서는 프로젝트-속성-상단 옵션에서 모든 구성, 모든 플랫폼 선택-디버깅-환경 입력란에 PATH=FMOD; 이렇게 적어주자.
이는 컴파일러한테 디버그&릴리즈 돌릴 때 dll파일이 어디있는지 알려주는 것이다.
▶  실행파일 디렉토리에 dll파일 두기
exe실행파일로 돌릴 경우 실행파일 디렉토리에 fmodex.dll 파일을 같이 두어야 돌릴 수 있다.
▶  x86, x64 호환성 문제
현재 x86에서만 돌아간다.
*/
#pragma once
// 기본 헤더
#include<stdio.h>
#include<Windows.h>
// FMOD 관련
#include "FMOD/fmod.hpp"
#pragma comment(lib, "FMOD/fmodex_vc.lib")
using namespace FMOD;
// SoundManager 클래스는 Sound 라고 재정의할 수가 없다.
// FMOD 자체에 Sound라는 클래스가 있기 때문..
class SoundClip
{
public:
    SoundClip();
    ~SoundClip();
    void init(char* filePrefix, int maxIndex, char* fileFormat);
    void setVolume(float volume);
    void play(int index);
    void playChannel(int index, int channel);
    void update();
    void release();
    // 멤버상수
    const static int STRING_LENGTH = 256;
    const static int SOUND_INDEX   = 512;  // 사운드 인덱스 = 사운드 종류 
    const static int CHANNEL_INDEX = 2048; // 채  널 인덱스 = 동시에 출력할 수 있는 사운드 개수
private:
    char       _string[STRING_LENGTH];
    System *   _system;
    Sound  *   _sound[SOUND_INDEX];
    Channel*   _channel;
    float      _volume;
};