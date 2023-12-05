#pragma once

#include <iostream>
#include <ImGui.h>
#include <Include/UtilityFunctions.h>
#include <Include/SFML/Graphics.hpp>

#include "RessourceHandler/FileDataMacros.h"
#include "GUI/Property/Inspector/UITagSelector.h"
#include "GUI/Property/Inspector/UIInspectorBoxCollider.h"
#include "GUI/UIUtility/UIModels.h"
#include "GUI/Property/UIBase.h"
#include "Utility/Style.h"
#include "GUI/UIUtility/UIUtility.h"
#include "Core/Time.h"
#include "GUI/Color.h"
#include "Savesystem.h"

#include "Sprite/Sprite.h"

#define DEFAULT_BACKGROUND_COLOR spe::Vector3(139, 165, 187)
#define INSPECTOR_WINDOW_POS ImVec2(1530, 53)
#define INSPECTOR_DEFAULT_WINDOW_SIZE ImVec2(390, 1000)
#define ADD_COMPONENTS_MARGIN 300
#define SEARCH_BAR_MARGIN 290
#define COMPONENT_PADDING_LEFT 20
#define COMPONENT_SELECTED_COLOR SPRITE_BACKGROUND_COLOR
#define DUMMY_COMPONENT ImGui::Dummy(ImVec2(0, 8))
#define DUPE_NAME_TIME_EDIT 0.3f

namespace spe
{
	enum class InspectorState
	{
		None = -1,
		GameWindowEditor = 0,
		SpriteEditorWindow = 1
	};
	class UIInspector : public spe::IUIBase
	{
	private:
		// gui repo (rectangles)
		spe::Rectangle* m_ptr_ColliderRec;
		spe::Rectangle* m_ptr_SpriteRec;

		spe::UITagSelector m_TagSelector;
		spe::UIInspectorBoxCollider m_Collider;

		spe::ResizeWindowData m_WindowData;
		std::string m_MenuName;
		std::string m_SpriteName;
;
		ImVec2 m_PopUpCursorPos;
		float m_SpriteInputWidth;
		float m_DupeNameEditorTimer;
		std::vector<const char*> m_ComponentsName;

		//When the box collider component is open we want to display the rectangle as a box collider and not as a rectangle
		float m_WindowSizeWidth;
		std::string m_ComponentSelected;
		char* m_InputName;

		float m_LightRadius;
		float m_LightIntensity;
		float m_CamZoom;

		void RenderOptions();
		void ResizeWindow();
		void SetupComponents();

		void TransformComponent();
		void SpriteRendererComponent();
		void BoxColliderComponent();
		void PhysicsBodyComponent();
		void AnimatorComponent();
		void PrefabComponent();
		void LightComponent();

		void DisplayDefaultInspectorView();

		void InputXY(const char* label, float& inputX, float& inputY, float x, float y);
	
		void RenameSprite();
		void ComponentSelector();
		void SetCompontents();
		void DrawRectangleOverCurrentObject();
		void RenderBackgroundBehindComponent();
		void BackgroundSetting();
		void GameEngineViewSetting();
		void RenderComponentOptions(spe::Component& component, const std::string& name);
		void GeneralSettings();

		void Init() override;

	public:
		spe::InspectorState State;

		void Render() override;
	};
}

