#pragma once

#include "GUI/Property/UIBase.h"
#include "Core/SeceneHandler.h"

namespace spe
{
	class UITopbar : public IUIBase
	{
	private:
		spe::SceneHandler* m_ptr_SceneHandler;

		void Init() override;
	public:

		void Render() override;

		void SetSceneHandler(spe::SceneHandler& scene) { this->m_ptr_SceneHandler = &scene; }

	};
}