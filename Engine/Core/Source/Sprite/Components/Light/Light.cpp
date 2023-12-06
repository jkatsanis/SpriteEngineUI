#include "Light.h"
#include "Sprite.h"

// Constructor

void spe::Light::Init()
{
	this->m_Intensity = 200;
	this->m_Color = sf::Vector3f(1, 1, 1);
	this->m_Radius = DEFAULT_LIGHT_RADIUS;
	this->Exist = false;
	this->BaseComponent = false;
	this->m_RadiusChanged = false;
}

spe::Light::Light()
{
	this->Init();
}

spe::Light::Light(Sprite* ptr_attached_sprite, spe::LightRepository* ptr)
{
	this->Init();
	this->ptr_attached_sprite = ptr_attached_sprite;
	this->m_ptr_LighRepository = ptr;
}

spe::Light::Light(Sprite* ptr_sprite, const spe::Light& rhs)
{
	this->Exist = rhs.Exist;
	this->m_Color = rhs.GetColor();
	this->m_Radius = rhs.GetRadius();
	this->m_Intensity = rhs.GetIntensity();
	this->m_ptr_LighRepository = rhs.m_ptr_LighRepository;

	this->ptr_attached_sprite = ptr_sprite;

	if (this->Exist)
	{
		this->Exist = false;
		this->Enable();
	}

}

// Public methods

void spe::Light::DeleteLight()
{
	THROW_IF_NO_LIGHT_REPO
	if (this->Exist)
	{
		this->m_ptr_LighRepository->Remove(this->m_LightIndex);
		this->m_LightIndex = 0;
	}
}

void spe::Light::Enable()
{
	THROW_IF_NO_LIGHT_REPO
	if (this->Exist)
	{
		return;
	}
	this->Exist = true;
	this->m_ptr_LighRepository->Add(this->ptr_attached_sprite->Transform.GetPosition(), this->m_Radius, this->m_Intensity, this->m_Color);
	this->m_LightIndex = this->m_ptr_LighRepository->GetIndex();
}

void spe::Light::Reset()
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

