#pragma once
#include <stdint.h>
#include <array>
#include <SDL.h>
#include "lib.h"

struct KeyState
{
	bool isDown;
	bool justPressed;
	bool justReleased;
	uint8_t pressedCounter = 0;
};

struct MouseState
{
	std::array<KeyState, 3> mouseButtonsState;
	float mouseWheelScroll;
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

inline std::array<KeyState, 290> s_keyboardInputState;
inline MouseState s_mouseInputState;
