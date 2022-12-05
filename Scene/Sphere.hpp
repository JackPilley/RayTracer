#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Sphere
{
private:
	std::string name;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;

	float ambientFactor;
	float diffuseFactor;
	float specularFactor;
	float reflectanceFactor;

	uint32_t specularExponent;

	glm::mat4x4 inverse;
	glm::mat4x4 inverseTranspose;

public:
	Sphere(std::string name, float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float r, float g, float b, float ka, float kd, float ks, float kr, uint32_t specExp);

	const glm::vec3& GetPosition() const { return position; }
	const glm::vec3& GetScale() const { return scale; }
	const glm::vec3& GetColor() const { return color; }

	float GetAmbientFactor() const { return ambientFactor; }
	float GetDiffuseFactor() const { return diffuseFactor; }
	float GetSpecularFactor() const { return specularFactor; }
	float GetReflectanceFactor() const { return reflectanceFactor; }

	uint32_t GetSpecularExponent() const { return specularExponent; }

	const glm::mat4x4& GetInverseMatrix() const { return inverse; }
	const glm::mat4x4& GetInverseTransposeMatrix() const { return inverseTranspose; }

};