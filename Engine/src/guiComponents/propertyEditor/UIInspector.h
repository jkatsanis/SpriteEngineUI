#pragma once

#include <iostream>
#include <ImGui.h>
#include <stdExtension.h>
#include <SFML/Graphics.hpp>
#include <calculationComponents/vector3.h>
#include <physicalComponents/gameObject.h>
#include <UIInspectorBoxCollider.h>
#include <data/UIInfo.h>
#include <UIAssetFolder.h>
#include <manager/fontManager.h>
#include <manager/spriteRepository.h>

#define DEFAULT_BACKGROUND_COLOR s2d::Vector3(139, 165, 187)

#define INSPECTOR_WINDOW_POS ImVec2(1530, 53)
#define INSPECTOR_DEFAULT_WINDOW_SIZE ImVec2(390, 1000)

namespace s2d
{
	enum class InspectorState
	{
		None = -1,
		GameWindowEditor = 0,
		SpriteEditorWindow = 1
	};
	class UIInspector
	{
	private:
		s2d::SpriteRepository* m_ptr_repo;
		std::string m_menu_name;
		std::string m_sprite_name;
		sf::RectangleShape m_rectangle;
		sf::RectangleShape m_box_collider;
		sf::Texture m_texture_over_sprite;
		ImVec2 m_window_size;
		float m_sprite_input_width;

		//When the box collider component is open we want to display the rectangle as a box collider and not as a rectangle
		float m_window_size_width;
		const char* m_current_component_selected;
		char* m_input_name;
		s2d::UIInspectorBoxCollider m_collider;
		bool m_clicked_resize_button;

		void render();
		void resizeWindow();

		void setupComponents();
		void transformComponent();
		void spriteRendererComponent();
		void boxColliderComponent();
		void physicsBodyComponent();
		void animatorComponent();
		void prefabComponent();

		void componentSelector();
		void setCompontents();

		void drawRectangleOverCurrentObject();

		void displayDefaultInspectorView();
		void backgroundSetting();
		void gameEngineViewSetting();

		void checkDupeName();

		void init();
	public:
		bool is_hovered;
		s2d::InspectorState state;

		// Get the value from UIWindow.cpp
		s2d::Vector3 background_color;

		UIInspector();

		void createUIInspector();

		void setSpriteRepository(s2d::SpriteRepository& repo) { this->m_ptr_repo = &repo; }

		const float* getSizePtrX() const { return &this->m_window_size.x; }

	public:
		static std::string getNamePathSplit(std::string path);
	};
}

