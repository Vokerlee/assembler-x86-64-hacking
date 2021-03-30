#include "hack.h"

extern const char* hello_message;
extern const char* end_message;
extern const char* suggestion_message;

int main() 
{
    // Edit console appereance
    edit_console();

    // Cool music
    PlaySound(TEXT("telephone.wav"), NULL, SND_ASYNC | SND_LOOP | SND_FILENAME);

    print_message(hello_message);
    delay(3000);
    print_message(suggestion_message);

    sf::RenderWindow window(sf::VideoMode(WINDOW_SZ, WINDOW_SZ), "Deep hacking");

    // Object for pointer-moving observing
    sf::Vector2i mouse_pos = {};
    sf::Vector2i mouse_pos_old = sf::Mouse::getPosition();

    // Background object
    sf::Texture bg = {};
    bg.loadFromFile("data/table.jpg");

    // Placing the background on the table
    sf::Sprite table = {};
    table.setTexture(bg);
    table.setOrigin(20, 20);
    table.setScale(0.93, 0.93);

    // Cross object
    sf::Texture cross_pic = {};
    cross_pic.loadFromFile("data/cross.png");

    // Zero object
    sf::Texture zero_pic = {};
    zero_pic.loadFromFile("data/crinj.jpg");

    // Initialization the memory for field
    int* field = (int*) calloc(FIELD_SZ, sizeof(int));
    clear_field(field);

    // Drawing the table
    window.draw(table);
    window.display();

    // Main cycle
    while (window.isOpen())
    {
        // If the button is pressed and no one is already winner
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !(game_over(field).first)) 
        {
            mouse_pos = sf::Mouse::getPosition(window);

            if (mouse_pos != mouse_pos_old && (mouse_pos.x < WINDOW_SZ && mouse_pos.x > 0) \
                && (mouse_pos.y < WINDOW_SZ && mouse_pos.y > 0))
            {
                int index = mouse_pos.x / 100 + (mouse_pos.y / 100) * LINE_SZ;  // the number of the cell in field

                //printf ("x %d, y %d, index %d\n", MousePos.x, MousePos.y, index);
                if (field[index] == EMPTY)
                {
                    field[index] = CROSS;
                    window.draw(table);

                    // The step of robot
                    int* new_field = set_zero_AI(field);
                    field = new_field;

                    draw_table_SFML(&window, field, &cross_pic, &zero_pic);
                }
            }

            mouse_pos_old = mouse_pos;
        }

        // Checking the escape button
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();

        if (game_over(field).first) 
        {
            print_message(game_over(field).second == WIN ? "FUC[.....Crack access is open.....]K YOU!\n" : \
                game_over(field).second == DRAW ? "OK, you are worthy.\n"
                                                  "Crack access is open\n\n" : \
                "YOU LOSE! HA! HA! HA!\n");

            window.close();
        }
    }

    if (game_over(field).first && (game_over(field).second == WIN || game_over(field).second == DRAW))
    {
        free(field);

        size_t program_size = 0;
        char* program = read_file("S:\\Doc\\Projects\\Danya_hack\\HACKPASS.COM", &program_size);

        unsigned long long hash = hash_calc(program, program_size);

        if (hash == cracked_hash)
        {
            print_message("Program is already cracked!!\n");
            delay(12000);

            return 0;
        }

        if (hash != normal_hash)
        {
            print_message("Program is modified. Please check version\n");
            delay(12000);

            return 0;
        }

        //                      PROGRAM CRACKING
        //====================================================================
        program[6] = 144;
        program[7] = 144;
        program[8] = 144;
        program[9] = 235;
        program[11] = 144;
        program[12] = 144;

        program[66] = 74;

        program[75] = 195;
        program[76] = 90;
        //====================================================================

        write_file("S:\\Doc\\Projects\\Danya_hack\\HACKPASS.COM", program_size, program);

        print_status_bar();
        print_message(end_message);

        delay(60000);
    }
    else
    {
        print_message("This time you can't crack the program, because you are not worthy!");
        free(field);

        delay(60000);
    }
        
    return 0;
}

