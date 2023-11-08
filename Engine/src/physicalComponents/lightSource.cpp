#include "lightSource.h"

s2d::LightSource::LightSource() : position(), radius(0.0f), light_intensities(0.0f), color() {
}

s2d::LightSource::LightSource(const s2d::Vector2& pos, float radius, float intensiti, const sf::Vector3f& color)
    : position(pos), radius(radius), light_intensities(intensiti), color(color) {
}
