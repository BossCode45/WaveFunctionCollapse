#pragma once

#include "vector"

#include "Sprite.h"

class Tile : public Sprite
{
	public:
		float x = 0;
		float y = 0;
	protected:
		void rotate(int times);
	public:
		Tile(std::string path, int rots, std::string connections[]);
		~Tile();
		void free();
		void findPossibles(std::vector<Tile*> tiles);
		std::string connections[4];
		std::vector<Tile*> neighbors[4];
		std::string reverseSTR(std::string str);
};
