#include "Camera.h"

//Constructor

spe::Camera::Camera() 
{ 
	this->m_ptr_sprite_repo = nullptr;
	this->ptr_Window = nullptr;
	this->Position = spe::Vector2(0, 0);
	this->m_camera_zoom = 1.0f;
	this->camera_speed = 2000.0f;
	this->m_zoom_changed = false;
}

spe::Camera::Camera(sf::RenderWindow* ptr, spe::SpriteRepository& repo)
{
	spe::Vector2 defaultPos = this->getDefaultPosition();

	this->m_ptr_sprite_repo = &repo;
	this->m_camera_zoom = 1.0f;
	this->Position = spe::Vector2(0, 0);
	this->ptr_Window = ptr;
	this->camera_view = sf::View(sf::Vector2f(defaultPos.x, defaultPos.y), sf::Vector2f(1920, 1080));
}

void spe::Camera::setZoom(float zoom)
{
	if (zoom == this->m_camera_zoom)
	{
		return;
	}
	// need to update the light shader here !! 
	this->m_camera_zoom = zoom;
	this->m_zoom_changed = true;
}

void spe::Camera::reset()
{
	this->Position = spe::Vector2(0, 0);
	this->m_camera_zoom = 1.0f;
}

void spe::Camera::Update(spe::LightRepository* lightrepo)
{	
	this->camera_view.setSize(1920 * this->m_camera_zoom, 1080 * this->m_camera_zoom);

	spe::Vector2 defaultPos = this->getDefaultPosition();

	defaultPos.x += this->Position.x;
	defaultPos.y += this->Position.y;

	this->camera_view.setCenter(sf::Vector2f(defaultPos.x, defaultPos.y));

	sf::Shader& shader = lightrepo->getShader();

	sf::Vector2f a = sf::Vector2f(defaultPos.x - 960, defaultPos.y - 540);
	shader.setUniform("cameraPosition", a);
	shader.setUniform("cameraZoom", this->m_camera_zoom);
	// std::cout << this->camera_zoom << std::endl;
}

spe::Vector2 spe::Camera::getDefaultPosition()
{
	return spe::Vector2(960, 540);
}

