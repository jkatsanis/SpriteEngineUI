#include "engineConfig.h"
#include <engineComponents/gameEngine.h>

void s2d::EngineConfig::loadScene(const std::string& name)
{
	this->m_ptr_game_engine->loadScene(name);
}
