#include "LightRepository.h"

#include "Sprite.h"
#include "Camera/Camera.h"

namespace spe
{
    void LightRepository::Init(const std::string& shader)
    {
        this->m_update = true;
        this->m_index = 0;
        if (!m_light_shader.loadFromFile(shader, sf::Shader::Fragment))
        {
            spe::Log::LogString("Could not load light shader");
        }
    }


    void LightRepository::updateLightSource(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.exist)
        {
            return;
        }

        uint32_t idx = sprite->Light.getLightIndex();
        spe::LightSource& source = m_light_sources[idx];

        if (sprite->Transform.position_changed || cam->HasZoomChanged())
        {
            sprite->Transform.position_changed = false;
            m_update = true;

            float zoom = cam->GetZoom() - 1;

            const float a = ((sprite->Transform.GetPosition().Y * -1) + 540) + 540 * zoom;
            spe::Vector2 new_pos = spe::Vector2((sprite->Transform.GetPosition().X + 960) + 960 * zoom, a);

            source.position = new_pos;
        }
        if (sprite->Light.hasRadiusChanged())
        {
            m_update = true;

            sprite->Light.setRadiosChangeFlagFalse();
            source.radius = sprite->Light.getRadius();
        }
        if (sprite->Light.hasIntensityChanged())
        {
            m_update = true;

            sprite->Light.setIntensityChangeFlagFalse();
            source.light_intensities = sprite->Light.getIntensity();
        }
        if (sprite->Light.hasColorChanged())
        {
            m_update = true;

            sprite->Light.setColorChangeFlag();
            source.color = sprite->Light.getColor();
        }
    }

    void LightRepository::updateSprite(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.exist)
        {
            return;
        }
        updateLightSource(sprite, cam);
        updateArrays();
    }

    void LightRepository::add(const spe::Vector2& pos, float radius, float intensity, const sf::Vector3f& color)
    {
        float zoom = 1;

        const float a = ((pos.Y * -1) + 540) + 540 * zoom;
        spe::Vector2 new_pos = spe::Vector2((pos.X + 960) + 960 * zoom, a);

        m_index++;
        m_light_sources[m_index] = spe::LightSource(new_pos, radius, intensity, color);

        m_update = true;
        updateArrays();
    }

    void LightRepository::remove(uint32_t index)
    {
        m_light_sources.erase(index);

        m_update = true;
        updateArrays();
    }

    void LightRepository::moveLightSource(uint32_t idx, const spe::Vector2& pos)
    {
        auto it = m_light_sources.find(idx);

        if (it != m_light_sources.end())
        {
            it->second.position = pos;

            sf::Vector2f* lightPositions = getPositionArray();

            m_light_shader.setUniformArray("lightPositions", lightPositions, m_light_sources.size());

            delete[] lightPositions;
        }
        else
        {
            std::cout << "LOG [ERROR] Key does not exist " << idx << std::endl;
        }
    }

    void LightRepository::updateArrays()
    {
        if (!m_update)
        {
            return;
        }
        m_update = false;
        size_t size = m_light_sources.size();

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
        for (const auto& pair : m_light_sources)
        {
            const LightSource& source = pair.second;
            lightPositions[i] = spe::Vector2::toImVec2(source.position);
            lightRadii[i] = source.radius;
            lightIntensities[i] = source.light_intensities;
            lightColors[i] = sf::Vector3f(source.color.x, source.color.y, source.color.z);
            i++;
        }

        m_light_shader.setUniform("lightAmount", (int)size);
        m_light_shader.setUniformArray("lightPositions", lightPositions, size);
        m_light_shader.setUniformArray("lightRadii", lightRadii, size);
        m_light_shader.setUniformArray("lightIntensities", lightIntensities, size);
        m_light_shader.setUniformArray("lightColors", lightColors, size);

        delete[] lightPositions;
        delete[] lightRadii;
        delete[] lightIntensities;
        delete[] lightColors;
    }

    sf::Vector2f* LightRepository::getPositionArray()
    {
        const size_t size = m_light_sources.size();
        sf::Vector2f* lightPositions = new sf::Vector2f[size];

        size_t i = 0;
        for (const auto& pair : m_light_sources)
        {
            lightPositions[i] = spe::Vector2::toSFVector(pair.second.position);
            i++;
        }

        return lightPositions;
    }
}
