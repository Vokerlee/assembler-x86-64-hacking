#pragma once
#pragma comment(lib, "Winmm.lib")

#define NOMINMAX
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <assert.h>

#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

// Useful pair and enum for checking the result of game every step
enum result
{
    LOSE = -1,
    DRAW = 0,
    WIN = 1
};

typedef std::pair <bool, result> pair;

// Sizes
const int WINDOW_SZ    = 300;  // 300x300  pixels
const int FIELD_SZ     = 9;    // 3x3 field size
const int LINE_SZ      = 3;    // the size of line
const int GAMEOVERS_SZ = 8;    // 8 possible gameover combinations

// Designations of cells in the field
const int EMPTY        = -1;
const int ZERO		   = 0;
const int CROSS        = 1;

const int INT_MAXIMUM  = 1000;

const unsigned long long normal_hash = 11793140125191960234;
const unsigned long long cracked_hash = 12068795574347971473;
const int PROGRESS_NUMBER = 20;

void edit_console();

char* read_file(const char* file_name, size_t* program_size);

void write_file(const char* file_name, size_t program_size, char* program);

unsigned long long hash_calc(void* buffer, size_t buffer_size);

void delay(int number_of_seconds);

void print_status_bar();

void print_message(const char* message);

void clear_field (int* field);

int* set_zero (int* field);

int* set_zero_AI (int* field);

int free_cells (int* field);

void copy_field (int* source, int* destination);

int** new_fields (int* field, int player);

void free_fields (int** fields, int fields_size);

pair game_over (int* field);

int minimax (int field[FIELD_SZ], int player, bool ret_index, int* index);

void draw_table_SFML (sf::RenderWindow* window, int* field, \
    sf::Texture* cross_pic, sf::Texture* zero_pic);

