#include "lightSource.h"

spe::LightSource::LightSource() : position(), radius(0.0f), light_intensities(0.0f), color() {
}

spe::LightSource::LightSource(const spe::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color)
    : position(pos), radius(radius), light_intensities(intensiti), color(color) {
}
