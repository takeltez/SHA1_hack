#include "key_generator.h"
#include "key_generator.c"

int main(int argc, char **args)
{
    int rank, commsize;
    MPI_Init(&argc, &args);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);

    int index[256];
    int alphabet_length, key_length;
    char *alphabet; 
    char *src_key; 
    char *symbols;
    unsigned char src_hash[SHA_DIGEST_LENGTH];
    char *buffer_current_line;
    double t_total;

    symbols = args[1];
    src_key = args[2];

    alphabet = (char*)malloc(ALPHABET_SIZE * sizeof(char));
    alphabet_parser(symbols, alphabet);

    alphabet_length = (int) strlen(alphabet); 
    key_length = atoi(args[3]); 


    if (key_length != strlen(src_key)) {
        if (rank == 0)
            printf("Incorrect length of key\n");
        exit(0);
    }

    if (alphabet_length == 0) {
        if (rank == 0)
            printf("Alphabet is empty\n");
        exit(0);
    }

    if (rank == 0) {
        printf("Alphabet: %s\n", alphabet);
        printf("Alphabet length: %d, key length: %d: count of variants: %d\n\n", alphabet_length, key_length, (int)pow(alphabet_length, key_length));
    }

    buffer_current_line = malloc(sizeof(char) * key_length + 1);

    SHA1(src_key, strlen(src_key), src_hash); 
    qsort(alphabet, (size_t) alphabet_length, sizeof(char), (__compar_fn_t) strcmp);

    double t = MPI_Wtime();

    long sizePerm = getSizePerm(alphabet_length, key_length);
	ull_t comb_per_proc = get_block_size((ull_t) sizePerm, rank, commsize);
    ull_t start_comb = get_start_block((ull_t) sizePerm, (ull_t) rank, commsize);

    size_t n = comb(buffer_current_line, start_comb, key_length, alphabet_length, alphabet);
    int maximum_length = key_length;

    key_length = (int) n;

    setIndex(index, alphabet, alphabet_length);
    do {
        brute_force(0, key_length, alphabet_length, alphabet, src_hash, buffer_current_line, index, comb_per_proc);
        key_length++;
        memset(buffer_current_line, alphabet[0], (size_t) alphabet_length);
        buffer_current_line[key_length] = '\0';
    }
    while ((key_length <= maximum_length) && (count_perm < comb_per_proc));

    t = MPI_Wtime() - t;
    MPI_Reduce(&t, &t_total, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nCount of procceses: %d\n", commsize);
        printf("Elapsed time: %.6f\n", t_total);
    }

    free(buffer_current_line);
    MPI_Finalize();
    return 0;
}
