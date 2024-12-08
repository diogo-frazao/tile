#pragma once
#include <stdint.h>

struct Vec2
{
	float x = 0;
	float y = 0;

	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y) {};
};

struct IVec2
{
	int32_t x = 0;
	int32_t y = 0;

	IVec2() = default;
	IVec2(int x, int y) : x(x), y(y) {};
};

inline float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

inline float lerp(int a, int b, float t)
{
	float aa = (float)a;
	float bb = (float)b;
	return aa + (bb - aa) * t;
}