#include "spritePreviewer.h"

void SpritePreviewer::start()
{
	_spritesToPreview.reserve(5);
	_backgroundSprite = textures::getSprite(PREVIEWER_BG);
}

void SpritePreviewer::render()
{
	if (!s_isActive)
	{
		return;
	}

	uint8_t k_maxSpritesPerRow = 6;
	uint8_t offset = 9;
	int spritesThisRow = 0;
	int numColumns = 0;
	Vec2 startingLocation{ 50, 50 };
	Vec2 targetRect{ 8,8 };

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
		sprite.position = startingLocation;
		sprite.position.x += spritesThisRow * offset;
		sprite.position.y += numColumns * offset;

		_backgroundSprite.position = sprite.position;
		renderSprite(_backgroundSprite);

		// Center sprite inside the background if needed
		bool willScaleSprite = ((float)sprite.size.x > targetRect.x) || ((float)sprite.size.y > targetRect.y);
		if (!willScaleSprite)
		{
			sprite.position.x += (targetRect.x - sprite.size.x) / 2;
			sprite.position.y += (targetRect.y - sprite.size.y) / 2;
		}

		renderSpriteInsideRect(sprite, targetRect);
	}
}