#pragma once

#include <vector>
#include <physicalComponents/lightSource.h>
#include <manager/spriteRepository.h>
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace s2d
{
	class Camera;
	class Sprite;
	class LightRepository
	{
	private:
		static std::unordered_map<uint32_t, s2d::LightSource> s_m_light_sources;
		static sf::Shader s_m_light_shader;	// Only 1 shader, (circle-ish)
		static uint32_t s_m_index;
		static bool s_m_update;

		static sf::Vector2f* getPositionArray();

	public:
		LightRepository() = delete;
		static void updateArrays();

		static void updateLightSource(s2d::Sprite* sprite, s2d::Camera* cam);
		static void updateSprite(s2d::Sprite* sprite, s2d::Camera* cam);

		static void init();
		static void add(const s2d::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color);
		static void remove(uint32_t index);

		static void moveLightSource(uint32_t idx, const s2d::Vector2& pos);

		static uint32_t getIndex() { return LightRepository::s_m_index; }
		static sf::Shader& getShader() { return LightRepository::s_m_light_shader; }
	};
}

