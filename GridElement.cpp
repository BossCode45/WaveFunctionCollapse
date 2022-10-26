#include "GridElement.h"

#include <algorithm>
#include <iostream>

using std::cout;
using std::vector;

#define vecIn(vec, element) std::find(vec.begin(), vec.end(), element) != vec.end()

GridElement::GridElement(std::vector<Tile*> possibleTiles)
{
	possibilities = possibleTiles;
	collapsed = false;
	tile = NULL;
}

void GridElement::narrow(GridElement* adjacent[])
{
	vector<int> toRemove;
	for(int i = 0; i < possibilities.size(); i++)
	{
		Tile* t = possibilities[i];
		if(!(adjacent[0] == NULL || !adjacent[0]->collapsed || vecIn(adjacent[0]->tile->neighbors[2], t)))
		{
			toRemove.push_back(i);
			continue;
		}
		if(!(adjacent[1] == NULL || !adjacent[1]->collapsed || vecIn(adjacent[1]->tile->neighbors[3], t)))
		{
			toRemove.push_back(i);
			continue;
		}
		if(!(adjacent[2] == NULL || !adjacent[2]->collapsed || vecIn(adjacent[2]->tile->neighbors[0], t)))
		{
			toRemove.push_back(i);
			continue;
		}
		if(!(adjacent[3] == NULL || !adjacent[3]->collapsed || vecIn(adjacent[3]->tile->neighbors[1], t)))
		{
			toRemove.push_back(i);
			continue;
		}
	}
	int removed = 0;
	for(int i : toRemove)
	{
		possibilities.erase(possibilities.begin() + (i-removed));
		removed++;
	}
	if(possibilities.size() < 1)
	{
		cout << "All possiblities failed, exiting\n";
		for(int i = 0; i < 4; i++)
		{
			if(!adjacent[i]->collapsed)
			{
				cout << "\tNot collapsed\n";
			}
			else
			{
				cout << "\tIMG: " << adjacent[i]->tile->path << ", ROTS: " << adjacent[i]->tile->rotation << "\n";
				cout << "\t\t" << adjacent[i]->tile->connections[(i+2)%4] << "\n";
			}
		}
		exit(1);
	}
}

void GridElement::pick()
{
	int index = (possibilities.size()!=1)? rand()%(possibilities.size()-1) : 0;
	tile = possibilities[index];
	collapsed = true;
}
