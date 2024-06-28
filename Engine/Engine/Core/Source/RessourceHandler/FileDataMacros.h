#pragma once
#include "Core/EngineData.h"

/////////////////////
// ENGINE (NAMES) 
#define DELIMITER ';'
#define PREFAB_DELIMITER '|'
#define EXTENSION_PREFAB_FILE ".prfb"
#define EXTENSION_SAVE_FILE ".txt"
#define EXTENSION_SCENE_SAVE_FILE ".scn"
#define EXTENSION_ANIMATION_FILE ".txt"
///////////////////// 

#define PATH_TO_SAVE_FOLDER "Engine\\Saves"
#define PATH_TO_SCENE_FOLDER "Engine\\Saves\\" + spe::EngineData::s_Scene

#define USER_FOLDER_NAME "Assets"

//////////
/// SPRITE
#define PATH_TO_SPRITES PATH_TO_SCENE_FOLDER + "\\sprites.txt"
// This macro points to a animation file the sprite can havem and in this file there are path of animations to actually load
#define PATH_TO_CAMERA PATH_TO_SCENE_FOLDER + "\\camera.txt"
#define PATH_TO_BACKGROUND PATH_TO_SCENE_FOLDER + "\\background.txt"

#define PATH_TO_HIGHEST_INDEX "Engine\\Saves\\index.txt"
#define PATH_TO_TAG_FILE "Engine\\Saves\\tags.txt"
#define PATH_TO_SCENE_FILE "Engine\\Saves\\scenes.scn"

#define PATH_TO_DEFAULT_SPRITE "Engine\\Ressources\\Sprites\\Default.png"

// This is only editor specify btw

///////////////
// RESSOURCES
#define PATH_TO_RESSOURCES spe::EngineData::s_PathOfEngine + "\\Editor\\Ressources"
#define PATH_TO_KNOWN_PROJECTS PATH_TO_RESSOURCES + "\\Saves\\projects.txt"
#define PATH_TO_TRANSPARENT_PIC PATH_TO_RESSOURCES + "\\Sprites\\transparent.png"
#define PATH_TO_LIGHT_SHADER PATH_TO_RESSOURCES + "\\Shaders\\circulaer_gradient.frag"
