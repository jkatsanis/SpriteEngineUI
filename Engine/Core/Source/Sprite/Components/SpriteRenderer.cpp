#include "SpriteRenderer.h"

// Constructor / Destructor

spe::SpriteRenderer::SpriteRenderer()
{
	this->init();
}

spe::SpriteRenderer::SpriteRenderer(const spe::SpriteRenderer& rhs)
{
	this->EffectedByLight = rhs.EffectedByLight;
	this->Path = rhs.Path;
	this->SortinLayerIdx = rhs.SortinLayerIdx;
	this->EffectedByLight = rhs.EffectedByLight;
}

void spe::SpriteRenderer::init()
{
	this->Render = true;
	this->base_component = true;
	this->EffectedByLight = false;
	this->Path = "";
	this->exist = true;
	this->SortinLayerIdx = 0;
}

// Public functions

void spe::SpriteRenderer::reset()
{
	this->Path = "defaultpath";
	this->exist = true;
	this->SortinLayerIdx = 0;
}