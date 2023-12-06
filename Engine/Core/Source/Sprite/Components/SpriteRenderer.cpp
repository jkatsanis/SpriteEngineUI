#include "SpriteRenderer.h"

// Constructor / Destructor

spe::SpriteRenderer::SpriteRenderer()
{
	this->Init();
}

spe::SpriteRenderer::SpriteRenderer(const spe::SpriteRenderer& rhs)
{
	this->EffectedByLight = rhs.EffectedByLight;
	this->Path = rhs.Path;
	this->SortinLayerIdx = rhs.SortinLayerIdx;
	this->EffectedByLight = rhs.EffectedByLight;
}

void spe::SpriteRenderer::Init()
{
	this->Render = true;
	this->BaseComponent = true;
	this->EffectedByLight = false;
	this->Path = "";
	this->Exist = true;
	this->SortinLayerIdx = 0;
}

// Public functions

void spe::SpriteRenderer::Reset()
{
	this->Path = "defaultpath";
	this->Exist = true;
	this->SortinLayerIdx = 0;
}