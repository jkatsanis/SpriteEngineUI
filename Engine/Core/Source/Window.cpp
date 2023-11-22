#include "Window.h"

spe::Window::Window()
{
	this->m_ptr_RenderWindow = nullptr;
}

spe::Window::Window(const spe::Vector2& size, const std::string& name)
{
	this->m_ptr_RenderWindow = new sf::RenderWindow(sf::VideoMode(size.x, size.y), name, sf::Style::Default);
}

void spe::Window::Draw()
{
	std::cout << "hi";
}
