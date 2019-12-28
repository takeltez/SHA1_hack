#pragma once
#include <string.h>
#include "openssl/sha.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#define MAX_KEY_SIZE 50
#define SYMBOL_LENGTH 4
#define ALPHABET_SIZE 62

int number[MAX_KEY_SIZE];

void alphabet_parser(char *symbols, char *alphabet);
void print_hash(unsigned char *hash);
int compare_hash(unsigned char *cur_hash, unsigned char *src_hash) ;
int brute_force(int k, int key_length, int alphabet_length, char *alphabet, unsigned char *src_hash);
