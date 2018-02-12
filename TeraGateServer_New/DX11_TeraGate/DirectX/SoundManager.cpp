#include "SoundManager.h"
SoundManager* SoundManager::_singleton = NULL;
SoundManager* SoundManager::getSingleton()
{
    if (_singleton == NULL)
    {
        _singleton = new SoundManager;
    }
    return _singleton;
}
SoundManager::SoundManager()
{
    setVolume(0.3);
    setBGMVolume(0.3);
    System_Create(&_system);
    _system->init(CHANNEL_INDEX, FMOD_INIT_NORMAL, NULL);
    for (int i = 1; i <= SOUND_INDEX; i++)
    {
        loadSound("Resource/Sound/Sound (", i, "mp3");
    }
    for (int i = 1; i <= INDEX_ANNOUNCER; i++)
    {
        _announcer.init("Resource/Sound/Announcer/Announcer (", i, "mp3");
        _announcer.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_MAGICIAN; i++)
    {
        _magician.init("Resource/Sound/Magician/Magician (", i, "mp3");
        _magician.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_MINION; i++)
    {
        _minion.init("Resource/Sound/Minion/Minion (", i, "mp3");
        _minion.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_EFFECT; i++)
    {
        _effect.init("Resource/Sound/Effect/Effect (", i, "mp3");
        _effect.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_JOHNNY; i++)
    {
        _johnny.init("Resource/Sound/Johnny/Johnny (", i, "mp3");
        _johnny.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_MARINE; i++)
    {
        _marine.init("Resource/Sound/Marine/Marine (", i, "mp3");
        _marine.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_ZATO; i++)
    {
        _zato.init("Resource/Sound/Zato/Zato (", i, "mp3");
        _zato.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_BGM; i++)
    {
        _bgm.init("Resource/Sound/BGM/BGM (", i, "mp3");
        _bgm.setVolume(1.0f);
    }
    for (int i = 1; i <= INDEX_GATE; i++)
    {
        _gate.init("Resource/Sound/GATE/gate (", i, "mp3");
        _gate.setVolume(1.0f);
    }
}
SoundManager::~SoundManager()
{
    release();
}
void SoundManager::loadSound(char* filePrefix, int index, char* fileFormat)
{
    sprintf_s(_string, sizeof(_string), "%s%d).%s", filePrefix, index, fileFormat);
    _system->createSound(_string, FMOD_HARDWARE, 0, &_sound[index - 1]);
}
void SoundManager::setVolume(float volume)
{
    _volume = volume;
}
void SoundManager::setBGMVolume(float volume)
{
    _bgmVolume = volume;
}
void SoundManager::play(int index)
{
    _channel->setVolume(_volume);
    if (1 <= index && index <= SOUND_INDEX)
        _system->playSound(FMOD_CHANNEL_FREE, _sound[index - 1], 0, &_channel);
}
void SoundManager::playBGM(int index)
{
    _channelBGM->setVolume(_bgmVolume);
    if (1 <= index && index <= SOUND_INDEX)
        _system->playSound(FMOD_CHANNEL_FREE, _sound[index - 1], 0, &_channelBGM);
}
void SoundManager::update()
{
    // 사운드 시스템을 업데이트해주지 않으면
    // 일정 사운드 반복 후 사운드가 정지되는 현상이 발생한다.

    _system->update();
    _announcer.update();
    _magician.update();
    _minion.update();
    _effect.update();
    _johnny.update();
    _marine.update();
    _zato.update();
    _bgm.update();
}
void SoundManager::release()
{
    _system->release();
    _system->close();
}