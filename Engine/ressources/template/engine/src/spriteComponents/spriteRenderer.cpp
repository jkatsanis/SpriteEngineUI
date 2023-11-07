#include "spriteRenderer.h"

// Constructor / Destructor

s2d::SpriteRenderer::SpriteRenderer()
{
 	this->init();
}

void s2d::SpriteRenderer::init()
{
	this->path = "";
	this->exist = true;
	this->effected_by_light = false;
	this->sorting_layer_index = 0;
}

// Public functions

void s2d::SpriteRenderer::reset()
{
	this->path = "";
	this->exist = true;
	this->sorting_layer_index = 0;
}
