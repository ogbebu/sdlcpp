#pragma once

#include <SDL3/SDL.h>
#include <cmath>

constexpr float PI = 3.14159265359f;

void draw_circle(SDL_Renderer* renderer, float cx, float cy, float radius)
{
    for (float y = -radius; y <= radius; y++) {
        float x = sqrtf(radius * radius - y * y);

        SDL_RenderLine(
            renderer,
            cx - x,
            cy + y,
            cx + x,
            cy + y
        );
    }
}

void draw_circle_outlined(SDL_Renderer* renderer, float cx, float cy, float radius)
{
    const int segments = 64;

    for (int i = 0; i < segments; i++) {
        float a1 = i * 2.0f * PI / segments;
        float a2 = (i + 1) * 2.0f * PI / segments;

        SDL_RenderLine(
            renderer,
            cx + cosf(a1) * radius,
            cy + sinf(a1) * radius,
            cx + cosf(a2) * radius,
            cy + sinf(a2) * radius
        );
    }
}

void draw_textured_circle(SDL_Renderer* renderer, SDL_Texture* texture, float cx, float cy, float radius)
{
    float diameter = radius * 2.0f;
    
    for (int y = 0; y < (int)diameter; y++) {
        float dy = y - radius;
        float halfWidth = sqrtf(radius * radius - dy * dy);

        SDL_FRect src = { radius - halfWidth, (float)y, halfWidth * 2.0f, 1.0f };
        SDL_FRect dst = { cx - halfWidth, cy - radius + y, halfWidth * 2.0f, 1.0f };

        SDL_RenderTexture(renderer, texture, &src, &dst);
    }
}