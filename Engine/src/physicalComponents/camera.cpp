#include "camera.h"

//Constructor

s2d::Camera::Camera() 
{ 
	this->ptr_renderWindow_to_engine = nullptr;
	this->transform.position = Vector2(0, 0);
	this->camera_zoom = 1.0f;
	this->camera_speed = 2000.0f;
}

s2d::Camera::Camera(sf::RenderWindow* ptr)
{
	s2d::Vector2 defaultPos = this->getDefaultPosition();

	this->camera_zoom = 1.0f;
	this->transform.position = Vector2(0, 0);
	this->ptr_renderWindow_to_engine = ptr;
	this->camera_view = sf::View(sf::Vector2f(defaultPos.x, defaultPos.y), sf::Vector2f(1920, 1080));
}

void s2d::Camera::reset()
{
	this->transform.position = Vector2(0, 0);
	this->camera_zoom = 1.0f;
}

void s2d::Camera::update()
{
	this->camera_zoom = 1.25f;
	this->camera_view.setSize(1920 * this->camera_zoom, 1080 * this->camera_zoom);

	s2d::Vector2 defaultPos = this->getDefaultPosition();

	defaultPos.x += transform.position.x;
	defaultPos.y += transform.position.y;

	this->camera_view.setCenter(sf::Vector2f(defaultPos.x, defaultPos.y));

	sf::Shader& shader =  s2d::LightRepository::getShader();

	sf::Vector2f a = sf::Vector2f(defaultPos.x - 960, defaultPos.y - 540);
	shader.setUniform("cameraPosition", a);
	shader.setUniform("cameraZoom", this->camera_zoom);

	// std::cout << this->camera_zoom << std::endl;
}

void s2d::Camera::updateLights()
{
	
}


s2d::Vector2 s2d::Camera::getDefaultPosition()
{
	return s2d::Vector2(960, 540);
}

