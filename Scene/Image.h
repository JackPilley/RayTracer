#pragma once
#include <cstdint>
#include <glm/vec3.hpp>

struct Color
{
	uint8_t r;
	uint8_t g;
	uint8_t b;

	Color() = default;

	Color(glm::vec3 col)
	{
		r = (uint8_t)(col.r * 255);
		g = (uint8_t)(col.g * 255);
		b = (uint8_t)(col.b * 255);
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
	uint32_t x;
	uint32_t y;

	Color* pixels;
};