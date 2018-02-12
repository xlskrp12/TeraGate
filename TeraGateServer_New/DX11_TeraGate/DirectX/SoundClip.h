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
#include<stdio.h>
#include<Windows.h>
// FMOD ����
#include "FMOD/fmod.hpp"
#pragma comment(lib, "FMOD/fmodex_vc.lib")
using namespace FMOD;
// SoundManager Ŭ������ Sound ��� �������� ���� ����.
// FMOD ��ü�� Sound��� Ŭ������ �ֱ� ����..
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
    // ������
    const static int STRING_LENGTH = 256;
    const static int SOUND_INDEX   = 512;  // ���� �ε��� = ���� ���� 
    const static int CHANNEL_INDEX = 2048; // ä  �� �ε��� = ���ÿ� ����� �� �ִ� ���� ����
private:
    char       _string[STRING_LENGTH];
    System *   _system;
    Sound  *   _sound[SOUND_INDEX];
    Channel*   _channel;
    float      _volume;
};