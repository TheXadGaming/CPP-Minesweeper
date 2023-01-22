#pragma once
#include <SDL_image.h>
#include <SDL.h>

class window_renderer {
	public:
		window_renderer(const char* window_name, int window_width, int window_height);
		SDL_Texture* load_texture(const char* file_path);
		void render(int x, int y, SDL_Texture* texture);
		void clean_up();
		void display();
		void clear();
	private:
		SDL_Window* window_var;
		SDL_Renderer* renderer_var;
};