#include "core/spriteHandler.h"
#include <vector>
#include <array>

class TilePlayground
{
public:
	TilePlayground()
	{
		_placedSprites.reserve(50);
		_spriteInHandIndex = k_invalidIndex;
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

		void invalidate()
		{
			sprite.invalidate();
			layer = EMPTY;
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

		std::array<int16_t, 30> _placedSpritesIndexesHistory;
		int8_t _lastPlacedSpriteHistoryIndex = k_invalidIndex;

		std::array<PlaceableSprite, 30> _removedSprites;
		int8_t _lastRemovedSpriteIndex = k_invalidIndex;

		void pushSpriteIndexToHistory(int16_t spriteIndex)
		{
			_lastPlacedSpriteHistoryIndex = (_lastPlacedSpriteHistoryIndex + 1) % _placedSpritesIndexesHistory.size();
			_placedSpritesIndexesHistory[_lastPlacedSpriteHistoryIndex] = spriteIndex;
		}

		void moveSpriteToRemovedSprites(PlaceableSprite&& other)
		{
			_lastRemovedSpriteIndex = (_lastRemovedSpriteIndex + 1) % _removedSprites.size();
			_removedSprites[_lastRemovedSpriteIndex] = other;
		}

		int16_t getLastPlacedSpriteIndex()
		{
			if (!isIndexValid(_lastPlacedSpriteHistoryIndex))
			{
				return -1;
			}
			return _placedSpritesIndexesHistory[_lastPlacedSpriteHistoryIndex];
		}

		void deleteLastPlacedSpriteHistoryIndex()
		{
			_placedSpritesIndexesHistory[_lastPlacedSpriteHistoryIndex] = k_invalidIndex;

			if (_lastPlacedSpriteHistoryIndex == 0)
			{
				int lastIndex = _placedSpritesIndexesHistory.size() - 1;
				bool hasValidSpriteAtLastIndex = isIndexValid(_placedSpritesIndexesHistory[lastIndex]);
				_lastPlacedSpriteHistoryIndex = hasValidSpriteAtLastIndex ? lastIndex : k_invalidIndex;
			}
			else
			{
				--_lastPlacedSpriteHistoryIndex;
			}
		}

		void deleteLastRemovedSpriteIndex()
		{
			_removedSprites[_lastRemovedSpriteIndex].invalidate();

			if (_lastRemovedSpriteIndex == 0)
			{
				int lastIndex = _removedSprites.size() - 1;
				bool hasValidSpriteAtLastIndex = _removedSprites[_lastRemovedSpriteIndex].isValid();
				_lastRemovedSpriteIndex = hasValidSpriteAtLastIndex ? lastIndex : k_invalidIndex;
			}
			else
			{
				--_lastRemovedSpriteIndex;
			}
		}
	};

	Sprite& getSpriteInHand()
	{
		D_ASSERT((_spriteInHandIndex > k_invalidIndex), "Can't access invalid sprite in hand");
		return _placedSprites[_spriteInHandIndex].sprite;
	}

	PlaceableSprite& getPlaceableSpriteInHand()
	{
		D_ASSERT((_spriteInHandIndex > k_invalidIndex), "Can't access invalid sprite in hand");
		return _placedSprites[_spriteInHandIndex];
	}

	bool hasSpriteInHand()
	{
		return isIndexValid(_spriteInHandIndex);
	}

	void clearSpriteInHand()
	{
		_spriteInHandIndex = k_invalidIndex;
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
			addSpriteToRenderAndPutSpriteInHand(sprite);
		}
		
	}

	void addSpriteToRender(PlaceableSprite&& placeableSprite)
	{
		if (_placedSprites.size() == 0)
		{
			_placedSprites.emplace_back(placeableSprite);
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
				return;
			}
		}

		if (indexToPlaceSprite == (_placedSprites.size() - 1))
		{
			_placedSprites.emplace_back(placeableSprite);
			_undoRedoSprites.pushSpriteIndexToHistory(_placedSprites.size() - 1);
			return;
		}
	}

	void addSpriteToRenderAndPutSpriteInHand(const PlaceableSprite& placeableSprite)
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
		int16_t lastPlacedSpriteIndex = _undoRedoSprites.getLastPlacedSpriteIndex();
		if (!isIndexValid(lastPlacedSpriteIndex))
		{
			return;
		}

		_undoRedoSprites.moveSpriteToRemovedSprites(std::move(_placedSprites[lastPlacedSpriteIndex]));
		_placedSprites.erase(_placedSprites.begin() + lastPlacedSpriteIndex);
		_undoRedoSprites.deleteLastPlacedSpriteHistoryIndex();

		clearSpriteInHand();
	}

	void redoLastRemovedSprite()
	{
		if(!isIndexValid(_undoRedoSprites._lastRemovedSpriteIndex))
		{
			return;
		}

		addSpriteToRender(std::move(_undoRedoSprites._removedSprites[_undoRedoSprites._lastRemovedSpriteIndex]));
		_undoRedoSprites.deleteLastRemovedSpriteIndex();
	}

	UndoRedoPlaceSprites _undoRedoSprites;
	int16_t _spriteInHandIndex;
	std::vector<PlaceableSprite> _placedSprites;
};