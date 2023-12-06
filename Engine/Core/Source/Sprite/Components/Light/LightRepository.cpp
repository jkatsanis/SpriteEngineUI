#include "LightRepository.h"

#include "Sprite.h"
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
        this->m_Update = true;
        this->m_Index = 0;
        if (!m_LightShader.loadFromFile(shader, sf::Shader::Fragment))
        {
            spe::Log::LogString("Could not load light shader");
        }
    }


    void LightRepository::UpdateLightSource(spe::Sprite* sprite, spe::Camera* cam)
    {
        if (!sprite->Light.Exist)
        {
            return;
        }

        uint32_t idx = sprite->Light.GetLightIndex();
        spe::LightSource& source = m_LightSources[idx];

        if (sprite->Transform.PositionChanged || cam->HasZoomChanged())
        {
            sprite->Transform.PositionChanged = false;
            m_Update = true;

            float zoom = cam->GetZoom() - 1;

            const float a = ((sprite->Transform.GetPosition().Y * -1) + 540) + 540 * zoom;
            spe::Vector2 new_pos = spe::Vector2((sprite->Transform.GetPosition().X + 960) + 960 * zoom, a);

            source.Position = new_pos;
        }
        if (sprite->Light.HasRadiusChanged())
        {
            m_Update = true;

            sprite->Light.SetRadiosChangeFlagFalse();
            source.Radius = sprite->Light.GetRadius();
        }
        if (sprite->Light.HasIntensityChanged())
        {
            m_Update = true;

            sprite->Light.SetIntensityChangeFlagFalse();
            source.LightIntensity = sprite->Light.GetIntensity();
        }
        if (sprite->Light.HasColorChanged())
        {
            m_Update = true;

            sprite->Light.SetColorChangeFlag();
            source.Color = sprite->Light.GetColor();
        }
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
        float zoom = 1;

        const float a = ((pos.Y * -1) + 540) + 540 * zoom;
        spe::Vector2 new_pos = spe::Vector2((pos.X + 960) + 960 * zoom, a);

        m_Index++;
        m_LightSources[m_Index] = spe::LightSource(new_pos, radius, intensity, color);

        m_Update = true;
        UpdateArrays();
    }

    void LightRepository::Remove(uint32_t index)
    {
        m_LightSources.erase(index);

        m_Update = true;
        UpdateArrays();
    }

    void LightRepository::MoveLightSource(uint32_t idx, const spe::Vector2& pos)
    {
        auto it = m_LightSources.find(idx);

        if (it != m_LightSources.end())
        {
            it->second.Position = pos;

            sf::Vector2f* lightPositions = GetPositionArray();

            m_LightShader.setUniformArray("lightPositions", lightPositions, m_LightSources.size());

            delete[] lightPositions;
        }
        else
        {
            std::cout << "LOG [ERROR] Key does not exist " << idx << std::endl;
        }
    }

    void LightRepository::UpdateArrays()
    {
        if (!m_Update)
        {
            return;
        }
        m_Update = false;
        size_t size = m_LightSources.size();

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
        for (const auto& pair : m_LightSources)
        {
            const LightSource& source = pair.second;
            lightPositions[i] = spe::Vector2::toImVec2(source.Position);
            lightRadii[i] = source.Radius;
            lightIntensities[i] = source.LightIntensity;
            lightColors[i] = sf::Vector3f(source.Color.x, source.Color.y, source.Color.z);
            i++;
        }

        m_LightShader.setUniform("lightAmount", (int)size);
        m_LightShader.setUniformArray("lightPositions", lightPositions, size);
        m_LightShader.setUniformArray("lightRadii", lightRadii, size);
        m_LightShader.setUniformArray("lightIntensities", lightIntensities, size);
        m_LightShader.setUniformArray("lightColors", lightColors, size);

        delete[] lightPositions;
        delete[] lightRadii;
        delete[] lightIntensities;
        delete[] lightColors;
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
