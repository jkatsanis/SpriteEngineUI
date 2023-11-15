#include "spriteRenderer.h"
#include <data/spriteData.h>

// Constructor / Destructor

s2d::SpriteRenderer::SpriteRenderer()
{
	this->init();
}

s2d::SpriteRenderer::SpriteRenderer(const s2d::SpriteRenderer& rhs)
{
	this->effected_by_light = rhs.effected_by_light;
	this->path = rhs.path;
	this->sorting_layer_index = rhs.sorting_layer_index;
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
