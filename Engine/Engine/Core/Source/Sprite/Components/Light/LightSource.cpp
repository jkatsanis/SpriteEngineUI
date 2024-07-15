#include "lightSource.h"

spe::LightSource::LightSource() : Position(), Radius(0.0f), LightIntensity(0.0f), Color() {
}

spe::LightSource::LightSource(const spe::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color)
    : Position(pos), Radius(radius), LightIntensity(intensiti), Color(color), Process(false) {
}
