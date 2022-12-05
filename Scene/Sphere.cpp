#include <glm/ext/matrix_transform.hpp>
#include "Sphere.hpp"

Sphere::Sphere(std::string name, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float r, float g, float b, float ka, float kd, float ks, float kr, uint32_t specExp):
	name(name),
	position(posX, posY, posZ),
	scale(scaleX, scaleY, scaleZ),
	color(r, g, b),
	ambientFactor(ka),
	diffuseFactor(kd),
	specularFactor(ks),
	reflectanceFactor(kr),
	specularExponent(specExp),
	inverse(1.0f)
{
	inverse = glm::translate(inverse, position);
	inverse = glm::scale(inverse, scale);
	inverse = glm::inverse(inverse);

	inverseTranspose = glm::transpose(inverse);
}