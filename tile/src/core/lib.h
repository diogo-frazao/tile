#pragma once
#include <stdint.h>
#include <SDL_pixels.h>

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

struct RectCollider
{
	Vec2 centerPoint;
	Vec2 size;
	SDL_Color debugColor{252, 15, 192, 255};

	RectCollider() = default;
	RectCollider(const Vec2& pos, const Vec2& size) : centerPoint(pos), size(size) {};
};

enum DrawMode
{
	TOP_LEFT,
	TOP_CENTER,
	TOP_RIGHT
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

inline int max(int a, int b)
{
	if (a >= b)
	{
		return a;
	}
	return b;
}

inline int min(int a, int b)
{
	if (a <= b)
	{
		return a;
	}
	return b;
}

inline bool pointInRect(const IVec2& point, const RectCollider& rect)
{
	return point.x >= rect.centerPoint.x &&
		point.x <= rect.centerPoint.x + rect.size.x &&
		point.y >= rect.centerPoint.y &&
		point.y <= rect.centerPoint.y + rect.size.y;
}