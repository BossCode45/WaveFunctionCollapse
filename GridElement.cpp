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

bool checkSide(Tile* thisTile, GridElement* otherElement, int dir)
{
	if(otherElement == NULL)
		return(false);
	else if(!otherElement->collapsed)
	{
		for(Tile* t : otherElement->possibilities)
		{
			if(vecIn(t->neighbors[(dir+2)%4], thisTile))
			{
				return(false);
			}
		}
		return(true);
	}
	else if(vecIn(otherElement->tile->neighbors[(dir+2)%4], thisTile))
		return(false);
	return(true);
}

bool GridElement::narrow(GridElement* adjacent[])
{
	bool ret = false;
	vector<int> toRemove;
	for(int i = 0; i < possibilities.size(); i++)
	{
		Tile* t = possibilities[i];
		for(int dir = 0; dir < 4; dir++)
		{
			if(checkSide(t, adjacent[dir], dir))
			{
				toRemove.push_back(i);
				break;
			}
		}
	}
	if(toRemove.size() > possibilities.size())
	{
		cout << "uh oh, toRemove is massive\n";
	}
	int removed = 0;
	for(int i : toRemove)
	{
		if(i-removed > possibilities.size() || i-removed < 0)
		{
			cout << "uh oh, erasing something that doesn't exist\n";
		}
		possibilities.erase(possibilities.begin() + (i-removed));
		removed++;
		ret = true;
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
	return ret;
}

void GridElement::pick()
{
	int index = rand()%possibilities.size(); //(possibilities.size()!=1)? rand()%(possibilities.size()-1) : 0;
	tile = possibilities[index];
	collapsed = true;
}
