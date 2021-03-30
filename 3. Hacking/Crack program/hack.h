#pragma once
#pragma comment(lib, "Winmm.lib")

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

const unsigned long long normal_hash = 11793140125191960234;
const unsigned long long cracked_hash = 12068795574347971473;
const int PROGRESS_NUMBER = 20;

char* read_file (const char* file_name, size_t* program_size);

void write_file (const char* file_name, size_t program_size, char* program);

unsigned long long hash_calc (void* buffer, size_t buffer_size);

void delay (int number_of_seconds);

void print_status_bar();

void print_message (const char* message);

