#pragma once

#include "Core/SeceneHandler.h"

namespace spe
{
	struct EngineConfig
	{
		spe::SceneHandler* ptr_SceneHandler;
		spe::Camera* ptr_Camera;
		spe::Vector3* ptr_BackgroundColor;
		spe::SpriteRepository* ptr_Sprites;

		EngineConfig()
			: ptr_SceneHandler(nullptr), ptr_Camera(nullptr), ptr_BackgroundColor(nullptr), ptr_Sprites(nullptr)
		{
		}

		EngineConfig(spe::SceneHandler* sceneHandler, spe::Camera* camera, spe::Vector3* backgroundColor)
			: ptr_SceneHandler(sceneHandler), ptr_Camera(camera), ptr_BackgroundColor(backgroundColor)
		{
			this->ptr_Sprites = &this->ptr_SceneHandler->SpriteRepository;
		}
	};
}