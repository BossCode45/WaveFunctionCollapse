#include "Tile.h"
#include <iostream>

Tile::Tile(std::string path, int rots, std::string connections[4])
{
	this->path = path;
	for(int i = 0; i < 4; i++)
	{
		this->connections[i] = connections[i];
	}
	rotate(rots);
	scale = 4;
}
Tile::~Tile()
{
	free();
}
void Tile::free()
{
	Sprite::free();
}

void Tile::rotate(int times)
{
	rotation = times*90;
	for(int t = 0; t < times; t++)
	{
		std::string newConns[4];
		for(int i = 0; i < 4; i++)
		{
			newConns[i] = connections[(i+3)%4];
		}
		for(int i = 0; i < 4; i++)
		{
			connections[i] = newConns[i];
		}
	}
}


void Tile::findPossibles(std::vector<Tile*> tiles)
{
	for(Tile* tile : tiles)
	{
		if(reverseSTR(tile->connections[0]) == this->connections[2])
		{
			this->neighbors[2].push_back(tile);
		}
		if(reverseSTR(tile->connections[2]) == this->connections[0])
		{
			this->neighbors[0].push_back(tile);
		}
		if(reverseSTR(tile->connections[3]) == this->connections[1])
		{
			this->neighbors[1].push_back(tile);
		}
		if(reverseSTR(tile->connections[1]) == this->connections[3])
		{
			this->neighbors[3].push_back(tile);
		}
	}
}

std::string Tile::reverseSTR(std::string str)
{
	std::string newStr = "";
	for(int i = str.size()-1; i > -1; i--)
	{
		newStr.push_back(str.at(i));
	}
	return newStr;
}
