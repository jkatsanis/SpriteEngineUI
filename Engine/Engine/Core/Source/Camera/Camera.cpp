#include "Camera.h"

//Constructor

spe::Camera::Camera()
{
	this->m_WindowSize = spe::Vector2(1920, 1080);
	spe::Vector2 defaultPos = this->GetDefaultPosition();

	this->m_CameraZoom = 1.0f;
	this->Position = spe::Vector2(0, 0);
	this->CameraView = sf::View(sf::Vector2f(defaultPos.X, defaultPos.Y), sf::Vector2f(this->m_WindowSize.X, this->m_WindowSize.Y));
}

void spe::Camera::SetZoom(float zoom) noexcept
{
	if (zoom == this->m_CameraZoom)
	{
		return;
	}
	// need to update the light shader here !! 
	this->m_CameraZoom = zoom;
	this->m_ZoomChanged = true;
}
 
void spe::Camera::Reset() noexcept
{
	this->Position = spe::Vector2(0, 0);
	this->m_CameraZoom = 1.0f;
}

void spe::Camera::Update(spe::LightRepository* lightrepo)
{	
	this->CameraView.setSize(sf::Vector2f(this->m_WindowSize.X * this->m_CameraZoom, this->m_WindowSize.Y * this->m_CameraZoom));

	spe::Vector2 defaultPos = this->GetDefaultPosition();

	defaultPos.X += this->Position.X;
	defaultPos.Y += this->Position.Y;

	this->CameraView.setCenter(sf::Vector2f(defaultPos.X, defaultPos.Y));

	sf::Shader& shader = lightrepo->GetShader();

	sf::Vector2f a = sf::Vector2f(defaultPos.X - this->m_WindowSize.X / 2.0f, defaultPos.Y - this->m_WindowSize.Y / 2.0f);
	shader.setUniform("cameraPosition", a);
	shader.setUniform("cameraZoom", this->m_CameraZoom);
	shader.setUniform("screenSize", sf::Vector2f(this->m_WindowSize.X, this->m_WindowSize.Y));

	this->m_ZoomChanged = false;
}

spe::Vector2 spe::Camera::GetDefaultPosition() noexcept
{
	return spe::Vector2(this->m_WindowSize.X / 2.0f, this->m_WindowSize.Y / 2.0f);
}
