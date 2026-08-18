#include "LightRepository.h"

#include "Sprite/Sprite.h"
#include "Camera/Camera.h"

namespace spe
{
    // Ctor
    LightRepository::LightRepository()
    {
        this->m_Index = 0;
        this->m_Update = false;
    }

    // Public

    void LightRepository::Init(const std::string& shader)
    {
        const std::string updatetePath = spe::Utility::ToRightPath(shader);
        this->m_Update = true;
        this->m_Index = 0;
        if (!m_LightShader.loadFromFile(updatetePath, sf::Shader::Fragment))
        {
            spe::Log::LogString("Could not load light shader");
        }
    }


    void LightRepository::UpdateLightSource(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.Exist || !sprite->Process)
        {
            return;
        }

        m_Update = true;

        uint32_t idx = sprite->Light.GetLightIndex();
        spe::LightSource& source = m_LightSources[idx];

        // Update position if changed (Just use World Position)
        if (sprite->Transform.PositionChanged || cam->HasZoomChanged() || sprite->Light.HasUpdate())
        {
            sprite->Transform.PositionChanged = false;
            source.Position = sprite->Transform.GetPosition();
        }
        if (sprite->Light.HasRadiusChanged())
        {
            source.Radius = sprite->Light.GetRadius();
        }
        if (sprite->Light.HasIntensityChanged())
        {
            source.LightIntensity = sprite->Light.GetIntensity();

        }
        if (sprite->Light.HasColorChanged())
        {
            source.Color = sprite->Light.GetColor();
        }

        sprite->Light.DisableFlags();
    }

    void LightRepository::UpdateSprite(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.Exist)
        {
            return;
        }
        UpdateLightSource(sprite, cam);
        UpdateArrays();
    }

    void LightRepository::Add(const spe::Vector2& pos, float radius, float intensity, const sf::Vector3f& color)
    {
        // Just use World Position
        m_Index++;
        m_LightSources[m_Index] = spe::LightSource(pos, radius, intensity, color);

        m_Update = true;
        UpdateArrays();
    }

    void LightRepository::Remove(uint32_t index)
    {
        m_LightSources.erase(index);

        m_Update = true;
        UpdateArrays();
    }

    void LightRepository::UpdateArrays()
    {
        if (!this->m_Update)
        {
            return;
        }

        m_Update = false;
        size_t size = 0;

        std::vector<sf::Vector2f> lightPositions(size);
        std::vector<float> lightRadii(size);
        std::vector<float> lightIntensities(size);
        std::vector<sf::Vector3f> lightColors(size);


        for (auto& pair : m_LightSources)
        {
            LightSource& source = pair.second;
            if (source.Process)
            {
                lightPositions.push_back(spe::Vector2::toSFVector(source.Position));
                lightRadii.push_back(source.Radius);
                lightIntensities.push_back(source.LightIntensity);
                lightColors.push_back(sf::Vector3f(source.Color.x, source.Color.y, source.Color.z));
                size++;
            }
        }

        if (size > 0)
        {
            m_LightShader.setUniform("lightAmount", static_cast<int>(size));
            m_LightShader.setUniformArray("lightPositions", lightPositions.data(), size);
            m_LightShader.setUniformArray("lightRadii", lightRadii.data(), size);
            m_LightShader.setUniformArray("lightIntensities", lightIntensities.data(), size);
            m_LightShader.setUniformArray("lightColors", lightColors.data(), size);
        }
  
    }

    sf::Vector2f* LightRepository::GetPositionArray()
    {
        const size_t size = m_LightSources.size();
        sf::Vector2f* lightPositions = new sf::Vector2f[size];

        size_t i = 0;
        for (const auto& pair : m_LightSources)
        {
            lightPositions[i] = spe::Vector2::toSFVector(pair.second.Position);
            i++;
        }

        return lightPositions;
    }
}
