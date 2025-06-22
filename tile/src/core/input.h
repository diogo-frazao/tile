#pragma once
#include <stdint.h>
#include <array>
#include "lib.h"
#include <SDL_events.h>
#include <SDL_scancode.h>

// Game keys
inline const SDL_Scancode k_confirmKey = SDL_SCANCODE_RETURN;
inline const SDL_Scancode k_returnKey = SDL_SCANCODE_ESCAPE;

inline const int k_createSpriteOnSpritePreviewerClickMouseButton = SDL_BUTTON_LEFT;
inline const int k_deleteSpriteOnSpritePreviewerClickMouseButton = SDL_BUTTON_RIGHT;

inline const SDL_Scancode k_undoKey = SDL_SCANCODE_Z;
inline const SDL_Scancode k_redoKey = SDL_SCANCODE_Y;

inline const SDL_Scancode k_snapKey = SDL_SCANCODE_LSHIFT;


struct KeyState
{
	bool isDown = false;
	bool justPressed = false;
	bool justReleased = false;
	uint8_t pressedCounter = 0;
};

struct MouseState
{
	std::array<KeyState, 3> mouseButtonsState;
	float mouseWheelScroll = 0.f;
};

void handleKeyboardInput(SDL_Event& ev);
void handleMouseInput(SDL_Event& ev);
void resetKeyboardAndMouseInput();

bool isKeyDown(SDL_Scancode index);
bool wasKeyPressedThisFrame(SDL_Scancode index);
bool wasKeyReleasedThisFrame(SDL_Scancode index);

bool isMouseButtonDown(uint8_t mouseButtonIndex);
bool wasMouseButtonPressedThisFrame(uint8_t mouseButtonIndex);
bool wasMouseButtonReleasedThisFrame(uint8_t mouseButtonIndex);
IVec2 getMousePosition();

inline IVec2 s_mousePositionThisFrame;
inline std::array<KeyState, 290> s_keyboardInputState;
inline MouseState s_mouseInputState;
inline SDL_Scancode s_keyPressedThisFrame = SDL_SCANCODE_UNKNOWN;
