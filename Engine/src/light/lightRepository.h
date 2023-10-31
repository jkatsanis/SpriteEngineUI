#pragma once

#include <vector>
#include <light/lightSource.h>
#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace s2d
{
	class LightRepository
	{
	private:
		std::unordered_map<std::string, s2d::LightSource> m_light_sources;

		sf::Shader m_light_shader;	// Only 1 shader, (circle-ish)


		uint32_t m_index;

		sf::Vector2f* getPositionArray();

	public:
		LightRepository();

		void add(const s2d::Vector2& pos, float radius, float intensiti, const s2d::Vector3& color, const std::string& name);
			
		/// <summary>
		/// Very ineffiecient. Get the source and then update it with the refernce
		/// </summary>
		/// <param name="source"></param>
		/// <param name="pos"></param>
		void moveLightSource(const std::string& source, const s2d::Vector2& pos);

		const sf::Shader& getShader() const { return this->m_light_shader; }
	};
}

