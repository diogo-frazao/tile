#pragma once
#include <stdint.h>
#include <array>

namespace user_defaults
{
	struct Settings
	{
		int8_t resolutionIndex = 1;
		int8_t isFullscreen = 0;

		Settings() = default;
		Settings(int8_t res, int8_t fullscreen) : resolutionIndex{ res }, isFullscreen{ fullscreen } {};
	};

	Settings _currentSettings;

	std::array<const char*, 5> _resolutions{
		"<  320x180  >",
		"<  960x540  >",
		"< 1280x720  >",
		"< 1920x1080 >",
		"< 2560x1440 >"
	};

	inline void applySettings(const Settings& settings)
	{
		_currentSettings = settings;
	}
}