#include "key_generator.h"

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}

int main(int argc, char **args)
{
    int alphabet_length, key_length;
    char *alphabet; 
    char *src_key; 
    char *symbols;
    unsigned char src_hash[SHA_DIGEST_LENGTH];

    symbols = args[1];
    src_key = args[2];

    alphabet = (char*)malloc(ALPHABET_SIZE * sizeof(char));
    alphabet_parser(symbols, alphabet);

    alphabet_length = (int) strlen(alphabet); 
    key_length = atoi(args[3]); 

     if (key_length != strlen(src_key)) {
        printf("Incorrect length of key\n");
        exit(0);
    }

    if (alphabet_length == 0) {
        printf("Alphabet is empty\n");
        exit(0);
    }

    printf("Alphabet: %s\n", alphabet);
    printf("Alphabet length: %d, key length: %d: count of variants: %d\n\n", alphabet_length, key_length, (int)pow(alphabet_length, key_length));

    SHA1(src_key, strlen(src_key), src_hash); 
   
    double t = wtime();
    int h = key_length; 
    key_length = 1; 
     do {
        brute_force(0, key_length, alphabet_length, alphabet, src_hash);
    } while (key_length++ < h);

    t = wtime() - t;
    printf("\nElapsed time: %.6f\n", t);

    free(alphabet);

    return 0;
 }