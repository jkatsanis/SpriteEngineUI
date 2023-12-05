#include "Camera.h"

//Constructor

spe::Camera::Camera() 
{ 
	this->m_ptr_Repo = nullptr;
	this->m_ptr_Window = nullptr;
	this->Position = spe::Vector2(0, 0);
	this->m_CameraZoom = 1.0f;
	this->CameraSpeed = 2000.0f;
	this->m_ZoomChanged = false;
}

spe::Camera::Camera(sf::RenderWindow* ptr, spe::SpriteRepository& repo)
{
	spe::Vector2 defaultPos = this->GetDefaultPosition();

	this->m_ptr_Repo = &repo;
	this->m_CameraZoom = 1.0f;
	this->Position = spe::Vector2(0, 0);
	this->m_ptr_Window = ptr;
	this->CameraView = sf::View(sf::Vector2f(defaultPos.X, defaultPos.Y), sf::Vector2f(1920, 1080));
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
	this->CameraView.setSize(1920 * this->m_CameraZoom, 1080 * this->m_CameraZoom);

	spe::Vector2 defaultPos = this->GetDefaultPosition();

	defaultPos.X += this->Position.X;
	defaultPos.Y += this->Position.Y;

	this->CameraView.setCenter(sf::Vector2f(defaultPos.X, defaultPos.Y));

	sf::Shader& shader = lightrepo->GetShader();

	sf::Vector2f a = sf::Vector2f(defaultPos.X - 960, defaultPos.Y - 540);
	shader.setUniform("cameraPosition", a);
	shader.setUniform("cameraZoom", this->m_CameraZoom);
	// std::cout << this->camera_zoom << std::endl;
}

spe::Vector2 spe::Camera::GetDefaultPosition() noexcept
{
	return spe::Vector2(960, 540);
}

