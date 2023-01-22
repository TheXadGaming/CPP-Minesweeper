#include <SDL_image.h>
#include <SDL.h>

#include "renderer.h"
#include <iostream>
#include <stdlib.h> 
using namespace std; 


const int BoardHeight = 12;
const int BoardWidth = 12;

const int ScreenHeight = BoardHeight * 64;
const int ScreenWidth = BoardWidth * 64;

const int has_bomb = 1;
const int no_bomb = 0;

const int red_tile = 2;
const int black_tile = 1;
const int white_tile = 0;

const int has_flag = 1;
const int no_flag = 0;

const int you_lost = 1;
const int you_won = 0;

window_renderer window("SDL2 Test", ScreenWidth, ScreenHeight);
SDL_Texture* texture_white_tile = window.load_texture("res/gfx/white_tile.png");
SDL_Texture* texture_black_tile = window.load_texture("res/gfx/black_tile.png");
SDL_Texture* texture_red_tile = window.load_texture("res/gfx/red_tile.png");
SDL_Texture* texture_bomb_tile = window.load_texture("res/gfx/bomb.png");
SDL_Texture* texture_flag_tile = window.load_texture("res/gfx/flag.png");
SDL_Texture* texture_number_tiles[10]{
	{},
	{window.load_texture("res/gfx/num_1.png")},
	{window.load_texture("res/gfx/num_2.png")},
	{window.load_texture("res/gfx/num_3.png")},
	{window.load_texture("res/gfx/num_4.png")},
	{window.load_texture("res/gfx/num_5.png")},
	{window.load_texture("res/gfx/num_6.png")},
	{window.load_texture("res/gfx/num_7.png")},
	{window.load_texture("res/gfx/num_8.png")}
};

bool game_running = true;
bool developer = true;
bool hit_bomb = false;

int mousex{};
int mousey{};

int tile_grid[BoardWidth + 1][BoardHeight + 1]{};
int bomb_grid[BoardWidth + 1][BoardHeight + 1]{};
int flag_grid[BoardWidth + 1][BoardHeight + 1]{};
int num_grid[BoardWidth + 1][BoardHeight + 1]{};

int tile_x{};
int tile_y{};

void render_grid();
void generate_bombs();
void generate_nums();
void display_debug();
void post_game(int state);
void check_won();

int	main(int argc, char** args) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl; //Check for Error.
		return 0;
	}

	generate_bombs();
	generate_nums();
	render_grid();

	while (game_running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				game_running = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				SDL_GetMouseState(&mousex, &mousey);
				tile_x = (mousex / 64) + 1;
				tile_y = (mousey / 64) + 1;
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (flag_grid[tile_x][tile_y] == no_flag) {
						tile_grid[tile_x][tile_y] = black_tile;
					}
					if (bomb_grid[tile_y][tile_x] == has_bomb) {
						game_running = false;
						hit_bomb = true;
					}
				}
				if (tile_grid[tile_x][tile_y] == white_tile) {
					if (event.button.button == SDL_BUTTON_RIGHT) {
						if (flag_grid[tile_x][tile_y] == no_flag) {
							flag_grid[tile_x][tile_y] = has_flag;
							render_grid();
							continue;
						}
						if (flag_grid[tile_x][tile_y] == has_flag) {
							flag_grid[tile_x][tile_y] = no_flag;
							render_grid();
							continue;
						}
					}
				}
				check_won();
				if (game_running) {
					render_grid();
				}
			}
		}
		window.display();
	}
	if (hit_bomb) {
		post_game(you_lost);
	}
	window.clean_up();
	SDL_Quit();
	return 0;
}

void check_won() {
	for (int x = 1; x <= BoardWidth; x++) {
		for (int y = 1; y <= BoardHeight; y++) {
			if (tile_grid[x][y] == black_tile && bomb_grid[y][x] == no_bomb) {
				continue;
			}
			if (tile_grid[x][y] == white_tile && bomb_grid[y][x] == no_bomb) {
				return;
			}
		}
	}
	post_game(you_won);
	game_running = false;
}

void generate_nums() {
	for (int x = 1; x <= BoardWidth; x++) {
		for (int y = 1; y <= BoardHeight; y++) {
			int bombs{ 0 };
			if (bomb_grid[x][y] == has_bomb) continue;
			if (bomb_grid[x + 1][y] == has_bomb) bombs++;
			if (bomb_grid[x][y + 1] == has_bomb) bombs++;
			if (bomb_grid[x - 1][y] == has_bomb) bombs++;
			if (bomb_grid[x][y - 1] == has_bomb) bombs++;
			if (bomb_grid[x + 1][y + 1] == has_bomb) bombs++;
			if (bomb_grid[x - 1][y - 1] == has_bomb) bombs++;
			if (bomb_grid[x - 1][y + 1] == has_bomb) bombs++;
			if (bomb_grid[x + 1][y - 1] == has_bomb) bombs++;
			num_grid[x][y] = bombs;
		}
	}
}

void render_grid() {
	for (int x = 1; x <= BoardWidth; x++) {
		for (int y = 1; y <= BoardHeight; y++) {
			if (tile_grid[x][y] == white_tile) {
				window.render((64 * (x - 1)), (64 * (y - 1)), texture_white_tile);
				if (flag_grid[x][y] == has_flag) {
					window.render((64 * (x - 1)), (64 * (y - 1)), texture_flag_tile);
				}
			}
			else if (tile_grid[x][y] == black_tile) {
				window.render((64 * (x - 1)), (64 * (y - 1)), texture_black_tile);
				window.render((64 * (x - 1)), (64 * (y - 1)), texture_number_tiles[num_grid[y][x]]);
			}
			else if (tile_grid[x][y] == red_tile) {
				window.render((64 * (x - 1)), (64 * (y - 1)), texture_red_tile);
				if (bomb_grid[y][x] == has_bomb) {
					window.render((64 * (x - 1)), (64 * (y - 1)), texture_bomb_tile);
				}
			}
		}
	}
	display_debug();
}

void generate_bombs() {
	int random_num{};
	srand(time(0));
	for (int x = 1; x <= BoardWidth; x++) {
		for (int y = 1; y <= BoardHeight; y++) {
			if (rand() % 5 == 0) {
				bomb_grid[x][y] = has_bomb;
			}
			else {
				bomb_grid[x][y] = no_bomb;
			}
		}
	}
}

void display_debug() {
	if (developer == true) {
		cout << endl;
		cout << "---------------" << endl;
		cout << endl;
		cout << "Flag Grid: " << endl;
		for (int x = 1; x <= BoardWidth; x++) {
			for (int y = 1; y <= BoardHeight; y++) {
				cout << flag_grid[x][y] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "Number Grid: " << endl;
		for (int x = 1; x <= BoardWidth; x++) {
			for (int y = 1; y <= BoardHeight; y++) {
				cout << num_grid[x][y] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "Bomb Grid:" << endl;
		for (int x = 1; x <= BoardWidth; x++) {
			for (int y = 1; y <= BoardHeight; y++) {
				cout << bomb_grid[x][y] << " ";
			}
			cout << endl;
		}
		cout << endl;
		cout << "Tile Grid:" << endl;
		for (int x = 1; x <= BoardWidth; x++) {
			for (int y = 1; y <= BoardHeight; y++) {
				cout << tile_grid[y][x] << " ";
			}
			cout << endl;
		}
	}
}

void post_game(int state) {
	for (int x = 1; x <= BoardWidth; x++) {
		for (int y = 1; y <= BoardHeight; y++) {
			if (state == you_lost) {
				if (bomb_grid[x][y] == has_bomb) {
					tile_grid[y][x] = red_tile;
				}
			}
			else if (state == you_won) {
				flag_grid[x][y] = no_flag;
				tile_grid[x][y] = black_tile;
			}
		}
	}
	render_grid();
	window.display();

	bool on_menu = true;
	while (on_menu) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				on_menu = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					on_menu = false;
				}
			}
		}
	}
}