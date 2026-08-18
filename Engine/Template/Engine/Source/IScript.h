#pragma once

#include <iostream>
#include "EngineConfig.h"

namespace spe
{
	class IScript
	{
	public:

		IScript() = default;

		// Do not declare them as pure virtual because maybe user wants to pass a reference
		virtual void Update() { };
		virtual void Start(spe::EngineConfig& cnfg, const std::string& scene) {
			(void)cnfg;
			(void)scene;
		}
	};
}