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

void setIndex(int *index, char *alphabet, int alphabet_length) {

    for (int i = 0; i < alphabet_length; i++) {
        index[alphabet[i]] = i;
    }
}

void print_hash(unsigned char *hash)
{
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        printf("%02x", hash[i]);
}

// Сравниваем хеши. Нашли или нет
int compare_hash(unsigned char *cur_hash, unsigned char *src_hash)
{
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
        if (cur_hash[i] != src_hash[i])
            return -1;
    return 0;
}

int brute_force(int k, int key_length, int alphabet_length, char *alphabet, unsigned char *src_hash, char *buffer_current_line, int *index, ull_t comb_per_proc)
{
    if (k == key_length) {
        unsigned char current_hash[SHA_DIGEST_LENGTH];
        unsigned char current_key[key_length + 1];
        //Заполнение текущей строки
        for (int i = 0; i < key_length; i++) {
            current_key[i] = alphabet[number[i]];
        }
        current_key[key_length] = '\0';

        SHA1(current_key, strlen(current_key), current_hash);
        ++count_perm;
       
        if (compare_hash(current_hash, src_hash) == 0) {
            int rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            printf("Procces %d found collision!\n", rank);
            printf("source hash(SHA1): ");
            print_hash(src_hash);
            printf("\ncurrent hash(SHA1): ");
            print_hash(current_hash);
            printf("\npasword: %s\n", current_key);
            return 1;
        }
    } else {
        for (int j = index[buffer_current_line[k]]; j < alphabet_length && count_perm < comb_per_proc; j++) {
            number[k] = j;
           
            brute_force(k + 1, key_length, alphabet_length, alphabet, src_hash, buffer_current_line, index, comb_per_proc);
        }
        buffer_current_line[k] = alphabet[0];
    }
    return 0;
}

long getSizePerm(int alphabet_length, int key_length) {
    long count_permutations = 0;

    for (int i = 1; i <= key_length; i++)
        count_permutations += pow(alphabet_length, i);

    return count_permutations;
}

size_t comb(char *str, ull_t combination, int key_length, int alphabet_length, char *alphabet) 
{
    ull_t sum = 0;
    ull_t cur_len = 0;
    for (ull_t i = 1; i <= key_length; ++i) {
        ull_t tmp = (ull_t) pow(alphabet_length, i);
        if (sum + tmp >= combination) {
            cur_len = i;
            break;
        }
        sum += tmp;
    }
    combination -= (sum);
    ull_t ind;
    ull_t p = (ull_t) pow(alphabet_length, cur_len - 1);
    for (ull_t i = 0; i < cur_len; ++i) {
        ind = (ull_t) floor(combination / p);
        combination -= (ind * p);
        str[i] = alphabet[ind];
        p /= alphabet_length;
    }
    str[cur_len] = '\0';

    return cur_len;
}

ull_t get_block_size(ull_t n, int rank, int nprocs) 
{
    ull_t s = n / nprocs;
    if (n % nprocs > rank)
        s++;
    return s;
}

ull_t get_start_block(ull_t n, ull_t rank, int nprocs) 
{
    ull_t rem = n % nprocs;
    return n / nprocs * rank + ((rank >= rem) ? rem : rank);
}

