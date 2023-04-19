#include "UIInfo.h"

float s2d::UIInfo::s_defaultFontSize = 0.8f;
bool s2d::UIInfo::s_renderAssetFolder = true;
bool s2d::UIInfo::s_isAssetFolderActive = true;

const std::string s2d::UIInfo::s_icons[ICONS_AMOUNT] =
{
	PATH_TO_RESSOURCS"\\Icons\\defaultIcon.png",
	PATH_TO_RESSOURCS"\\Icons\\assetFolder.png",
	PATH_TO_RESSOURCS"\\Icons\\prefabIcon.png",
	PATH_TO_RESSOURCS"\\Icons\\codeIcon.png",
	PATH_TO_RESSOURCS"\\Icons\\pngIcon.png"
};

