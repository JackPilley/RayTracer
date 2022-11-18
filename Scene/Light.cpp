#include "Light.hpp"

Light::Light(std::string name, float posX, float posY, float posZ, float r, float g, float b):
	name(name),
	position(posX, posY, posZ),
	intensity(r, g, b)
{}