#pragma once

#include "IScript.h"

class Game : public spe::IScript
{	
public:
	
	// Gets called on start of the engine
	void Start() override;

	// Gets called once per frame
	void Update() override;
};

