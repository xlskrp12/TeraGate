/*
��  �����Ϸ����� dll ��ġ ������ֱ�
�����Ϸ������� ������Ʈ-�Ӽ�-��� �ɼǿ��� ��� ����, ��� �÷��� ����-�����-ȯ�� �Է¶��� PATH=FMOD; �̷��� ��������.
�̴� �����Ϸ����� �����&������ ���� �� dll������ ����ִ��� �˷��ִ� ���̴�.
��  �������� ���丮�� dll���� �α�
exe�������Ϸ� ���� ��� �������� ���丮�� fmodex.dll ������ ���� �ξ�� ���� �� �ִ�.
��  x86, x64 ȣȯ�� ����
���� x86������ ���ư���.
*/
#pragma once
// �⺻ ���
#include"SoundClip.h"
class SoundManager
{
public:// �̱���
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
    // ������
    const static int STRING_LENGTH = 256;
    const static int SOUND_INDEX   = 5;   // ���� �ε��� = ���� Ŭ�� ���� 
    const static int CHANNEL_INDEX = 2048; // ä  �� �ε��� = ���ÿ� ����� �� �ִ� ���� ����
    // ���� Ŭ�� �ε���
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