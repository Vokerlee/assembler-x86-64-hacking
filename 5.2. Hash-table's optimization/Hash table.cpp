#define OPTIMIZATION_ON

#include "config.h"
#include "hash_functions.h"
#include "hash_table.h"

typedef CRC32Hash HASH;

int main()
{
    std::ifstream in;
    in.open(WORD_SET);

    auto table = hash_table<char*, HASH>(TABLE_SIZE, HASH());
    char* words = new char[N_WORDS * WORD_SIZE]();

    for (int i = 0; i < N_WORDS * WORD_SIZE; i += WORD_SIZE)
    {
        in >> (words + i);
        table.add(words + i);
    }

    int kek = 0;
    for (int i = 0; i < N_SAMPLES; ++i)
    {
        int word_index = i % N_WORDS;
        kek = kek + i + table.contains(words + word_index * WORD_SIZE);
    }

    printf("Kek = %d", kek);

    std::ofstream out;
    out.open(LOG_PATH);
    table.dump_lists_lens(out);

    in.close();
    out.close();

    delete[] words;

    return 0;
}
