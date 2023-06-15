#include "UIInfo.h"

float s2d::UIInfo::s_default_font_size = 0.8f;
s2d::OpenWindow s2d::UIInfo::s_is_asset_folder_open = s2d::OpenWindow();
s2d::OpenWindow s2d::UIInfo::s_is_hierarchy_open = s2d::OpenWindow();
s2d::OpenWindow s2d::UIInfo::s_is_inspector_open = s2d::OpenWindow();
s2d::OpenWindow s2d::UIInfo::s_is_animation_open = s2d::OpenWindow(false);

const std::string s2d::UIInfo::s_icons[ICONS_AMOUNT] =
{
	PATH_TO_RESSOURCS"\\Icons\\defaultIcon.png",
	PATH_TO_RESSOURCS"\\Icons\\assetFolder.png",
	PATH_TO_RESSOURCS"\\Icons\\prefabIcon.png",
	PATH_TO_RESSOURCS"\\Icons\\codeIcon.png",
	PATH_TO_RESSOURCS"\\Icons\\pngIcon.png"
};

