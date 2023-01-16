#include "camera.h"

//Constructor

s2d::Camera::Camera()
{
	this->ptr_renderWindow_to_engine = nullptr;
	this->transform.position = Vector2(0, 0);
	this->cameraZoom = 0.8f;
}

s2d::Camera::Camera(sf::RenderWindow* ptr)
{
	s2d::Vector2 defaultPos = this->getDefaultPosition();

	this->cameraZoom = 1;
	this->transform.position = Vector2(0, 0);
	this->ptr_renderWindow_to_engine = ptr;
	this->cameraView = sf::View(sf::Vector2f(defaultPos.x, defaultPos.y), sf::Vector2f(1920, 1080));
}

void s2d::Camera::reset()
{
	this->transform.position = Vector2(0, 0);
	this->cameraZoom = 0.8f;
}

void s2d::Camera::update()
{
	this->cameraView.setSize(this->cameraZoom * 1920, this->cameraZoom * 1080);

	s2d::Vector2 defaultPos = this->getDefaultPosition();

	defaultPos.x += transform.position.x;
	defaultPos.y -= transform.position.y;

	this->cameraView.setCenter(sf::Vector2f(defaultPos.x, defaultPos.y));
	this->ptr_renderWindow_to_engine->setView(this->cameraView);
}


s2d::Vector2 s2d::Camera::getDefaultPosition()
{
	return s2d::Vector2(960, 540);
}

