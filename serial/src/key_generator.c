#include "key_generator.h"

void alphabet_parser(char *symbols, char *alphabet)
{
    char title_letters[SYMBOL_LENGTH];
    char capital_letters[SYMBOL_LENGTH];
    char numbers [SYMBOL_LENGTH];

    int s_pos, pos;

    for (int i = 0; symbols[i] != ','; ++i) {
        title_letters[i] = symbols[i];
        s_pos = i;
        pos = i;
    }

    title_letters[pos + 1] = '\0';

    for (int i = s_pos + 2, j = 0; symbols[i] != ','; ++i, ++j) {
        capital_letters[j] = symbols[i];
        s_pos = i;
        pos = j;
    }

    capital_letters[pos + 1] = '\0';

    for (int i = s_pos + 2, j = 0; symbols[i] != '.'; ++i, ++j) {
        numbers[j] = symbols[i];
        pos = j;
    }

    numbers[pos + 1] = '\0';
    pos = 0;

    if (strcmp(title_letters, "-") != 0) {

        for (int i = title_letters[0], j = pos; i <= title_letters[strlen(title_letters) - 1]; ++i, ++j) {
            alphabet[j] = i;
            pos = j;
        }
        ++pos;
    }

    if (strcmp(capital_letters, "-") != 0) {

        for (int i = capital_letters[0], j = pos; i <= capital_letters[strlen(capital_letters) - 1]; ++i, ++j) {
            alphabet[j] = i;
            pos = j;
        }
        ++pos;
    }

    if (strcmp(numbers, "-") != 0) {
    	
       for (int i = numbers[0], j = pos; i <= numbers[strlen(numbers) - 1]; ++i, ++j) {
            alphabet[j] = i;
            pos = j;
        }
        ++pos;
    }

    alphabet[pos] = '\0';
}

void print_hash(unsigned char *hash)
{
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        printf("%02x", hash[i]);
}

int compare_hash(unsigned char *cur_hash, unsigned char *src_hash)
{
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        if (cur_hash[i] != src_hash[i])
            return -1;
    return 0;
}

int brute_force(int k, int key_length, int alphabet_length, char *alphabet, unsigned char *src_hash)
{
    if (k == key_length) {
        unsigned char current_hash[SHA_DIGEST_LENGTH];
        unsigned char current_key[key_length];
        for (int i = 0; i < key_length; i++) {
            current_key[i] = alphabet[number[i]];
        }
        current_key[key_length] = '\0';

        SHA1(current_key, strlen(current_key), current_hash);

        if (compare_hash(current_hash, src_hash) == 0) {
            printf("Collision!\n");
            printf("source hash(SHA1): ");
            print_hash(src_hash);
            printf("\ncurrent hash(SHA1): ");
            print_hash(current_hash);
            printf("\npasword: %s\n", current_key);
            return 1;
        }
    } else {
        for (int j = 0; j < alphabet_length; j++) {
            number[k] = j;
            brute_force(k + 1, key_length, alphabet_length, alphabet, src_hash);
        }
    }
    return 0;
}

