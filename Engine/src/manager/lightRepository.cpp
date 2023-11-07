#include "lightRepository.h"
#include <physicalComponents/sprite.h>


// Public methods

void s2d::LightRepository::updateSprite(s2d::Sprite* sprite, bool call_by_update)
{

	if (!sprite->light.exist)
	{
		return;
	}

	uint32_t idx = sprite->light.getLightIndex();
	s2d::LightSource& source = s2d::LightRepository::s_m_light_sources[idx];

	if (sprite->transform.position_changed || call_by_update)
	{
		float zoom = s2d::Camera::zoom - 1;

		const float a = ((sprite->transform.position.y * -1) + 540) + 540 * zoom;
	    s2d::Vector2 new_pos = s2d::Vector2((sprite->transform.position.x + 960) + 960 * zoom, a);

		source.position = new_pos;
	}
	if (sprite->light.hasRadiusChanged() || call_by_update)
	{
		sprite->light.setRadiosChangeFlagFalse();
		source.radius = sprite->light.getRadius();
	}
	if (sprite->light.hasIntensityChanged() || call_by_update)
	{
		sprite->light.setIntensityChangeFlagFalse();
		source.light_intensities = sprite->light.getIntensity();
	}

	s2d::LightRepository::s_update_next = false;
	s2d::LightRepository::s_m_update = true;
	s2d::LightRepository::updateArrays();
}

void s2d::LightRepository::init()
{
	if (!LightRepository::s_m_light_shader.loadFromFile("ressources/shaders/circulaer_gradient.frag", sf::Shader::Fragment))
	{
		std::cout << "LOG [ERROR] Could not load light shader" << std::endl;
	}
	LightRepository::s_m_index = 0;
}

void s2d::LightRepository::add(const s2d::Vector2& pos, float radius, float intensiti, const s2d::Vector3& color)
{
	float zoom = s2d::Camera::zoom - 1;

	const float a = ((pos.y * -1) + 540) + 540 * zoom;
	s2d::Vector2 new_pos = s2d::Vector2((pos.x + 960) + 960 * zoom, a);

	LightRepository::s_m_light_sources[LightRepository::s_m_index] = s2d::LightSource(new_pos, radius, intensiti, color);
	s2d::LightRepository::s_m_update = true;
	s2d::LightRepository::updateArrays();
}

void s2d::LightRepository::remove(uint32_t index)
{
	LightRepository::s_m_light_sources.erase(index);
	s2d::LightRepository::s_m_update = true;
	LightRepository::updateArrays();	
}

void s2d::LightRepository::moveLightSource(uint32_t idx, const s2d::Vector2& pos)
{
	auto it = LightRepository::s_m_light_sources.find(idx);

	if (it != LightRepository::s_m_light_sources.end())
	{
		it->second.position = pos;

		sf::Vector2f* lightPositions = LightRepository::getPositionArray();

		LightRepository::s_m_light_shader.setUniformArray("lightPositions", lightPositions, LightRepository::s_m_light_sources.size());

		delete[] lightPositions;
	}
	else 
	{
		std::cout << "LOG [ERROR] Key does not exist " << idx << std::endl;
	}
}


// Private functions


void s2d::LightRepository::updateArrays()
{
	if (!s2d::LightRepository::s_m_update)
	{
		return;
	}
	s2d::LightRepository::s_m_update = false;
    size_t size = LightRepository::s_m_light_sources.size();

	if (size == 0)
	{
		size++;
	}

	sf::Vector2f* lightPositions = new sf::Vector2f[size];
	float* lightRadii = new float[size];
	float* lightIntensities = new float[size];
	sf::Vector3f* lightColors = new sf::Vector3f[size];

	lightPositions[0] = sf::Vector2f(0, 0);

	lightRadii[0] = 0;
	lightIntensities[0] = 0;

	lightColors[0] = sf::Vector3f(0, 0, 0);

 
	size_t i = 0;
	for (const auto& pair : LightRepository::s_m_light_sources)
	{
		const LightSource& source = pair.second;
		lightPositions[i] = s2d::Vector2::toImVec2(source.position);
		lightRadii[i] = source.radius;
		lightIntensities[i] = source.light_intensities;
		lightColors[i] = s2d::Vector3::toSFVector3(source.color);
		i++;
	}

	LightRepository::s_m_light_shader.setUniform("lightAmount", (int)size);
	LightRepository::s_m_light_shader.setUniformArray("lightPositions", lightPositions, size);
	LightRepository::s_m_light_shader.setUniformArray("lightRadii", lightRadii, size);
	LightRepository::s_m_light_shader.setUniformArray("lightIntensities", lightIntensities, size);
	LightRepository::s_m_light_shader.setUniformArray("lightColors", lightColors, size);

	LightRepository::s_m_index = s2d::LightRepository::s_m_light_sources.size();

	delete[] lightPositions;
	delete[] lightRadii;
	delete[] lightIntensities;
	delete[] lightColors;

}

sf::Vector2f* s2d::LightRepository::getPositionArray()
{
	const size_t size = LightRepository::s_m_light_sources.size();
	sf::Vector2f* lightPositions = new sf::Vector2f[size];

	size_t i = 0;
	for (const auto& pair : LightRepository::s_m_light_sources)
	{
		lightPositions[i] = s2d::Vector2::toSFVector(pair.second.position);
		i++;	
	}

	return lightPositions;
}


// Public static vars

std::unordered_map<uint32_t, s2d::LightSource> s2d::LightRepository::s_m_light_sources;
sf::Shader s2d::LightRepository::s_m_light_shader;
uint32_t s2d::LightRepository::s_m_index = 0;
bool s2d::LightRepository::s_m_update = true;
bool s2d::LightRepository::s_update_next = true;
