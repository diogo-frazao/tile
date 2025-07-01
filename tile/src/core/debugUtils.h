#pragma once
#include "app.h"
#include "lib.h"
#include <SDL_render.h>

inline void debugDrawRect(const RectCollider& rect)
{
    if (!s_debugCollidersEnabled)
    {
        return;
    }

    SDL_SetRenderDrawColor(s_renderer, rect.debugColor.r, rect.debugColor.g, rect.debugColor.b, rect.debugColor.a);
    SDL_FRect debugRect{ rect.topLeftPoint.x, rect.topLeftPoint.y, rect.size.x, rect.size.y };
    SDL_RenderDrawRectF(s_renderer, &debugRect);
    SDL_SetRenderDrawColor(s_renderer, 0,0,0,1);
}

inline void debugDrawPoint(const Vec2& pos)
{
    if (!s_debugCollidersEnabled)
    {
        return;
    }

    SDL_SetRenderDrawColor(s_renderer, 255, 0, 0, 255);
    SDL_RenderDrawPointF(s_renderer, pos.x, pos.y);
    SDL_SetRenderDrawColor(s_renderer, 0,0,0,1);
}

inline void debugDrawGrid(const uint16_t gridSize)
{
    if (!s_debugCollidersEnabled)
    {
        return;
    }

    SDL_SetRenderDrawColor(s_renderer, 255, 0, 0, 255);
    for (int16_t x = 16; x < k_screenWidth; x += 16)
    {
        SDL_RenderDrawLine(s_renderer, x, 0, x, k_screenHeight);
        SDL_RenderDrawLine(s_renderer, 0, x, k_screenWidth, x);
    }
    SDL_SetRenderDrawColor(s_renderer, 0,0,0,1);
}