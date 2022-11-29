#pragma once

#include "Tile.h"

#include <vector>

class GridElement
{
	public:
		GridElement(std::vector<Tile*> possibleTiles);
		bool narrow(GridElement* adjacent[]);
		void pick();
		bool collapsed;
		Tile* tile;
		std::vector<Tile*> possibilities;
};
