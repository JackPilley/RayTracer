#pragma once
#include <cstdint>
#include <string>
#include <glm/vec3.hpp>
#include <utility>

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color() = default;

	Color(glm::vec3 col)
	{
		// Clamps the values to [0,1] and then converts to [0,255]
		// Explicit cast is because Visual Studio was warning about precision loss, but it isn't really needed
		r = (uint8_t)(std::fminf(col.r, 1.0f) * 255.0f);
		g = (uint8_t)(std::fminf(col.g, 1.0f) * 255.0f);
		b = (uint8_t)(std::fminf(col.b, 1.0f) * 255.0f);
	}

	Color(glm::u8vec3 col)
	{
		r = col.r;
		g = col.g;
		b = col.b;
	}
};

struct Image
{
	std::string name;

	uint32_t x;
	uint32_t y;

	Color* pixels;
};