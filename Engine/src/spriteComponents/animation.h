#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Time.h"
#include <spriteComponents/keyFrame.h>
#include <manager/spriteRepository.h>

namespace s2d
{
	class Sprite;
	class Animation
	{
	private:
		std::vector<sf::Texture> m_textures;
		std::vector<s2d::KeyFrame> m_keyframes;
		std::string m_path_to_file;
		bool m_saved_already;

		/// <summary>
		/// Gets set in the play function and gets resettet in the s
		/// stop function
		/// </summary>
		std::string m_base_path;

		void setVectorSizes();

	public:
		std::string name;
		int current_frame;
		float time_passed;
		float total_time_passed;
		float total_frame_passed;

		bool is_playing;

	    Sprite* ptr_applied_sprite;

		Animation();
		Animation(Sprite* ptr_applied_sprite, const std::string& name, const std::string file_location, const std::vector<s2d::KeyFrame>& frames);

		void deleteKeyFrame(const int pos);
		void play();
		void update();
		void stop();
		int getSize() const { return (int)this->m_keyframes.size(); }
		const std::vector<s2d::KeyFrame>& getKeyFrames() const { return this->m_keyframes; }
		std::vector<s2d::KeyFrame>& getKeyFrames() { return this->m_keyframes; }
		
		s2d::KeyFrame& getKeyFrameAtMs(const float ms);
		int getFrameSize() { return int(this->m_keyframes.size()); }

		const std::string& getPathToFile() const { return this->m_path_to_file; }

	    const std::string getEnginePathToFile() const;

		float getAnimationTime() const;

		float getTimeTillFrame(size_t frame);

		/// <summary>
		/// THIS METHOD DOES NOT SET THE KEYFRAME POSITION!!!
		/// </summary>
		/// <param name="vecpos"></param>
		/// <param name="frame"></param>
		void addKeyFrameAt(const int vecpos, const s2d::KeyFrame& frame);

		bool removeOnClose() const { return !this->m_saved_already; }
		void setFlagToNotDeleteAfterExit() { this->m_saved_already = true; }

	public:
		static void updateAllAnimations(s2d::SpriteRepository& repo);
	};
}

