#pragma once

#include <vector>
#include <unordered_map>

#include <Include/SFML/Graphics.hpp>

#include "Sprite/Components/Light/LightSource.h"

namespace spe
{
	class Camera;
	class Sprite;
	class LightRepository
	{
	private:
		static std::unordered_map<uint32_t, spe::LightSource> s_m_light_sources;
		static sf::Shader s_m_light_shader;	// Only 1 shader, (circle-ish)
		static uint32_t s_m_index;
		static bool s_m_update;

		static sf::Vector2f* getPositionArray();

	public:
		LightRepository() = delete;
		static void updateArrays();

		static void updateLightSource(spe::Sprite* sprite, spe::Camera* cam);
		static void updateSprite(spe::Sprite* sprite, spe::Camera* cam);

		static void init();
		static void add(const spe::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color);
		static void remove(uint32_t index);

		static void moveLightSource(uint32_t idx, const spe::Vector2& pos);

		static uint32_t getIndex() { return LightRepository::s_m_index; }
		static sf::Shader& getShader() { return LightRepository::s_m_light_shader; }
	};
}

