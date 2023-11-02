#pragma once

#include <vector>
#include <physicalComponents/lightSource.h>
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace s2d
{
	class LightRepository
	{
	private:
		static std::unordered_map<std::string, s2d::LightSource> s_m_light_sources;
		static sf::Shader s_m_light_shader;	// Only 1 shader, (circle-ish)
		static uint32_t s_m_index;

		static sf::Vector2f* getPositionArray();

	public:
		LightRepository() = delete;

		static void init();
		static void add(const s2d::Vector2& pos, float radius, float intensiti, const s2d::Vector3& color, const std::string& name);
			
		/// <summary>
		/// Very ineffiecient. Get the source and then update it with the refernce
		/// </summary>
		/// <param name="source"></param>
		/// <param name="pos"></param>
		static void moveLightSource(const std::string& source, const s2d::Vector2& pos);

		static uint32_t getIndex() { return LightRepository::s_m_index; }
		static sf::Shader& getShader() { return LightRepository::s_m_light_shader; }
	};
}

