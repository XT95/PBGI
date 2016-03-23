#include "Utils/loadfile.h"

bool loadFile(const char *name, std::string &str)
{
	std::ifstream ifs(name);

	str.clear();

	if(ifs)
	{
		ifs.seekg (0, ifs.end);
		str.reserve(ifs.tellg());
		ifs.seekg (0, ifs.beg);

		str.assign(std::istreambuf_iterator<char>(ifs),
			       std::istreambuf_iterator<char>());
	}
	else
	{
		std::cerr << "The file " << name << " couldn't be opened." << std::endl;
		return false;
	}
	
	return true;
}
