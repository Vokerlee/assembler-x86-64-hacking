#pragma once

#include <iostream>
#include <algorithm>
#include <random>
#include <fstream>
#include <cstdlib>
#include <cstdio>

#include <immintrin.h>
#include <xmmintrin.h>
#include <nmmintrin.h>

const int TABLE_SIZE = 1009;
const int N_WORDS    = 24000;
const int N_SAMPLES  = 8000000;
const int WORD_SIZE  = 48;

const char* WORD_SET = "word_set.txt";
const char* LOG_PATH = "test.txt";