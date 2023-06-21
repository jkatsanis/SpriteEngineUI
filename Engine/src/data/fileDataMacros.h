#define DELIMITER ";"

#define EXTENSION_PREFAB_FILE ".prfb"
#define EXTENSION_SAVE_FILE ".txt"
#define EXTENSION_SCENE_SAVE_FILE ".scn"
#define EXTENSION_ANIMATION_FILE ".txt"

/////////////////////
// ENGINE 
#define PATH_TO_RESSOURCES "ressources"
#define PATH_TO_TEMPLATE_FOLDER PATH_TO_RESSOURCES"\\template"
#define PATH_TO_TRANSPARENT_PIC PATH_TO_RESSOURCES"\\Sprites\\transparent.png"
#define PATH_TO_KNOWN_PROJECTS PATH_TO_RESSOURCES"\\saves\\projects.txt" 
/////////////////////


/////////////////////
// USER
#define PATH_TO_USER_ASSET_FOLDER s2d::EngineData::s_path_to_user_project + "\\assets"
#define PATH_TO_USER_DEBUG_FOLDER s2d::EngineData::s_path_to_user_project + "\\x64\\Debug\\" 
#define PATH_TO_USER_SAVES_FOLDER s2d::EngineData::s_path_to_user_project + "\\engine\\saves"
#define PATH_TO_SCENE_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\scenes" + EXTENSION_SCENE_SAVE_FILE
#define PATH_TO_ANIMATION_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\" + s2d::EngineData::s_scene + "\\animation.txt"
#define PATH_TO_SPRITE_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\" + s2d::EngineData::s_scene + "\\sprites.txt"
#define PATH_TO_BACKGROUND_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\" + s2d::EngineData::s_scene + "\\background.txt"
#define PATH_TO_CAMERA_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\" + s2d::EngineData::s_scene + "\\camera.txt"
#define PATH_TO_INDEX_FILE s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\" + s2d::EngineData::s_scene + "\\index.txt"
#define PATH_TO_KNOWN_ANIMATIONS s2d::EngineData::s_path_to_user_project + "\\engine\\saves\\" + s2d::EngineData::s_scene + "\\ animations.txt"
/////////////////////