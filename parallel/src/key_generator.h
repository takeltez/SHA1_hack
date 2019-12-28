#pragma once
#include <string.h>
#include "openssl/sha.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define MAX_KEY_SIZE 50
#define SYMBOL_LENGTH 4
#define ALPHABET_SIZE 62.

int number[MAX_KEY_SIZE];

typedef unsigned long long ull_t;
static int count_perm = 0;

void alphabet_parser(char *symbols, char *alphabet);
void setIndex(int *index, char *alphabet, int alphabet_length);
long getSizePerm(int alphabet_length, int key_length);
size_t comb(char *str, ull_t combination, int key_length, int alphabet_length, char *alphabet);
ull_t get_block_size(ull_t n, int rank, int nprocs);
ull_t get_start_block(ull_t n, ull_t rank, int nprocs);
void print_hash(unsigned char *hash);
int compare_hash(unsigned char *cur_hash, unsigned char *src_hash) ;
int brute_force(int k, int key_length, int alphabet_length, char *alphabet, unsigned char *src_hash, char *buffer_current_line, int *index, ull_t comb_per_proc);
