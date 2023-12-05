#include "Light.h"
#include "Sprite.h"

// Constructor

void spe::Light::init()
{
	this->m_Intensity = 200;
	this->m_Color = sf::Vector3f(1, 1, 1);
	this->m_Radius = DEFAULT_LIGHT_RADIUS;
	this->exist = false;
	this->base_component = false;
	this->m_RadiusChanged = false;
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
	this->m_Color = rhs.GetColor();
	this->m_Radius = rhs.GetRadius();
	this->m_Intensity = rhs.GetIntensity();
	this->m_ptr_LighRepository = rhs.m_ptr_LighRepository;

	this->ptr_attached_sprite = ptr_sprite;

	if (this->exist)
	{
		this->exist = false;
		this->Enable();
	}

}

// Public methods

void spe::Light::DeleteLight()
{
	THROW_IF_NO_LIGHT_REPO
	if (this->exist)
	{
		this->m_ptr_LighRepository->Remove(this->m_LightIndex);
		this->m_LightIndex = 0;
	}
}

void spe::Light::Enable()
{
	THROW_IF_NO_LIGHT_REPO
	if (this->exist)
	{
		return;
	}
	this->exist = true;
	this->m_ptr_LighRepository->Add(this->ptr_attached_sprite->Transform.GetPosition(), this->m_Radius, this->m_Intensity, this->m_Color);
	this->m_LightIndex = this->m_ptr_LighRepository->GetIndex();
}

void spe::Light::reset()
{
	this->m_Radius = DEFAULT_LIGHT_RADIUS;
}

void spe::Light::SetColor(const sf::Vector3f& color) noexcept
{
	if (this->m_Color == color)
	{
		return;
	}
	this->m_Color = color;
	this->m_ColorChanged = true;
}


void spe::Light::SetIntensity(float intense) noexcept
{
	if (this->m_Intensity == intense)
	{
		return;
	}
	this->m_Intensity = intense;
	this->m_IntensityChanged = true;
}

void spe::Light::SetRadius(float radius) noexcept
{
	if (radius == this->m_Radius)
	{
		return;
	}
	this->m_Radius = radius;
	this->m_RadiusChanged = true;
}

