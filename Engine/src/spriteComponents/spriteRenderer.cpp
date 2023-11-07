#include "spriteRenderer.h"
#include <data/spriteData.h>

// Constructor / Destructor

s2d::SpriteRenderer::SpriteRenderer()
{
	this->init();
}

void s2d::SpriteRenderer::init()
{
	this->base_component = true;
	this->effected_by_light = false;
	this->path = "";
	this->exist = true;
	this->sorting_layer_index = 0;
}

// Public functions

void s2d::SpriteRenderer::reset()
{
	this->path = s2d::SpriteData::s_default_sprite_path;
	this->exist = true;
	this->sorting_layer_index = 0;
}
