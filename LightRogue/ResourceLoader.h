#pragma once

#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <stdexcept>
#include <string>

inline void LoadTextureFromResource(sf::Texture& texture, const std::string& filename)
{
	constexpr std::array<const char*, 3> prefixes = { "resource/", "../resource/", "../../resource/" };
	for (const char* prefix : prefixes)
	{
		if (texture.loadFromFile(std::string(prefix) + filename))
			return;
	}

	throw std::runtime_error("Failed to load texture: " + filename);
}
