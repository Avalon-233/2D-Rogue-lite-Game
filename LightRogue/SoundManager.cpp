#include "SoundManager.h"
#include "ResourceLoader.h"

SoundManager::SoundManager()
{
    for (int i = 0; i < PoolSize; ++i)
        _pool[i] = std::make_unique<sf::Sound>(_bufLevelUp);
}

SoundManager::~SoundManager()
{
    _bgm.stop();
}

bool SoundManager::IsReady() const
{
    return _sfxReady;
}

void SoundManager::LoadAll()
{
    bool ok = true;
    ok &= LoadSoundBufferFromResource(_bufLevelUp,"levelup.wav");
    ok &= LoadSoundBufferFromResource(_bufPlayerShoot,"shoot.wav");
    ok &= LoadSoundBufferFromResource(_bufPlayerDamaged,"playerdamaged.wav");
    ok &= LoadSoundBufferFromResource(_bufEnemyDamaged,"enemydamaged.wav");
    _sfxReady = ok;

    _bgmReady = LoadMusicFromResource(_bgm, "BGM.ogg");
    if (_bgmReady)
    {
        _bgm.setLooping(true);
        _bgm.setVolume(40.f);
    }
}

sf::Sound& SoundManager::GetFreeSound()
{
    for (int i = 0; i < PoolSize; ++i)
    {
        if (_pool[_nextIndex]->getStatus() != sf::Sound::Status::Playing)
            return *_pool[_nextIndex];
        _nextIndex = (_nextIndex + 1) % PoolSize;
    }
    _pool[_nextIndex]->stop();
    return *_pool[_nextIndex];
}


void SoundManager::PlayLevelUp()
{
    if (!_sfxReady) return;
    sf::Sound& s = GetFreeSound();
    s.setBuffer(_bufLevelUp);
    s.play();
}

void SoundManager::PlayPlayerShoot()
{
    if (!_sfxReady) return;
    sf::Sound& s = GetFreeSound();
    s.setBuffer(_bufPlayerShoot);
    s.play();
}

void SoundManager::PlayPlayerDamaged()
{
    if (!_sfxReady) return;
    sf::Sound& s = GetFreeSound();
    s.setBuffer(_bufPlayerDamaged);
    s.play();
}

void SoundManager::PlayEnemyDamaged()
{
    if (!_sfxReady) return;
    sf::Sound& s = GetFreeSound();
    s.setBuffer(_bufEnemyDamaged);
    s.play();
}

void SoundManager::PlayBGM()
{
    if (!_bgmReady) return;
    if (_bgm.getStatus() != sf::Music::Status::Playing)
        _bgm.play();
}

void SoundManager::StopBGM()
{
    _bgm.stop();
}

void SoundManager::PauseBGM()
{
    if (_bgm.getStatus() == sf::Music::Status::Playing)
        _bgm.pause();
}

void SoundManager::ResumeBGM()
{
    if (_bgm.getStatus() == sf::Music::Status::Paused)
        _bgm.play();
}

void SoundManager::SetBGMVolume(float volume)
{
    _bgm.setVolume(volume * 100.f);
}
