#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

#include "Sprite.h"

using namespace std;

Sprite::Sprite()
{
}
Sprite::~Sprite()
{
	free();
}
void Sprite::free()
{
	if(texture != NULL)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Sprite::init(SDL_Renderer* gRenderer)
{
	free();

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
	{
		printf("Unable to load image with path %s! SDL_image error : %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(texture == NULL)
		{
			printf("Unable to create texture from image with path %s! SDL error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			width = loadedSurface->w;
			height = loadedSurface->h;
			clippingRect = {0, 0, width, height};
			renderQuad = {0, 0, width*scale, height*scale};
			flip = SDL_FLIP_NONE;
		}

		SDL_FreeSurface(loadedSurface);
	}
}

void Sprite::draw(SDL_Renderer* gRenderer, int x, int y)
{
	renderQuad.x = x;
	renderQuad.y = y;
	SDL_RenderCopyEx(gRenderer, texture, &clippingRect, &renderQuad, rotation, NULL, flip);
}
