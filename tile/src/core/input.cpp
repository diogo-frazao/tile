#include "input.h"
#include "log.h"
#include "app.h"

void calculateMousePositionThisFrame()
{
	s_mousePositionThisFrame = getMousePosition();
}

IVec2 getMousePosition()
{
	int32_t w = 0;
	SDL_GetWindowSize(s_window, &w, nullptr);
	uint8_t screenfactor = w / k_screenWidth;

	int32_t mx = 0;
	int32_t my = 0;
	SDL_GetMouseState(&mx, &my);

	int32_t mouseX = mx / screenfactor;
	int32_t mouseY = my / screenfactor;
	return IVec2(mouseX, mouseY);
}

void handleKeyboardInput(SDL_Event& ev)
{
	if (ev.type != SDL_KEYDOWN && ev.type != SDL_KEYUP)
	{
		return;
	}

	SDL_Scancode key = ev.key.keysym.scancode;
	if (key == SDL_SCANCODE_UNKNOWN)
	{
		D_ASSERT(false, "Unkown key pressed");
		return;
	}

	KeyState& keyState = s_keyboardInputState[key];

	if (ev.type == SDL_KEYDOWN)
	{
		keyState.isDown = true;
		keyState.pressedCounter++;
		keyState.justPressed = keyState.pressedCounter == 1;
		if (keyState.justPressed)
		{
			s_keyPressedThisFrame = key;
		}
	}
	else if (ev.type == SDL_KEYUP)
	{
		keyState.isDown = false;
		keyState.pressedCounter = 0;
		keyState.justReleased = true;
	}
}

void handleMouseInput(SDL_Event& ev)
{
	calculateMousePositionThisFrame();

	if (ev.type == SDL_MOUSEWHEEL)
	{
		s_mouseInputState.mouseWheelScroll = static_cast<float>(ev.wheel.y);
	}

	if (ev.type != SDL_MOUSEBUTTONDOWN && ev.type != SDL_MOUSEBUTTONUP)
	{
		return;
	}

	uint8_t buttonIndex = ev.button.button;
	KeyState& mouseButtonState = s_mouseInputState.mouseButtonsState[buttonIndex - 1];

	if (ev.type == SDL_MOUSEBUTTONDOWN)
	{
		mouseButtonState.isDown = true;
		mouseButtonState.pressedCounter++;
		mouseButtonState.justPressed = mouseButtonState.pressedCounter == 1;
	}
	else if (ev.type == SDL_MOUSEBUTTONUP)
	{
		mouseButtonState.isDown = false;
		mouseButtonState.pressedCounter = 0;
		mouseButtonState.justReleased = true;
	}
}

void resetKeyboardAndMouseInput()
{
	// Reset was just press / was just release keys
	for (KeyState& keyState : s_keyboardInputState)
	{
		keyState.justPressed = 0;
		keyState.justReleased = 0;
	}

	for (KeyState& keyState : s_mouseInputState.mouseButtonsState)
	{
		keyState.justPressed = 0;
		keyState.justReleased = 0;
	}

	s_keyPressedThisFrame = SDL_SCANCODE_UNKNOWN;
	s_mouseInputState.mouseWheelScroll = 0;
}

bool isKeyDown(SDL_Scancode index)
{
	if (index < 0 || index > s_keyboardInputState.size() - 1)
	{
		D_ASSERT(false, "Trying to access invalid array index");
		return false;
	}

	return s_keyboardInputState[index].isDown;
}

bool wasKeyPressedThisFrame(SDL_Scancode index)
{
	if (index < 0 || index > s_keyboardInputState.size() - 1)
	{
		D_ASSERT(false, "Trying to access invalid array index");
		return false;
	}

	return s_keyboardInputState[index].justPressed;
}

bool wasKeyReleasedThisFrame(SDL_Scancode index)
{
	if (index < 0 || index > s_keyboardInputState.size() - 1)
	{
		D_ASSERT(false, "Trying to access invalid array index");
		return false;
	}

	return s_keyboardInputState[index].justReleased;
}

bool isMouseButtonDown(uint8_t mouseButtonIndex)
{
	// This function expects a SDL_Mouse button definition. Left mouse = 1, Middle = 2 and Right = 3. 
	// Because of this we have to remove 1 to index the buttons state (otherwise we would have an empty element at index 0
	uint8_t index = mouseButtonIndex - 1;

	if (index < 0 || index > s_mouseInputState.mouseButtonsState.size() - 1)
	{
		D_ASSERT(false, "Trying to access invalid array index");
		return false;
	}

	return s_mouseInputState.mouseButtonsState[mouseButtonIndex - 1].isDown;
}

bool wasMouseButtonPressedThisFrame(uint8_t mouseButtonIndex)
{
	// This function expects a SDL_Mouse button definition. Left mouse = 1, Middle = 2 and Right = 3. 
	// Because of this we have to remove 1 to index the buttons state (otherwise we would have an empty element at index 0
	uint8_t index = mouseButtonIndex - 1;

	if (index < 0 || index > s_mouseInputState.mouseButtonsState.size() - 1)
	{
		D_ASSERT(false, "Trying to access invalid array index");
		return false;
	}

	return s_mouseInputState.mouseButtonsState[index].justPressed;

}

bool wasMouseButtonReleasedThisFrame(uint8_t mouseButtonIndex)
{
	// This function expects a SDL_Mouse button definition. Left mouse = 1, Middle = 2 and Right = 3. 
	// Because of this we have to remove 1 to index the buttons state (otherwise we would have an empty element at index 0
	uint8_t index = mouseButtonIndex - 1;

	if (index < 0 || index > s_mouseInputState.mouseButtonsState.size() - 1)
	{
		D_ASSERT(false, "Trying to access invalid array index");
		return false;
	}

	return s_mouseInputState.mouseButtonsState[mouseButtonIndex - 1].justReleased;
}


