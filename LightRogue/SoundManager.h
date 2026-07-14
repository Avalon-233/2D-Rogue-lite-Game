#pragma once
#include<SFML/Audio.hpp>
#include <memory>

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void LoadAll();
	bool IsReady()const;

	void PlayLevelUp();
	void PlayPlayerShoot();
	void PlayPlayerDamaged();
	void PlayEnemyDamaged();

	void PlayBGM();
	void StopBGM();
	void PauseBGM();
	void ResumeBGM();
	void SetBGMVolume(float volume);

private:
	sf::Sound& GetFreeSound();

	static constexpr int PoolSize = 4;
	std::unique_ptr<sf::Sound> _pool[PoolSize];
	int _nextIndex = 0;
	sf::SoundBuffer _bufLevelUp;
	sf::SoundBuffer _bufPlayerShoot;
	sf::SoundBuffer _bufPlayerDamaged;
	sf::SoundBuffer _bufEnemyDamaged;
	bool _sfxReady = false;

	sf::Music _bgm;
	bool _bgmReady = false;
};