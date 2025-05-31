#include "core/spriteHandler.h"
#include <vector>
#include <array>

class TilePlayground
{
public:
	TilePlayground()
	{
		_placedSprites.reserve(50);
		_spriteInHandIndex = -1;
	}

	struct PlaceableSprite
	{
		Sprite sprite;
		LayerType layer;

		PlaceableSprite() : layer(EMPTY) {};
		PlaceableSprite(const Sprite& sprite, const LayerType layer) : sprite(sprite), layer(layer) {};

		bool isValid()
		{
			return layer != EMPTY;
		}
	};

	class UndoRedoPlaceSprites
	{
	public:
		UndoRedoPlaceSprites() 
		{
			for (int16_t& n : _placedSpritesIndexesHistory)
			{
				n = k_invalidIndex;
			}
		};

		std::array<int16_t, 10> _placedSpritesIndexesHistory;
		std::array<PlaceableSprite, 10> _removedSprites;

		void pushSpriteIndexToHistory(int16_t spriteIndex)
		{
			for (uint8_t i = 0; i < _placedSpritesIndexesHistory.size(); ++i)
			{
				if (isIndexValid(_placedSpritesIndexesHistory[i]))
				{
					continue;
				}

				_placedSpritesIndexesHistory[i] = spriteIndex;
				return;
			}

			// If we get here, the array is full with valid placeable sprites
			// TODO: We will start overriding them in order
		}

		void moveSpriteToRemovedSprites(PlaceableSprite&& other)
		{
			for (uint8_t i = 0; i < _removedSprites.size(); ++i)
			{
				if (_removedSprites[i].isValid())
				{
					continue;
				}

				_removedSprites[i] = other;
				return;
			}

			// If we get here, the array is full with valid placeable sprites
			// TODO: We will start overriding them in order
		}

		int16_t getLastPlacedSpriteIndex()
		{
			for (int16_t i = _placedSpritesIndexesHistory.size() - 1; i >= 0; --i)
			{
				if (isIndexValid(_placedSpritesIndexesHistory[i]))
				{
					return _placedSpritesIndexesHistory[i];
				}
			}

			return k_invalidIndex;
		}

		void deleteLastPlacedSpriteIndex()
		{
			for (int16_t i = _placedSpritesIndexesHistory.size() - 1; i >= 0; --i)
			{
				if (isIndexValid(_placedSpritesIndexesHistory[i]))
				{
					_placedSpritesIndexesHistory[i] = k_invalidIndex;
					return;
				}
			}
		}
	};

	Sprite& getSpriteInHand()
	{
		D_ASSERT((_spriteInHandIndex > -1), "Can't access invalid sprite in hand");
		return _placedSprites[_spriteInHandIndex].sprite;
	}

	bool hasSpriteInHand()
	{
		return _spriteInHandIndex > -1;
	}

	void clearSpriteInHand()
	{
		_spriteInHandIndex = -1;
	}

	void replaceSpriteInHand(const PlaceableSprite& sprite)
	{
		D_ASSERT((_spriteInHandIndex > -1), "Can't access invalid sprite in hand");

		if (sprite.layer == _placedSprites[_spriteInHandIndex].layer)
		{
			_placedSprites[_spriteInHandIndex] = sprite;
		}
		else
		{
			// If the layers are different, we have to delete the old item and create a new one
			// Since the rendering order will change
			_placedSprites.erase(_placedSprites.begin() + _spriteInHandIndex);
			clearSpriteInHand();
			addSpriteToRender(sprite);
		}
		
	}

	void addSpriteToRender(const PlaceableSprite& placeableSprite)
	{
		if (hasSpriteInHand())
		{
			return;
		}

		if (_placedSprites.size() == 0)
		{
			_placedSprites.emplace_back(placeableSprite);
			_spriteInHandIndex = 0;
			_undoRedoSprites.pushSpriteIndexToHistory(0);
			return;
		}

		int16_t indexToPlaceSprite = -1;
		for (uint16_t i = 0; i < _placedSprites.size(); ++i)
		{
			if (_placedSprites[i].layer <= placeableSprite.layer)
			{
				indexToPlaceSprite++;
				continue;
			}
			else
			{
				indexToPlaceSprite = (indexToPlaceSprite == -1) ? 0 : indexToPlaceSprite;
				_placedSprites.insert(_placedSprites.begin() + indexToPlaceSprite, placeableSprite);
				_undoRedoSprites.pushSpriteIndexToHistory(indexToPlaceSprite);
				_spriteInHandIndex = indexToPlaceSprite;
				return;
			}
		}

		if (indexToPlaceSprite == (_placedSprites.size() - 1))
		{
			_placedSprites.emplace_back(placeableSprite);
			_spriteInHandIndex = _placedSprites.size() - 1;
			_undoRedoSprites.pushSpriteIndexToHistory(_placedSprites.size() - 1);
			return;
		}
	}

	void undoLastPlacedSprite()
	{
		int16_t lastPlacedSpriteindex = _undoRedoSprites.getLastPlacedSpriteIndex();
		if (!isIndexValid(lastPlacedSpriteindex))
		{
			return;
		}

		clearSpriteInHand();
		_undoRedoSprites.moveSpriteToRemovedSprites(std::move(_placedSprites[lastPlacedSpriteindex]));
		_placedSprites.erase(_placedSprites.begin() + lastPlacedSpriteindex);
		_undoRedoSprites.deleteLastPlacedSpriteIndex();
	}

	UndoRedoPlaceSprites _undoRedoSprites;
	int16_t _spriteInHandIndex;
	std::vector<PlaceableSprite> _placedSprites;
};