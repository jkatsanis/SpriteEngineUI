#pragma once

#include <SFML/Graphics.hpp>
#include <ImGui/ImGUISFML.h>

#include "Math/Vector2.h"
#include "Camera/Camera.h"
#include "Core/Repository.h"
#include "UIUtility/UIModels.h"
#include "UIUtility/UIModels.h"
#include "Core/Event.h"

namespace spe
{

	class GUIRepository : public spe::Repository<spe::Rectangle>
	{
	private:
		std::vector <spe::Rectangle*> m_Rectangles;

	public:
		spe::UIWindowData HierarchyData;
		spe::UIWindowData InspectorData;
		spe::UIWindowData AssetFolderData;
		spe::UIWindowData AnimationData;

		spe::EditorTools Tools;

		std::string DragAndDropPath;
		std::string DragAndDropName;

		spe::Sprite* child_to_parent;
		spe::Sprite* right_clicked_sprite;
		spe::Sprite* sprite_in_inspector;
		spe::Sprite* sprited_hovered_in_hierarchy;


		/// <summary>
		/// Apparently the data structure for sprites is a list and the sorting layer 
		/// gets sorted automaticly but the user wants to see displayed the sprite linear
		/// and when he creates anew sprite it shouldnt be sorted. This sorts the sprites whe na new 1 is added (ONLY EDITOR)
		/// Specfif of course and displays them in the Hierarchy
		/// </summary>
		std::vector<spe::Sprite*> HierarchySprites;

		/// <summary>
		/// Usally ptrs to the events of the window class
		/// </summary>
		spe::Event* ptr_Event;
		sf::Event* ptr_SFEvent;

		spe::Vector3 background_color;
		spe::Camera Camera;

		GUIRepository();

		void Render(sf::RenderWindow* ptr_render_window);

		spe::Rectangle* GetByName(const std::string& name) override;
		spe::Rectangle* GetById(uint32_t id) override;
		spe::Rectangle* GetByVecPos(uint32_t vec);

		void CleanUp();
		void InitHierarchySprites(std::list<spe::Sprite*>& sprites);

		uint32_t GetAmount() const override { return (uint32_t)this->m_Rectangles.size(); }
		void Add(spe::Rectangle* rec) override;
		void UpdateLayerIndex() override;
	};
}

