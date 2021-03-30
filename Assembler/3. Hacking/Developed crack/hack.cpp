#include "hack.h"

const char* hello_message      = "Danya_hack program cracking will be started in 3 seconds...\n";
const char* suggestion_message = "You are to play game with Dim Dimich!\n";
const char* end_message        = "100%\n"
                                 "Congratulations! Program is cracked!\n\n"  \
                                 "You can donate me to support poor student!\n";

void edit_console() 
{
    // Console colour
    system("color 61");

    // Ñonsole window options
    HANDLE out_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_FONT_INFOEX font_info;
    font_info.cbSize = sizeof(font_info);
    GetCurrentConsoleFontEx(out_handle, TRUE, &font_info);

    wcscpy(font_info.FaceName, L"Lucida Console");
    font_info.dwFontSize.X = 10;
    font_info.dwFontSize.Y = 20;

    COORD crd = { 70, 15 };
    SMALL_RECT src = { 0, 0, crd.X - 1, crd.Y - 1 };

    // Set all parameters
    SetConsoleWindowInfo(out_handle, true, &src);
    SetConsoleScreenBufferSize(out_handle, crd);
    SetCurrentConsoleFontEx(out_handle, TRUE, &font_info);
}

char* read_file (const char* file_name, size_t* program_size)
{
    assert(file_name);

    FILE* program_file = fopen(file_name, "rb");
    assert(program_file);

    fseek(program_file, 0, SEEK_END);
    size_t file_size = ftell(program_file);
    fseek(program_file, 0, SEEK_SET);

    char* program = (char*) calloc(file_size, sizeof(char));
    assert(program);

    fread(program, 1, file_size, program_file);
    fclose(program_file);

    *program_size = file_size;
    return program;
}

unsigned long long hash_calc (void* buffer, size_t buffer_size)
{
    assert(buffer);

    char* norm_buffer = (char*) buffer;
    unsigned long long hash = 0;

    for (int i = 0; i < buffer_size; ++i)
        hash = hash * 133419 + (unsigned char) norm_buffer[i] + 931384623;

    return hash;
}

void write_file (const char* file_name, size_t program_size, char* program)
{
    assert(file_name);
    assert(program);

    FILE* program_file = fopen(file_name, "wb");
    assert(program_file);

    fwrite(program, program_size, sizeof(char), program_file);
    fclose(program_file);
}

void delay (int number_of_miliseconds)
{
    int msec = number_of_miliseconds;
    clock_t start_time = clock();

    while (clock() < start_time + msec);
}

void print_status_bar()
{
    int stars_number = PROGRESS_NUMBER;

    print_message("Program is cracking:\n");

    for (int i = 0; i < stars_number + 1; ++i) 
    {
        printf("*");

        delay(300);
    }

    printf("\n");
}

void print_message (const char* message)
{
    size_t message_length = strlen(message);

    for (int i = 0; i < message_length; ++i)
    {
        putc(message[i], stdout);
        delay(100);
    }
}

void clear_field (int* field) 
{
    for (int i = 0; i < FIELD_SZ; ++i)
        field[i] = EMPTY;
}

int* set_zero (int* field)
{
    int x = 1 + rand() % LINE_SZ;
    int y = 1 + rand() % LINE_SZ;

    while (game_over (field).first == false) 
    {
        if (field[(x - 1) + (y - 1) * LINE_SZ] == EMPTY) 
        {
            field[(x - 1) + (y - 1) * LINE_SZ] = ZERO;

            return field;
        }
        else 
        {
            x = 1 + rand() % LINE_SZ;
            y = 1 + rand() % LINE_SZ;
        }
    }

    return field;
}

int* set_zero_AI (int* field)
{
    int index = 0;

    minimax(field, ZERO, true, &index);

    int new_fields_size = free_cells(field);

    if (new_fields_size == 0)
        return field;

    int** fields = new_fields(field, ZERO);

    return fields[index];
}

int free_cells (int* field) 
{
    int ans = 0;
    for (int i = 0; i < FIELD_SZ; ++i)
    {
        if (field[i] == EMPTY)
            ++ans;
    }

    return ans;
}

void copy_field (int* source, int* destination) 
{
    for (int i = 0; i < FIELD_SZ; ++i)
        destination[i] = source[i];
}

int** new_fields (int* field, int player) 
{
    int** ans = (int**) calloc(free_cells(field), sizeof(int*));
    int ans_counter = 0;

    int new_field[FIELD_SZ];
    clear_field(new_field);

    for (int i = 0; i < FIELD_SZ; ++i)
        new_field[i] = field[i];

    for (int i = 0; i < FIELD_SZ; ++i) 
    {
        if (new_field[i] == EMPTY) 
        {
            new_field[i] = player;
            ans[ans_counter] = (int*) calloc(FIELD_SZ, sizeof(int));
            copy_field(new_field, ans[ans_counter++]);
            new_field[i] = EMPTY;
        }
    }

    return ans;
}

void free_fields(int** fields, int fields_size) 
{
    for (int i = 0; i < fields_size; ++i)
        free(fields[i]);

    free(fields);
}

pair game_over (int* field) 
{
    bool result = false;

    int gameovers_cross[GAMEOVERS_SZ][FIELD_SZ] = 
    {
        {1, -1, -1, 1, -1, -1, 1, -1, -1},
        {-1, 1, -1, -1, 1, -1, -1, 1, -1},
        {-1, -1, 1, -1, -1, 1, -1, -1, 1},
        {1, 1, 1, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, 1, 1, 1, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, 1, 1, 1},
        {1, -1, -1, -1, 1, -1, -1, -1, 1},
        {-1, -1, 1, -1, 1, -1, 1, -1, -1} 
    };

    int gameovers_zero[GAMEOVERS_SZ][FIELD_SZ] = 
    {
        {0, -1, -1, 0, -1, -1, 0, -1, -1},
        {-1, 0, -1, -1, 0, -1, -1, 0, -1},
        {-1, -1, 0, -1, -1, 0, -1, -1, 0},
        {0, 0, 0, -1, -1, -1, -1, -1, -1},
        {-1, -1, -1, 0, 0, 0, -1, -1, -1},
        {-1, -1, -1, -1, -1, -1, 0, 0, 0},
        {0, -1, -1, -1, 0, -1, -1, -1, 0},
        {-1, -1, 0, -1, 0, -1, 0, -1, -1} 
    };

    for (int i = 0; i < GAMEOVERS_SZ; ++i) 
    {
        bool flag = true;

        for (int j = 0; j < FIELD_SZ; ++j) 
        {
            if (gameovers_cross[i][j] == CROSS && field[j] != CROSS)
                flag = false;
        }

        if (flag == true)
            return pair(true, WIN);

        for (int j = 0; j < FIELD_SZ; ++j)
        {
            if (gameovers_zero[i][j] == ZERO && field[j] != ZERO)
                flag = true;
        }

        if (flag == false)
            return pair(true, LOSE);
    }

    for (int i = 0; i < FIELD_SZ; ++i)
    {
        if (field[i] == EMPTY)
            return pair(false, DRAW);
    }

    return pair(true, DRAW);
}

int minimax (int field[FIELD_SZ], int player, bool ret_index, int* index) 
{
    pair temp = game_over(field);

    if (temp.first == true) 
    {
        index = 0;

        return temp.second;
    }

    if (player == CROSS) 
    {
        int next = -INT_MAXIMUM;
        int new_fields_size = free_cells(field);
        int** fields = new_fields(field, CROSS);

        for (int i = 0; i < new_fields_size; ++i) 
        {
            int temp = minimax(fields[i], ZERO, false, index);
            next = (temp > next ? (ret_index ? *index = i : *index = *index), temp : next);
        }

        free_fields(fields, new_fields_size);

        return next;
    }
    else 
    {
        int next = INT_MAX;
        int new_fields_size = free_cells(field);
        int** fields = new_fields(field, ZERO);

        for (int i = 0; i < new_fields_size; ++i) 
        {
            int temp = minimax(fields[i], CROSS, false, index);
            next = (temp < next ? (ret_index ? *index = i : *index = *index), temp : next);
        }

        free_fields(fields, new_fields_size);

        return next;
    }
}

void draw_table_SFML (sf::RenderWindow* window, int* field, sf::Texture* cross_pic, sf::Texture* zero_pic) 
{
    sf::Sprite objects[FIELD_SZ];

    for (int i = 0; i < FIELD_SZ; ++i) 
    {
        if (field[i] != EMPTY) 
        {
            if (field[i] == CROSS)
                objects[i] = sf::Sprite(*cross_pic);
            else
                objects[i] = sf::Sprite(*zero_pic);

            sf::Vector2i pos(((i % LINE_SZ) * 100 + 10), ((i / LINE_SZ) * 100 + 10));

            objects[i].setPosition((float) pos.x, (float) pos.y);
            objects[i].setOrigin(0, 0);
            objects[i].setScale(1, 1);
            window->draw(objects[i]);
        }
    }

    window->display();
}