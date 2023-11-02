#include "light.h"
#include <physicalComponents/sprite.h>

// Constructor

void s2d::Light::init()
{
	this->intensity = 1;
	this->color = s2d::Vector3(1, 1, 1);
	this->radius = DEFAULT_LIGHT_RADIUS;
	this->exist = false;
	this->base_component = false;
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

// Public methods

void s2d::Light::enable()
{
	this->exist = true;
	s2d::LightRepository::add(s2d::Vector2(0, 0), this->radius, this->intensity, this->color, "Oga");
}

void s2d::Light::reset()
{
	this->radius = DEFAULT_LIGHT_RADIUS;
}

