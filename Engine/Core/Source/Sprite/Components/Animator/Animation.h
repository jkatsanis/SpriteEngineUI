#pragma once

#include <iostream>
#include <Time.h>

#include <SFML/Graphics.hpp>

#include "UtilityFunctions.h"
#include "Core/EngineData.h"
#include "Core/Time.h"
#include "KeyFrame.h"
#include "Sprite/SpriteRepository.h"

namespace spe
{
	class Sprite;
	class Animation
	{
	private:
		std::vector<sf::Texture> m_Textures;
		std::vector<spe::KeyFrame> m_Keyframes;
		std::string m_PathToFile;
		std::string m_Name;
		std::string m_BasePath;

	public:
		int CurrentFrame;
		float TimePassed;
		float TotalTimePassed;
		float TotalFramePassed;
		bool Loop;

		bool IsPlaying;

	    Sprite* ptr_AppliedSprite;

		Animation();
		Animation(Sprite* ptr_AppliedSprite, const std::string& Name, const std::string FileLocation, const std::vector<spe::KeyFrame>& Frames);
		Animation(spe::Sprite* ptr_AppliedSprite, const spe::Animation& Animation);
		Animation(spe::Sprite* ptr_AppliedSprite, const spe::Animation& Animation, const std::string& Name);

		void InitCopyCtor(const spe::Animation& Animation);
		void RealoadTextures();
		void DeleteKeyFrame(const int Pos);
		void Play();
		void Update();
		void Stop();

		spe::KeyFrame& GetKeyFrameAtMs(const float ms);
		int GetSize() const { return (int)this->m_Keyframes.size(); }
		int GetFrameSize() { return int(this->m_Keyframes.size()); }
		const std::vector<spe::KeyFrame> GetkeyFrames() const { return this->m_Keyframes; }
		float GetAnimationTime() const;
		float GetTimeTillFrame(size_t Frame);
		const std::string& GetName() const { return this->m_Name; }

		/// <summary>
		/// THIS METHOD DOES NOT SET THE KEYFRAME POSITION!!!
		/// </summary>
		/// <param name="vecpos"></param>
		/// <param name="frame"></param>
		void AddKeyFrameAt(const int Vecpos, const spe::KeyFrame& Frame);
	};
}

