#include "Scene.hpp"
#include <fstream>
#include <sstream>

Scene::Scene(std::string filePath):
	camera(), resolution(), ambientColor(), backgroundColor()
{

	std::stringstream ss;

	std::ifstream file(filePath);
	std::string line;
	while (std::getline(file, line))
	{
		ss.str(line);
		std::string type;
		ss >> type;

		if (type == "SPHERE")
		{

		}
		else if (type == "LIGHT")
		{

		}
		else if (type == "NEAR")
		{

		}
		else if (type == "LEFT")
		{

		}
		else if (type == "RIGHT")
		{

		}
		else if (type == "BOTTOM")
		{

		}
		else if (type == "TOP")
		{

		}
		else if (type == "RES")
		{

		}
		else if (type == "BACK")
		{

		}
		else if (type == "AMBIENT")
		{

		}
		else if (type == "OUTPUT")
		{

		}
	}
}