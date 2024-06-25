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
		bool ReloadSortingLayersOnSceneSwap;

		EngineConfig() = default;

		EngineConfig(spe::SceneHandler* sceneHandler, spe::Camera* camera, spe::Vector3* backgroundColor)
			: ptr_SceneHandler(sceneHandler), ptr_Camera(camera), ptr_BackgroundColor(backgroundColor), ReloadSortingLayersOnSceneSwap(false)
		{
			this->ptr_Sprites = &this->ptr_SceneHandler->SpriteRepository;
		}

		void DontDeleteOnSceneSwap(spe::Sprite* spr)
		{
			spr->DontDeleteOnSceneSwap = true;
			this->ReloadSortingLayersOnSceneSwap = true;

			for (size_t i = 0; i < spr->ptr_Childs.size(); i++)
			{
				this->DontDeleteOnSceneSwap(spr->ptr_Childs[i]);
			}
		}

		void LoadScene(const std::string& scene)
		{
			this->ptr_SceneHandler->LoadScene(scene, *this->ptr_Camera, *this->ptr_BackgroundColor);
			if (this->ReloadSortingLayersOnSceneSwap)
			{
				this->ptr_Sprites->SortSpritesByLayer();
			}
		}
	};
}