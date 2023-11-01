#include "light.h"

// Constructor

void s2d::Light::init()
{
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

void s2d::Light::reset()
{
	this->radius = 20;
}

