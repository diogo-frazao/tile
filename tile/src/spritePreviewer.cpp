#include "spritePreviewer.h"
#include "core/log.h"
#include "core/debugUtils.h"
#include "core/input.h"
#include "screens.h"

SpritePreviewer::SpritePreviewer(const Vec2& startLocation)
{
	_locationToStartGrid = startLocation;
	_spritesToPreview.reserve(5);
	_backgroundSprite = textures::getSprite(PREVIEWER_BG);
}

bool didPressSpriteButton(const RectCollider& rect)
{
	if (wasMouseButtonReleasedThisFrame(SDL_BUTTON_LEFT))
	{
		return pointInRect(s_mousePositionThisFrame, rect);
	}

	return false;
}

void SpritePreviewer::render()
{
	if (!_isVisible)
	{
		return;
	}

	static const uint8_t offset = _backgroundSprite.size.x;
	static Vec2 targetRect = { _backgroundSprite.size.x - 1.f, _backgroundSprite.size.y - 1.f };

	uint8_t k_maxSpritesPerRow = 4;
	uint8_t spritesThisRow = 0;
	uint8_t numColumns = 0;

	for (Sprite& sprite : _spritesToPreview)
	{
		if (spritesThisRow < k_maxSpritesPerRow)
		{
			spritesThisRow++;
		}
		else
		{
			spritesThisRow = 1;
			numColumns++;
		}

		// Position sprite at top left of background sprite
		sprite.position = _locationToStartGrid;
		sprite.position.x += spritesThisRow * offset;
		sprite.position.y += numColumns * offset;

		_backgroundSprite.position = sprite.position;
		renderSprite(_backgroundSprite);

		RectCollider backgroundSpriteRect{_backgroundSprite.position, _backgroundSprite.size};
		debugDrawRect(backgroundSpriteRect);
		if (didPressSpriteButton(backgroundSpriteRect))
		{
			MainScreen::_spriteInHand = Sprite{sprite};
		}

		// Center sprite inside the background if smaller than background
		bool willScaleSprite = ((float)sprite.size.x > targetRect.x) || ((float)sprite.size.y > targetRect.y);
		if (!willScaleSprite)
		{
			sprite.position.x += (targetRect.x - sprite.size.x) / 2;
			sprite.position.y += (targetRect.y - sprite.size.y) / 2;
		}

		renderSpriteInsideRect(sprite, targetRect);
	}
}