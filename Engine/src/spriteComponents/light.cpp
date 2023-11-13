#include "light.h"
#include <physicalComponents/sprite.h>

// Constructor

void s2d::Light::init()
{
	this->m_intensity = 200;
	this->m_color = sf::Vector3f(1, 1, 1);
	this->m_radius = DEFAULT_LIGHT_RADIUS;
	this->exist = false;
	this->base_component = false;
	this->m_radius_changed = false;
}

s2d::Light::Light()
{
	this->init();
}

s2d::Light::Light(Sprite* ptr_attached_sprite)
{
	this->init();
	this->ptr_attached_sprite = ptr_attached_sprite;
}

s2d::Light::Light(Sprite* ptr_sprite, const s2d::Light& rhs)
{
	this->exist = rhs.exist;
	this->m_color = rhs.getColor();
	this->m_radius = rhs.getRadius();
	this->m_intensity = rhs.getIntensity();
}

// Public methods

void s2d::Light::deleteLight()
{
	s2d::LightRepository::remove(this->m_light_index);
	this->m_light_index = 0;
}

void s2d::Light::enable()
{
	if (this->exist)
	{
		return;
	}
	this->m_light_index = s2d::LightRepository::getIndex();
	this->exist = true;
	s2d::LightRepository::add(this->ptr_attached_sprite->transform.position, this->m_radius, this->m_intensity, this->m_color);
}

void s2d::Light::reset()
{
	this->m_radius = DEFAULT_LIGHT_RADIUS;
}

void s2d::Light::setColor(const sf::Vector3f& color)
{
	if (this->m_color == color)
	{
		return;
	}
	this->m_color = color;
	this->m_color_changed = true;
}


void s2d::Light::setIntensity(float intense)
{
	if (this->m_intensity == intense)
	{
		return;
	}
	this->m_intensity = intense;
	this->m_intensity_changed = true;
}

void s2d::Light::setRadius(float radius)
{
	if (radius == this->m_radius)
	{
		return;
	}
	this->m_radius = radius;
	this->m_radius_changed = true;
}

