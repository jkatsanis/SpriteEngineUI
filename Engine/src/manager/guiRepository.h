#pragma once

#include <ImGui.h>
#include <physicalComponents/camera.h>

namespace s2d
{
	struct Rectangle
	{
		uint32_t sorting_layer_index;
		uint32_t id;
		std::string name;
		sf::RectangleShape shape;
		sf::Texture texture;
		bool render;

		Rectangle(const std::string& path_to_texture)
		{
			this->sorting_layer_index = 0;
			this->render = true;
			this->id = 0;
			this->texture.loadFromFile(path_to_texture);
			this->shape.setTexture(&this->texture);
		}
	};
	class GUIRepository
	{
	private:
	    uint32_t m_highest_layer_idx;

		std::vector <s2d::Rectangle*> m_rectangles;
		uint32_t m_highest_rectangle_id;

	public:
		const ImVec2* ptr_asset_window_size;
		const ImVec2* ptr_inspector_window_size;
		const ImVec2* ptr_hierarchy_window_size;

		s2d::Camera camera;

		GUIRepository();

		void render(sf::RenderWindow* ptr_render_window);

		s2d::Rectangle* getByName(const std::string& name);
		s2d::Rectangle* getByVecPos(uint32_t vec);

		size_t amount() const { return this->m_rectangles.size(); }

		void add(const sf::Vector2f& pos, const sf::Vector2f& size, 
			const sf::Color& outline_color, float outline_thickness,
			const std::string& path_to_texture, const std::string& name);

		void updateHighestLayerIndex();
	};
}

