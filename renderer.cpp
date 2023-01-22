#include <SDL_image.h>
#include <SDL.h>

#include "renderer.h"
#include <iostream>
using namespace std;

window_renderer::window_renderer(const char* window_name, int window_width, int window_height)
	:window_var(NULL), renderer_var(NULL)
{
	window_var = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
	if (window_var == NULL) {
		cout << "Window did not open. Error: " << SDL_GetError() << endl;
	}
	renderer_var = SDL_CreateRenderer(window_var, -1, SDL_RENDERER_ACCELERATED);
}

SDL_Texture* window_renderer::load_texture(const char* file_path) {
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer_var, file_path);
	if (texture == NULL) {
		cout << "Failed to load texture. Error: " << SDL_GetError() << endl;
	}
	return texture;
}

void window_renderer::clean_up() {
	SDL_DestroyWindow(window_var);
}

void window_renderer::clear() {
	SDL_RenderClear(renderer_var);
}

void window_renderer::render(int x, int y, SDL_Texture* texture) {
	SDL_Rect src{};
	src.x = 0;
	src.y = 0;
	src.w;
	src.h;
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
	SDL_Rect dst{};
	dst.x = x;
	dst.y = y;
	dst.w = (src.w * 2);
	dst.h = (src.h * 2);
	SDL_RenderCopy(renderer_var, texture, &src, &dst);
}

void window_renderer::display() {
	SDL_RenderPresent(renderer_var);
}

