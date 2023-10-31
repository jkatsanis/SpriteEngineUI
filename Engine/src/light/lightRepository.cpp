#include "lightRepository.h"

// Constructor

s2d::LightRepository::LightRepository()
{
	if (!this->m_light_shader.loadFromFile("ressources/shaders/circulaer_gradient.frag", sf::Shader::Fragment))
	{
		std::cout << "LOG [ERROR] Could not load light shader" << std::endl;
	}
	this->m_index = 0;
}

// Public methods

void s2d::LightRepository::add(const s2d::Vector2& pos, float radius, float intensiti, const s2d::Vector3& color, const std::string& name)
{
	this->m_light_sources[name] = s2d::LightSource(pos, radius, intensiti, color);


	const size_t size = this->m_light_sources.size();

	sf::Vector2f* lightPositions = new sf::Vector2f[size];
	float* lightRadii = new float[size];
	float* lightIntensities = new float[size];
	sf::Vector3f* lightColors = new sf::Vector3f[size];


	size_t i = 0;
	for (const auto& pair : this->m_light_sources)
	{
		const LightSource& source = pair.second;
		lightPositions[i] = s2d::Vector2::toImVec2(source.position);
		lightRadii[i] = source.radius;
		lightIntensities[i] = source.light_intensities;
		lightColors[i] = s2d::Vector3::toSFVector3(source.color);
	}

	
	this->m_light_shader.setUniform("lightAmount", (int)size);
	this->m_light_shader.setUniformArray("lightPositions", lightPositions, size);
	this->m_light_shader.setUniformArray("lightRadii", lightRadii, size);
	this->m_light_shader.setUniformArray("lightIntensities", lightIntensities, size);
	this->m_light_shader.setUniformArray("lightColors", lightColors, size);


	delete[] lightPositions;
	delete[] lightRadii;
	delete[] lightIntensities;
	delete[] lightColors;
}

void s2d::LightRepository::moveLightSource(const std::string& source, const s2d::Vector2& pos)
{
	auto it = this->m_light_sources.find(source);

	if (it != this->m_light_sources.end()) 
	{
		it->second.position = pos;

		sf::Vector2f* lightPositions = this->getPositionArray();

		this->m_light_shader.setUniformArray("lightPositions", lightPositions, this->m_light_sources.size());

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
	const size_t size = this->m_light_sources.size(); 
	sf::Vector2f* lightPositions = new sf::Vector2f[size];

	size_t i = 0;
	for (const auto& pair : this->m_light_sources)
	{
		lightPositions[i] = s2d::Vector2::toSFVector(pair.second.position);
		i++;	
	}

	return lightPositions;
}