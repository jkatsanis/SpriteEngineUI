#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Time.h"

namespace s2d
{
	class Sprite;
	class Animation
	{
	private:
		std::vector<sf::Texture> m_textures;
		std::vector<std::string> m_paths;
		bool m_useBaseSprite;
        std::string m_basePath;
		bool m_usebaseNextFrame;

		void setVectorSizes(const std::vector<std::string>& textures);

	public:
		std::string name;
		int currentFrame;
		float delay;
		float timePassed;

		bool isPlaying;

		Sprite* ptr_appliedSprite;

		Animation(Sprite* ptr_appliedSprite, std::string name, std::vector<std::string>, float delay, bool useBaseSprite);

		void play();
		void update();
		void stop();

		int getFrameSize() { return int(this->m_paths.size()); }

	public:
		static void updateAllAnimations();
	};
}

