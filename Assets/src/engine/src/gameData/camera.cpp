#include "camera.h"
#include <sprite.h>

//Constructor

s2d::Camera::Camera()
{
	this->m_ptr_renderWindow_to_engine = nullptr;
	this->cameraZoom = 0.8f;
}

s2d::Camera::Camera(sf::RenderWindow& ptr)
{
	s2d::Vector2 defaultPos = this->getDefaultPosition();

	this->cameraZoom = 0.8f;
	this->m_ptr_renderWindow_to_engine = &ptr;
	this->cameraView = sf::View(sf::Vector2f(defaultPos.x, defaultPos.y), sf::Vector2f(1920, 1080));
}

//Public functions

void s2d::Camera::reset()
{
	this->cameraZoom = 0.8f;
}


s2d::Vector2 s2d::Camera::getDefaultPosition()
{
	return s2d::Vector2(960, 540);
}


void s2d::Camera::setPosition(const s2d::Vector2& position)
{
	s2d::Vector2 defaultPos = this->getDefaultPosition();

	defaultPos.x += position.x;
	defaultPos.y -= position.y;

	this->cameraView.setCenter(sf::Vector2f(defaultPos.x, defaultPos.y));
	this->m_ptr_renderWindow_to_engine->setView(this->cameraView);
}


