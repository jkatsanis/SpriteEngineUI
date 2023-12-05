#pragma once

#include <ImGui/ImGUISFML.h>
#include <iostream>

#include "Utility/Style.h"

#include "Math/Vector2.h"
#include "Camera/Camera.h"

#define D_SCALE spe::Style::s_DefaultFontSize
#define DEFAULT_FLAGS ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar

namespace spe 
{
	/// <summary>
	/// Uses the ImGUI library
	/// </summary>
	class GUI
	{
	private:
		static const spe::Camera* s_ptr_Camera;

	public:
		GUI() = delete;

		static bool Button(const std::string& content);

		static void SetCursorCamera(const spe::Vector2& position);

		static void SetCursor(const spe::Vector2& pos);

		static void SetFontScale(float scale);

		static void SetCamera(const spe::Camera* ptr) { spe::GUI::s_ptr_Camera = ptr; }

		static void Window(const std::string& id, const spe::Vector2& pos, const spe::Vector2& size);

		static void End();
	};
}