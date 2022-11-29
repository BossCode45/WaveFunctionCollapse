#include <SDL2/SDL.h>

#include "Tile.h"
#include "GridElement.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <iostream>

using namespace std;

bool init();
void close();
void mainLoop();

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 960;

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Event e;


string TConns[] = {"ABA", "ABA", "AAA", "ABA"};
string BConns[] = {"AAA", "AAA", "AAA", "AAA"};
string AConns[] = {"ABA", "ABA", "ABA", "ABA"};
string CConns[] = {"ABA", "AAA", "AAA", "ABA"};
string PConns[] = {"ABA", "AAA", "AAA", "AAA"};
string LConns[] = {"ABA", "AAA", "ABA", "AAA"};
Tile t1 = Tile("./images_s/T.png", 0, TConns);
Tile t2 = Tile("./images_s/T.png", 1, TConns);
Tile t3 = Tile("./images_s/T.png", 2, TConns);
Tile t4 = Tile("./images_s/T.png", 3, TConns);
Tile blank = Tile("./images_s/blank.png", 0, BConns);
Tile all = Tile("./images_s/all.png", 0, AConns);
Tile c1 = Tile("./images_s/corner.png", 0, CConns);
Tile c2 = Tile("./images_s/corner.png", 1, CConns);
Tile c3 = Tile("./images_s/corner.png", 2, CConns);
Tile c4 = Tile("./images_s/corner.png", 3, CConns);
Tile p1 = Tile("./images_s/point.png", 0, PConns);
Tile p2 = Tile("./images_s/point.png", 1, PConns);
Tile p3 = Tile("./images_s/point.png", 2, PConns);
Tile p4 = Tile("./images_s/point.png", 3, PConns);
Tile l1 = Tile("./images_s/line.png", 0, LConns);
Tile l2 = Tile("./images_s/line.png", 1, LConns);
vector<Tile*> possibleTiles = {&t1, &t2, &t3, &t4, &blank};//, &all, &c1, &c2, &c3, &c4, &p1, &p2, &p3, &p4, &l1, &l2};

vector<vector<GridElement>> grid;

bool init()
{
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0)
	{
		printf("SDL couldn't init, error: %s\n",  SDL_GetError());
		return(false);
	}
	else
	{
		gWindow = SDL_CreateWindow("WFC", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
		{
			printf("Window can't be created, error: %s\n", SDL_GetError());
			return(false);
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if(gRenderer == NULL)
			{
				printf("Renderer can't be created! error%s\n", SDL_GetError());
				return(false);
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return true;
}


void close()
{
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}

void narrowAdjacents(int x, int y)
{
	for(int dX = -1; dX < 2; dX++)
	{
		for(int dY = -1; dY < 2; dY++)
		{
			if(!(x + dX >= 0 && x + dX < 20 && y + dY >= 0 && y + dY < 20))
				continue;
			if(dX + dY == 0 || dX + dY == 2 || dX + dY == -2)
				continue;

			GridElement* up = (y+dY>0)? &grid[x+dX][y+dY-1] : NULL;
			GridElement* right = (x+dX<19)? &grid[x+dX+1][y+dY] : NULL;
			GridElement* down = (y+dY<19)? &grid[x+dX][y+dY+1] : NULL;
			GridElement* left = (x+dX>0)? &grid[x+dX-1][y+dY] : NULL;
			GridElement* arr[4] = {up, right, down, left};
			if(grid[x + dX][y + dY].narrow(arr))
			{
				narrowAdjacents(x + dX, y + dY);
			}
		}
	}
}

void mainLoop()
{
	int picked = 1;
	GridElement d = GridElement(possibleTiles);
	while(true)
	{
		//KEYS
		while(SDL_PollEvent(&e)!=0)
		{
			switch(e.type)
			{
				case SDL_KEYDOWN:
					if(e.key.keysym.sym == SDLK_ESCAPE)
						return;
					break;
				case SDL_QUIT:
					return;
			}
		}
		SDL_SetRenderDrawColor(gRenderer, 0xAA, 0xAA, 0xAA, 0xFF);
		SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_WIDTH};
		SDL_RenderFillRect(gRenderer, &rect);

		while(picked < 400)
		{
			int sX = -1, sY = -1, minEntropy = -1;
			for(int x = 0; x < 20; x++)
			{
				for(int y = 0; y < 20; y++)
				{
					if(!grid[x][y].collapsed && (minEntropy == -1 || grid[x][y].possibilities.size() < minEntropy))
					{
						sX = x;
						sY = y;
						minEntropy = grid[x][y].possibilities.size();
					}
				}
			}
			if(!grid[sX][sY].collapsed)
			{
				picked++;
				grid[sX][sY].pick();
				narrowAdjacents(sX, sY);
				if(picked == 400)
				{
					cout << "Finished\n";
				}
				break;
			}
		}

		for(int x = 0; x < 20; x++)
		{
			for(int y = 0; y < 20; y++)
			{
				if(grid[x][y].collapsed)
				{
					grid[x][y].tile->draw(gRenderer, x*48, y*48);
				}
			}
		}

		#ifdef DEBUG_LINES
		SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
		for(int x = 48; x < 960; x+= 48)
		{
			SDL_RenderDrawLine(gRenderer, x, 0, x, 960);
		}
		for(int y = 48; y < 960; y+= 48)
		{
			SDL_RenderDrawLine(gRenderer, 0, y, 960, y);
		}
		#endif

		SDL_RenderPresent(gRenderer);

	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	if(!init())
	{
		printf("Couldn't init");
	}
	else
	{
		SDL_RenderClear(gRenderer);

		for(Tile* tile : possibleTiles)
		{
			tile->init(gRenderer);
			tile->findPossibles(possibleTiles);
		}

		for(int x = 0; x < 20; x++)
		{
			std::vector<GridElement> col;
			for(int y = 0; y < 20; y++)
			{
				GridElement e = GridElement(possibleTiles);
				col.push_back(e);
			}
			grid.push_back(col);
		}

		int sX = 10, sY = 10;
		grid[sX][sY].pick();
		for(int dX = -1; dX < 2; dX++)
		{
			for(int dY = -1; dY < 2; dY++)
			{
				if(!(sX + dX > 0 && sX + dX < 20 && sY + dY > 0 && sY + dY < 20))
					continue;
				if(dX + dY == 0 || dX + dY == 2 || dX + dY == -2)
					continue;

				GridElement* up = (sY+dY>0)? &grid[sX+dX][sY+dY-1] : NULL;
				GridElement* right = (sX+dX<19)? &grid[sX+dX+1][sY+dY] : NULL;
				GridElement* down = (sY+dY<19)? &grid[sX+dX][sY+dY+1] : NULL;
				GridElement* left = (sX+dX>0)? &grid[sX+dX-1][sY+dY] : NULL;
				GridElement* arr[4] = {up, right, down, left};
				grid[sX + dX][sY + dY].narrow(arr);
			}
		}


		mainLoop();
	}
	close();
	return 0;
}
