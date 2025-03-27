#pragma once
#include <stdint.h>
#include <SDL_pixels.h>
#include <SDL_rect.h>
#include <string>
#include "log.h"

inline SDL_Rect s_src{};
inline SDL_FRect s_dest{};

struct IVec2
{
	int32_t x = 0;
	int32_t y = 0;

	IVec2() = default;
	IVec2(int x, int y) : x(x), y(y) {};
};

struct Vec2
{
	float x = 0;
	float y = 0;

	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y) {};
	Vec2(const IVec2& vec) : x(static_cast<float>(vec.x)), y(static_cast<float>(vec.y)) {};
};

struct RectCollider
{
	Vec2 centerPoint;
	Vec2 size;
	SDL_Color debugColor{252, 15, 192, 255};

	RectCollider() = default;
	RectCollider(const Vec2& pos, const Vec2& size) : centerPoint(pos), size(size) {};
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

inline float max(float a, float b)
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

inline int clamp(int a, int min, int max)
{
	if (a < min)
	{
		return min;
	}
	else if (a > max)
	{
		return max;
	}

	return a;
}

inline float mapZeroToOne(int value, float min, float max)
{
	int clampedValue = clamp(value, (int)min, (int)max);
	return (clampedValue - min) / (max - min);
}

inline bool pointInRect(const IVec2& point, const RectCollider& rect)
{
	return point.x >= rect.centerPoint.x &&
		point.x <= rect.centerPoint.x + rect.size.x &&
		point.y >= rect.centerPoint.y &&
		point.y <= rect.centerPoint.y + rect.size.y;
}

inline int convertStringToInt(std::string_view word)
{
	int result = 0;
	for (auto c : word)
	{
		if (c >= '0' && c <= '9')
		{
			result = result * 10 + (c - '0');
		}
	}

	if (result == 0)
	{
		D_LOG(ERROR, "Invalid string to convert to int");
	}

	return result; 
}