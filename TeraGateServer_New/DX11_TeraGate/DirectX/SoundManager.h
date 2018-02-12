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
#include"SoundClip.h"
class SoundManager
{
public:// 싱글톤
    static SoundManager* _singleton;
    static SoundManager* getSingleton();
    SoundManager();
    ~SoundManager();
    void loadSound(char* filePrefix, int maxIndex, char* fileFormat);
    void setVolume(float volume);
    void setBGMVolume(float volume);
    void play(int index);
    void playBGM(int index);
    void update();
    void release();
    // 멤버상수
    const static int STRING_LENGTH = 256;
    const static int SOUND_INDEX   = 5;   // 사운드 인덱스 = 사운드 클립 종류 
    const static int CHANNEL_INDEX = 2048; // 채  널 인덱스 = 동시에 출력할 수 있는 사운드 개수
    // 사운드 클립 인덱스
    const static int INDEX_ANNOUNCER = 2;
    const static int INDEX_MAGICIAN  = 3;
    const static int INDEX_MINION    = 7;
    const static int INDEX_EFFECT    = 1;
    const static int INDEX_JOHNNY = 9;
    const static int INDEX_MARINE = 3;
    const static int INDEX_ZATO   = 15;
    const static int INDEX_BGM    = 1;
    const static int INDEX_GATE   = 4;
    SoundClip _announcer;
    SoundClip _magician;
    SoundClip _minion;
    SoundClip _effect;
    SoundClip _johnny;
    SoundClip _marine;
    SoundClip _zato;
    SoundClip _bgm;
    SoundClip _gate;
private:
    char       _string[STRING_LENGTH];
    System *   _system;
    Sound  *   _sound[SOUND_INDEX];
    Channel*   _channel;
    Channel*   _channelBGM;
    float      _volume;
    float      _bgmVolume;
};