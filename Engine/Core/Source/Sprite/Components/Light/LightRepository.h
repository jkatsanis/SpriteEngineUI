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
        std::unordered_map<uint32_t, spe::LightSource> m_light_sources;
        sf::Shader m_light_shader; // Only 1 shader, (circle-ish)
        uint32_t m_index;
        bool m_update;

        sf::Vector2f* getPositionArray();

    public:
        void Init(const std::string& shader);
        void updateArrays();

        void updateLightSource(spe::Sprite* sprite, spe::Camera* cam);
        void updateSprite(spe::Sprite* sprite, spe::Camera* cam);

        void add(const spe::Vector2& pos, float radius, float intensity, const sf::Vector3f& color);
        void remove(uint32_t index);

        void moveLightSource(uint32_t idx, const spe::Vector2& pos);

        uint32_t getIndex() const { return m_index; }
        sf::Shader& getShader() { return m_light_shader; }
    };
}