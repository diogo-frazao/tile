#include "spritePreviewer.h"

void SpritePreviewer::start()
{
	_spritesToPreview.reserve(5);
}

void SpritePreviewer::render()
{
	if (!s_isActive)
	{
		return;
	}

	uint8_t k_maxSpritesPerRow = 3;
	uint8_t offset = 9;
	int spritesThisRow = 0;
	int numColumns = 0;
	Vec2 startingLocation{ 50, 50 };

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

		sprite.position = startingLocation;
		sprite.position.x += spritesThisRow * offset;
		sprite.position.y += numColumns * offset;

		renderSprite(sprite);
	}
}