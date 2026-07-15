#pragma once
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Music.hpp>
#include <array>
#include <string>

//designed with AI assistance

inline bool LoadTextureFromResource(sf::Texture& texture, const std::string& filename)
{
	constexpr std::array<const char*, 3> prefixes = { "resource/", "../resource/", "../../resource/" };
	for (const char* prefix : prefixes)
	{
		if (texture.loadFromFile(std::string(prefix) + filename))
			return true;
	}
	return false;
}

inline bool LoadSoundBufferFromResource(sf::SoundBuffer& buffer, const std::string& filename)
{
	constexpr std::array<const char*, 3> prefixes = { "resource/", "../resource/", "../../resource/" };
	for (const char* prefix : prefixes)
	{
		if (buffer.loadFromFile(std::string(prefix) + filename))
			return true;
	}
	return false;
}

inline bool LoadMusicFromResource(sf::Music& music, const std::string& filename)
{
	constexpr std::array<const char*, 3> prefixes = { "resource/", "../resource/", "../../resource/" };
	for (const char* prefix : prefixes)
	{
		if (music.openFromFile(std::string(prefix) + filename))
			return true;
	}
	return false;
}
