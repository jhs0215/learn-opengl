#include "includeOgl.h"
#include "ProgramDirectory.h"

std::string CProgramDirectory::GetWorkingDirectory()
{
	char aCurPath[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH, aCurPath);

	return std::string(aCurPath);
}

std::string CProgramDirectory::GetModuleDirectory()
{
	char aModulePath[MAX_PATH + 1];
	GetModuleFileName(NULL, aModulePath, MAX_PATH);

	std::string path = aModulePath;
	std::string dir;
	
	auto item = path.rfind('\\');
	if (item != std::string::npos)
		dir = path.substr(0, item);

	return dir;
}
