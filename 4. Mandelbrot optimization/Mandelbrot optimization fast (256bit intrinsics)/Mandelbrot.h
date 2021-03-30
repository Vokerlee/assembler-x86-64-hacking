#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <windows.h>

#include <smmintrin.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <immintrin.h>

const int X_SIZE = 1200;
const int Y_SIZE = 800;

const float RADIUS = 1e5;
const float SQ_RADIUS = RADIUS * RADIUS;
const int ITERATION_LIMIT = 255;
const double INITIAL_DIVISION = 240.0;

enum state
{
	IN_CIRCLE = 1,
	OUT_CIRCLE = 0
};

union colour 
{
	RGBQUAD colour;
	int number;
};

void fill_data(sf::Uint32* data, int x_size, int y_size, double division, int shift_hor, int shift_ver);

void print_figure(int x_size, int y_size);