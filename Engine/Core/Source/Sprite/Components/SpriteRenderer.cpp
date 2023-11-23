#include "SpriteRenderer.h"

// Constructor / Destructor

spe::SpriteRenderer::SpriteRenderer()
{
	this->init();
}

spe::SpriteRenderer::SpriteRenderer(const spe::SpriteRenderer& rhs)
{
	this->effected_by_light = rhs.effected_by_light;
	this->path = rhs.path;
	this->sorting_layer_index = rhs.sorting_layer_index;
}

void spe::SpriteRenderer::init()
{
	this->render = true;
	this->base_component = true;
	this->effected_by_light = false;
	this->path = "";
	this->exist = true;
	this->sorting_layer_index = 0;
}

// Public functions

void spe::SpriteRenderer::reset()
{
	this->path = "defaultpath";
	this->exist = true;
	this->sorting_layer_index = 0;
}