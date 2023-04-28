#include "guiRepository.h"

// Constrcutor

s2d::GUIRepository::GUIRepository()
{
	this->camera = nullptr;
	this->ptr_asset_window_size = nullptr;
	this->ptr_hierarchy_window_size = nullptr;
	this->ptr_inspector_window_size = nullptr;
	this->m_highest_rectangle_id = 0;
}

// Public functions

void s2d::GUIRepository::add(const sf::Vector2f& pos, 
	const sf::Vector2f& size, const sf::Color& outline_color, 
	float outline_thickness, const std::string& path_to_texture, const std::string& name)
{
	s2d::Rectangle* rectangle = new s2d::Rectangle(path_to_texture);

	rectangle->name = name;
	rectangle->id = this->m_highest_rectangle_id;
	rectangle->shape.setSize(size);
	rectangle->shape.setOutlineColor(outline_color);
	rectangle->shape.setOutlineThickness(outline_thickness);
	this->m_rectangles.push_back(rectangle);

	this->m_highest_rectangle_id++;
}

void s2d::GUIRepository::render(sf::RenderWindow* ptr_render_window)
{
	for (int i = 0; i < this->m_rectangles.size(); i++)
	{
		if (this->m_rectangles[i]->render)
		{
			ptr_render_window->draw(this->m_rectangles[i]->shape);
		}
	}
}

s2d::Rectangle* s2d::GUIRepository::getByName(const std::string& name)
{
	for (int i = 0; i < this->m_rectangles.size(); i++)
	{
		if (this->m_rectangles[i]->name == name)
		{
			return this->m_rectangles[i];
		}
	}
	return nullptr;
}

s2d::Rectangle* s2d::GUIRepository::getByVecPos(uint32_t vec)
{
	for (int i = 0; i < this->m_rectangles.size(); i++)
	{
		if (i == vec)
		{
			return this->m_rectangles[i];
		}
	}
	return nullptr;
}
