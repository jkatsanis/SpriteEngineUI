#include "Light.h"
#include "Sprite.h"

// Constructor

void spe::Light::init()
{
	this->m_intensity = 200;
	this->m_color = sf::Vector3f(1, 1, 1);
	this->m_radius = DEFAULT_LIGHT_RADIUS;
	this->exist = false;
	this->base_component = false;
	this->m_radius_changed = false;
}

spe::Light::Light()
{
	this->init();
}

spe::Light::Light(Sprite* ptr_attached_sprite, spe::LightRepository* ptr)
{
	this->init();
	this->ptr_attached_sprite = ptr_attached_sprite;
	this->m_ptr_LighRepository = ptr;
}

spe::Light::Light(Sprite* ptr_sprite, const spe::Light& rhs)
{
	this->exist = rhs.exist;
	this->m_color = rhs.getColor();
	this->m_radius = rhs.getRadius();
	this->m_intensity = rhs.getIntensity();
	this->m_ptr_LighRepository = rhs.m_ptr_LighRepository;
}

// Public methods

void spe::Light::deleteLight()
{
	THROW_IF_NO_LIGHT_REPO
	this->m_light_index = 0;
}

void spe::Light::enable()
{
	THROW_IF_NO_LIGHT_REPO
	if (this->exist)
	{
		return;
	}
	this->exist = true;
	this->m_ptr_LighRepository->add(this->ptr_attached_sprite->transform.GetPosition(), this->m_radius, this->m_intensity, this->m_color);
	this->m_light_index = this->m_ptr_LighRepository->getIndex();
}

void spe::Light::reset()
{
	this->m_radius = DEFAULT_LIGHT_RADIUS;
}

void spe::Light::setColor(const sf::Vector3f& color)
{
	if (this->m_color == color)
	{
		return;
	}
	this->m_color = color;
	this->m_color_changed = true;
}


void spe::Light::setIntensity(float intense)
{
	if (this->m_intensity == intense)
	{
		return;
	}
	this->m_intensity = intense;
	this->m_intensity_changed = true;
}

void spe::Light::setRadius(float radius)
{
	if (radius == this->m_radius)
	{
		return;
	}
	this->m_radius = radius;
	this->m_radius_changed = true;
}

