
#include "includeExample.h"
#include "ProgramDirectory.h"

int main()
{
	auto modulepath = CProgramDirectory::GetModuleDirectory();
	SetCurrentDirectory(modulepath.c_str());

	GlfwMultipleLights example;
	return example.main();
}