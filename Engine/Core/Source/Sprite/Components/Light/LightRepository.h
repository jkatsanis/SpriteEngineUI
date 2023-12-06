#pragma once

#include <vector>
#include <unordered_map>
#include <Include/SFML/Graphics.hpp>

#include "Sprite/Components/Light/LightSource.h"
#include "RessourceHandler/FileDataMacros.h"

namespace spe
{
    class Camera;
    class Sprite;
    class LightRepository 
    {
    private:
        std::unordered_map<uint32_t, spe::LightSource> m_LightSources;
        sf::Shader m_LightShader; // Only 1 shader, (circle-ish)
        uint32_t m_Index;
        bool m_Update;

        sf::Vector2f* GetPositionArray();

    public:
        LightRepository();

        void Init(const std::string& shader);
        void UpdateArrays();

        void UpdateLightSource(spe::Sprite* sprite, spe::Camera* cam);
        void UpdateSprite(spe::Sprite* sprite, spe::Camera* cam);

        void Add(const spe::Vector2& pos, float radius, float intensity, const sf::Vector3f& color);
        void Remove(uint32_t index);

        void MoveLightSource(uint32_t idx, const spe::Vector2& pos);

        uint32_t GetIndex() const noexcept{ return m_Index; }
        sf::Shader& GetShader() noexcept { return m_LightShader; }
    };
}