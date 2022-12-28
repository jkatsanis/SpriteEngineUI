#pragma once

#include <iostream>
#include <ImGui.h>
#include <stdExtension.h>
#include <SFML/Graphics.hpp>
#include <vector3.h>
#include <gameObject.h>
#include <UIInspectorBoxCollider.h>
#include <UIInfo.h>
#include <UIHirachy.h>
#include <UIAssetFolder.h>
#include <fontManager.h>

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
		//Everytime when we select a new sprite in the HY this will be the pointer to it
		s2d::Sprite* m_currentSpriteInInspector;

		std::string m_menuName;
		std::string m_spriteName;
		sf::RectangleShape m_rectangle;
		sf::RectangleShape m_boxCollider;
		sf::Texture m_textureOverSprite;

		//When the box collider component is open we want to display the rectangle as a box collider and not as a rectangle
		float m_windowSizeWidth;
		const char* m_currentComponentSelected;

		//name
		char* m_inputName;

		//Collider
		s2d::UIInspectorBoxCollider m_collider;

		//Game window background color
	    s2d::Vector3 m_defaultBackgroundColor;

		void render();
		bool isInspectorSpriteEqualToActiveSprites();

		void setupComponents();
		void transformComponent();
		void spriteRendererComponent();
		void boxColliderComponent();
		void physicsBodyComponent();
		void animatorComponent();

		void componentSelector();
		void setCompontents();

		void drawRectangleOverCurrentObject();

		void displayDefaultInspectorView();
		void backgroundSetting();
		void gameEngineViewSetting();

		void checkDupeName();
	public:
		bool isHovered;
		s2d::InspectorState state;

		// Get the value from UIWindow.cpp
		s2d::Vector3 backgroundColor;

		UIInspector();

		void createUIInspector();

	public:

		static char* getNamePathSplit(std::string string);
	};
}

