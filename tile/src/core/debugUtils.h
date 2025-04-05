#pragma once
#include "app.h"
#include "lib.h"
#include <SDL_render.h>

inline void debugDrawRect(const RectCollider& rect)
{
    if (s_debugCollidersEnabled)
    {
        SDL_SetRenderDrawColor(s_renderer, rect.debugColor.r, rect.debugColor.g, rect.debugColor.b, rect.debugColor.a);
        SDL_FRect debugRect{ rect.centerPoint.x, rect.centerPoint.y, rect.size.x, rect.size.y };
        SDL_RenderDrawRectF(s_renderer, &debugRect);
        SDL_SetRenderDrawColor(s_renderer, 0,0,0,1);
    }
}