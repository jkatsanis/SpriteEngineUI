#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Time.h"
#include <spriteComponents/keyFrame.h>

namespace s2d
{
	class Sprite;
	class Animation
	{
	private:
		std::vector<sf::Texture> m_textures;
		std::vector<s2d::KeyFrame> m_keyFrames;
		bool m_useBaseSprite;
        std::string m_basePath;

		void setVectorSizes();

	public:
		std::string name;
		int currentFrame;
		float delay;
		float timePassed;

		bool isPlaying;

	    Sprite* ptr_appliedSprite;

		Animation(Sprite* ptr_appliedSprite, const std::string& name, const std::vector<s2d::KeyFrame>& frames, const float delay, const bool useBaseSprite);

		void deleteKeyFrame(const int pos);
		void play();
		void update();
		void stop();
		int getSize() const { return (int)this->m_keyFrames.size(); }
		const std::vector<s2d::KeyFrame>& getKeyFrames() const { return this->m_keyFrames; }
		std::vector<s2d::KeyFrame>& getKeyFrames() { return this->m_keyFrames; }
		
		s2d::KeyFrame& getKeyFrameAtMs(const float ms);
		int getFrameSize() { return int(this->m_keyFrames.size()); }


		/// <summary>
		/// THIS METHOD DOES NOT SET THE KEYFRAME POSITION!!!
		/// </summary>
		/// <param name="vecpos"></param>
		/// <param name="frame"></param>
		void addKeyFrameAt(const int vecpos, const s2d::KeyFrame& frame);

	public:
		static void updateAllAnimations();
	};
}

