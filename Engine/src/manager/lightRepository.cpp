#include "lightRepository.h"


// Public methods

void s2d::LightRepository::init()
{
	if (!LightRepository::s_m_light_shader.loadFromFile("ressources/shaders/circulaer_gradient.frag", sf::Shader::Fragment))
	{
		std::cout << "LOG [ERROR] Could not load light shader" << std::endl;
	}
	LightRepository::s_m_index = 0;
}

void s2d::LightRepository::add(const s2d::Vector2& pos, float radius, float intensiti, const s2d::Vector3& color, const std::string& name)
{
	const s2d::Vector2 new_pos = s2d::Vector2(pos.x + 960, pos.y + 540);
	LightRepository::s_m_light_sources[name] = s2d::LightSource(new_pos, radius, intensiti, color);

	const size_t size = LightRepository::s_m_light_sources.size();

	sf::Vector2f* lightPositions = new sf::Vector2f[size];
	float* lightRadii = new float[size];
	float* lightIntensities = new float[size];
	sf::Vector3f* lightColors = new sf::Vector3f[size];


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
	LightRepository::s_m_light_shader.setUniformArray("lightPositions", lightPositions, LightRepository::s_m_light_sources.size());

	LightRepository::s_m_light_shader.setUniformArray("lightPositions", lightPositions, size);
	LightRepository::s_m_light_shader.setUniformArray("lightRadii", lightRadii, size);
	LightRepository::s_m_light_shader.setUniformArray("lightIntensities", lightIntensities, size);
	LightRepository::s_m_light_shader.setUniformArray("lightColors", lightColors, size);

	LightRepository::s_m_index++;

	delete[] lightPositions;
	delete[] lightRadii;
	delete[] lightIntensities;
	delete[] lightColors;
}

void s2d::LightRepository::moveLightSource(const std::string& source, const s2d::Vector2& pos)
{
	auto it = LightRepository::s_m_light_sources.find(source);

	if (it != LightRepository::s_m_light_sources.end())
	{
		it->second.position = pos;

		sf::Vector2f* lightPositions = LightRepository::getPositionArray();

		LightRepository::s_m_light_shader.setUniformArray("lightPositions", lightPositions, LightRepository::s_m_light_sources.size());

		delete[] lightPositions;
	}
	else 
	{
		std::cout << "LOG [ERROR] Key does not exist " << source << std::endl;
	}
}


// Private functions


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

std::unordered_map<std::string, s2d::LightSource> s2d::LightRepository::s_m_light_sources;
sf::Shader s2d::LightRepository::s_m_light_shader;
uint32_t s2d::LightRepository::s_m_index = 0;
