#pragma once

// Included from the editor-src
#include <Source/SpriteEngine.h>

#include "Player/PlayerController.h"

class Game : public spe::IScript
{		
private:
	PlayerController m_PlayerController;

public:
	spe::EngineConfig EngineConfig;
	
	// Gets called on start of the engine
	void Start() override;

	// Gets called once per frame
	void Update() override;
};

