#include "LightRepository.h"

#include "Sprite.h"
#include "Camera/Camera.h"


// Public methods

void spe::LightRepository::updateLightSource(spe::Sprite* sprite, spe::Camera* cam)
{
	if (!sprite->light.exist)
	{
		return;
	}

	uint32_t idx = sprite->light.getLightIndex();
	spe::LightSource& source = spe::LightRepository::s_m_light_sources[idx];

	if (sprite->transform.position_changed || cam->hasZoomChanged())
	{
		spe::LightRepository::s_m_update = true;

		float zoom = cam->getZoom() - 1;

		const float a = ((sprite->transform.getPosition().y * -1) + 540) + 540 * zoom;
		spe::Vector2 new_pos = spe::Vector2((sprite->transform.getPosition().x + 960) + 960 * zoom, a);

		source.position = new_pos;
	}
	if (sprite->light.hasRadiusChanged())
	{
		spe::LightRepository::s_m_update = true;

		sprite->light.setRadiosChangeFlagFalse();
		source.radius = sprite->light.getRadius();
	}
	if (sprite->light.hasIntensityChanged())
	{
		spe::LightRepository::s_m_update = true;

		sprite->light.setIntensityChangeFlagFalse();
		source.light_intensities = sprite->light.getIntensity();
	}
	if (sprite->light.hasColorChanged())
	{
		spe::LightRepository::s_m_update = true;

		sprite->light.setColorChangeFlag();
		source.color = sprite->light.getColor();
	}
}

void spe::LightRepository::updateSprite(spe::Sprite* sprite, spe::Camera* cam)
{
	if (!sprite->light.exist)
	{
		return;
	}
	spe::LightRepository::updateLightSource(sprite, cam);
	spe::LightRepository::updateArrays();
}

void spe::LightRepository::init()
{
	if (!LightRepository::s_m_light_shader.loadFromFile("ressources/shaders/circulaer_gradient.frag", sf::Shader::Fragment))
	{
		std::cout << "LOG [ERROR] Could not load light shader" << std::endl;
	}
	LightRepository::s_m_index = 0;
}

void spe::LightRepository::add(const spe::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color)
{
	float zoom = 1;

	const float a = ((pos.y * -1) + 540) + 540 * zoom;
	spe::Vector2 new_pos = spe::Vector2((pos.x + 960) + 960 * zoom, a);

	LightRepository::s_m_index++;
	LightRepository::s_m_light_sources[LightRepository::s_m_index] = spe::LightSource(new_pos, radius, intensiti, color);

	spe::LightRepository::s_m_update = true;
	spe::LightRepository::updateArrays();
}

void spe::LightRepository::remove(uint32_t index)
{
	LightRepository::s_m_light_sources.erase(index);

	spe::LightRepository::s_m_update = true;
	LightRepository::updateArrays();
}

void spe::LightRepository::moveLightSource(uint32_t idx, const spe::Vector2& pos)
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

void spe::LightRepository::updateArrays()
{
	if (!spe::LightRepository::s_m_update)
	{
		return;
	}
	spe::LightRepository::s_m_update = false;
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
		lightPositions[i] = spe::Vector2::toImVec2(source.position);
		lightRadii[i] = source.radius;
		lightIntensities[i] = source.light_intensities;
		lightColors[i] = sf::Vector3f(source.color.x, source.color.y, source.color.z);
		i++;
	}

	LightRepository::s_m_light_shader.setUniform("lightAmount", (int)size);
	LightRepository::s_m_light_shader.setUniformArray("lightPositions", lightPositions, size);
	LightRepository::s_m_light_shader.setUniformArray("lightRadii", lightRadii, size);
	LightRepository::s_m_light_shader.setUniformArray("lightIntensities", lightIntensities, size);
	LightRepository::s_m_light_shader.setUniformArray("lightColors", lightColors, size);

	delete[] lightPositions;
	delete[] lightRadii;
	delete[] lightIntensities;
	delete[] lightColors;

}

// Private functions

sf::Vector2f* spe::LightRepository::getPositionArray()
{
	const size_t size = LightRepository::s_m_light_sources.size();
	sf::Vector2f* lightPositions = new sf::Vector2f[size];

	size_t i = 0;
	for (const auto& pair : LightRepository::s_m_light_sources)
	{
		lightPositions[i] = spe::Vector2::toSFVector(pair.second.position);
		i++;
	}

	return lightPositions;
}


// Public static vars

std::unordered_map<uint32_t, spe::LightSource> spe::LightRepository::s_m_light_sources;
sf::Shader spe::LightRepository::s_m_light_shader;
uint32_t spe::LightRepository::s_m_index = 0;
bool spe::LightRepository::s_m_update = true;
