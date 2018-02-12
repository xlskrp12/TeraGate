#include"SoundClip.h"
SoundClip::SoundClip()
{
    setVolume(0.5);
    System_Create(&_system);
    _system->init(CHANNEL_INDEX, FMOD_INIT_NORMAL, NULL);
}
SoundClip::~SoundClip()
{
    release();
}
void SoundClip::init(char* filePrefix, int index, char* fileFormat)
{
    sprintf_s(_string, sizeof(_string), "%s%d).%s", filePrefix, index, fileFormat);
    _system->createSound(_string, FMOD_HARDWARE, 0, &_sound[index - 1]);
}
void SoundClip::setVolume(float volume)
{
    _volume = volume;
}
void SoundClip::play(int index)
{
    _channel->setVolume(_volume);
    if (1 <= index && index <= SOUND_INDEX)
        _system->playSound(FMOD_CHANNEL_FREE, _sound[index - 1], 0, &_channel);
}
void SoundClip::playChannel(int index, int channel)
{
    _channel->setVolume(_volume);
    if (1 <= index && index <= SOUND_INDEX)
        _system->playSound(FMOD_CHANNEL_FREE, _sound[index - 1], 0, &_channel);
}
void SoundClip::update()
{
    // ���� �ý����� ������Ʈ������ ������
    // ���� ���� �ݺ� �� ���尡 �����Ǵ� ������ �߻��Ѵ�.
    _system->update();
}
void SoundClip::release()
{
    _system->release();
    _system->close();
}