#pragma once

#include <SFML/Graphics.hpp>
#include <ImGui/ImGUISFML.h>

#include "Math/Vector2.h"
#include "Camera/Camera.h"
#include "Core/Repository.h"
#include "UIUtility/UIModels.h"

namespace spe
{

	class GUIRepository : public spe::Repository<spe::Rectangle>
	{
	private:
		std::vector <spe::Rectangle*> m_Rectangles;

	public:
		const ImVec2* ptr_AssetWindowSize;
		const ImVec2* ptr_InspectorWindowSize;
		const ImVec2* ptr_HierarchyWindowSize;

		std::string DragAndDropPath;
		std::string DragAndDropName;

		spe::Sprite* child_to_parent;
		spe::Sprite* right_clicked_sprite;
		spe::Sprite* sprite_in_inspector;
		spe::Sprite* sprited_hovered_in_hierarchy;

		spe::Vector3 background_color;
		spe::Camera Camera;

		GUIRepository();

		void Render(sf::RenderWindow* ptr_render_window);

		spe::Rectangle* GetByName(const std::string& name) override;
		spe::Rectangle* GetById(uint32_t id) override;
		spe::Rectangle* GetByVecPos(uint32_t vec);

		uint32_t GetAmount() const override { return (uint32_t)this->m_Rectangles.size(); }
		void Add(spe::Rectangle* rec) override;
		void UpdateLayerIndex() override;
	};
}

