#include "projectInfo.h" 

std::string s2d::ProjectInfo::getSolutionDir()
{
	char* cwd = _getcwd(NULL, 0);
	std::string copy = std::string(cwd);
	delete cwd;
	return copy;
}

std::string s2d::ProjectInfo::s_pathToUserProject = "";
