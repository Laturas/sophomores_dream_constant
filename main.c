#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "C:\MinGW\include\gmp.h"
#include <string.h>

#define TET(x) mpz_pow_ui(x,x,x)
#define NEW(i) mpz_t i; mpz_init(i);
#define MAX_SIZE 205000

mpz_t ten;

// It's actually numerator, denominator but I forgor to change it ;-;
void long_divide(FILE* out, mpz_t denominator, mpz_t numerator) {
    int i = 0;

    NEW(multiple_1); NEW(multiple_4); NEW(multiple_7);
    NEW(multiple_2); NEW(multiple_5); NEW(multiple_8);
    NEW(multiple_3); NEW(multiple_6); NEW(multiple_9);

    mpz_init_set(multiple_1, numerator);
    mpz_add(multiple_2,multiple_1,numerator); mpz_add(multiple_3,multiple_2,numerator);
    mpz_add(multiple_4,multiple_3,numerator); mpz_add(multiple_5,multiple_4,numerator);
    mpz_add(multiple_6,multiple_5,numerator); mpz_add(multiple_7,multiple_6,numerator);
    mpz_add(multiple_8,multiple_7,numerator); mpz_add(multiple_9,multiple_8,numerator);

    while (mpz_cmp(numerator, denominator) > 0) {
        mpz_mul(denominator,denominator,ten); i++;
        if (i >= MAX_SIZE) {goto KILL;}
    }
    while (i < MAX_SIZE) {
        int j = 1;
        int skip_first_zero = 1;

        while (mpz_cmp(numerator, denominator) > 0) {
            mpz_mul(denominator,denominator,ten); 
            if (skip_first_zero) {skip_first_zero = !skip_first_zero; continue;}
            fprintf(out, "0"); i++;
            if (i >= MAX_SIZE) {goto KILL;}
        }
        if (mpz_cmp(multiple_2, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_1); goto OUT;} j++;
        if (mpz_cmp(multiple_3, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_2); goto OUT;} j++;
        if (mpz_cmp(multiple_4, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_3); goto OUT;} j++;
        if (mpz_cmp(multiple_5, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_4); goto OUT;} j++;
        if (mpz_cmp(multiple_6, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_5); goto OUT;} j++;
        if (mpz_cmp(multiple_7, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_6); goto OUT;} j++;
        if (mpz_cmp(multiple_8, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_7); goto OUT;} j++;
        if (mpz_cmp(multiple_9, denominator) >= 0) {mpz_sub(denominator,denominator,multiple_8); goto OUT;} j++;
        mpz_sub(denominator,denominator,multiple_9);

        OUT:
        fprintf(out, "%d",j); i++;
    }
    KILL:
    fprintf(out,"\n");

    mpz_clear(multiple_1); mpz_clear(multiple_4); mpz_clear(multiple_7);
    mpz_clear(multiple_2); mpz_clear(multiple_5); mpz_clear(multiple_8);
    mpz_clear(multiple_3); mpz_clear(multiple_6); mpz_clear(multiple_9);
}

#define RIGHTSHIFT(x,amt) __gmpz_tdiv_q_2exp(x,x,amt);

/// @brief Calculates a partial sum
void calculate(FILE* out, unsigned int start, FILE* opt_denoms) {
    mpz_t minus_0, minus_1, minus_2, minus_3, minus_4, minus_5, minus_6, minus_7;

    mpz_init_set_ui(minus_0, start);   mpz_init_set_ui(minus_4, start-4);
    mpz_init_set_ui(minus_1, start-1); mpz_init_set_ui(minus_5, start-5);
    mpz_init_set_ui(minus_2, start-2); mpz_init_set_ui(minus_6, start-6);
    mpz_init_set_ui(minus_3, start-3); mpz_init_set_ui(minus_7, start-7);

    mpz_pow_ui(minus_0, minus_0, start);   mpz_pow_ui(minus_4, minus_4, start-4);
    mpz_pow_ui(minus_1, minus_1, start-1); mpz_pow_ui(minus_5, minus_5, start-5);
    mpz_pow_ui(minus_2, minus_2, start-2); mpz_pow_ui(minus_6, minus_6, start-6);
    mpz_pow_ui(minus_3, minus_3, start-3); mpz_pow_ui(minus_7, minus_7, start-7);

    NEW(sum_1); mpz_add(sum_1,minus_0,minus_1);
    NEW(sum_2); mpz_add(sum_2,minus_2,minus_3);
    NEW(sum_3); mpz_add(sum_3,minus_4,minus_5);
    NEW(sum_4); mpz_add(sum_4,minus_6,minus_7);

    NEW(fus_1); mpz_mul(fus_1,minus_0,minus_1);
    NEW(fus_2); mpz_mul(fus_2,minus_2,minus_3);
    NEW(fus_3); mpz_mul(fus_3,minus_4,minus_5);
    NEW(fus_4); mpz_mul(fus_4,minus_6,minus_7);

    mpz_clear(minus_0); mpz_clear(minus_1);
    mpz_clear(minus_2); mpz_clear(minus_3);
    mpz_clear(minus_4); mpz_clear(minus_5);
    mpz_clear(minus_6); mpz_clear(minus_7);

    mpz_mul(sum_1,sum_1,fus_2); mpz_mul(sum_2,sum_2,fus_1);
    mpz_mul(sum_3,sum_3,fus_4); mpz_mul(sum_4,sum_4,fus_3);

    mpz_mul(fus_2,fus_1,fus_2);
    mpz_mul(fus_4,fus_3,fus_4);

    mpz_add(sum_4,sum_3,sum_4);
    mpz_add(sum_2,sum_1,sum_2);

    mpz_mul(sum_4,fus_2,sum_4);
    mpz_mul(sum_2,fus_4,sum_2);

    NEW(numerator); mpz_add(numerator,sum_4,sum_2);
    RIGHTSHIFT(numerator, (start << 1) - 10); // TODO: Add rightshifting for denominators too
    RIGHTSHIFT(fus_4,     (start << 1) - 10);
    NEW(denominator); mpz_mul(denominator,fus_2,fus_4);

    mpz_clear(fus_1); mpz_clear(sum_1);
    mpz_clear(fus_2); mpz_clear(sum_2);
    mpz_clear(fus_3); mpz_clear(sum_3);
    mpz_clear(fus_4); mpz_clear(sum_4);

    long_divide(out,numerator,denominator);
    
    mpz_clear(numerator); mpz_clear(denominator);
}

#define ITERS 8000 //196 // 392 for equivalent with blocks of size 4

int main(int argc, char* argv[]) {
    if (argc != 2) {printf("-r or -w?\n"); return 1;}
    FILE* out;
    clock_t start = clock();
    if (argv[1][1] == 'w') {
        out = fopen("./output.txt", "w");
        mpz_init_set_str(ten, "10", 10);
        printf("Calculating partial sums for %d terms\n", ITERS*4+1);
        fflush(stdout);

        for (int i = 8; i < ITERS*8+1; i += 8) {calculate(out, i, NULL);}
    }
    else {
        printf("Summing partial sums\n");
        fflush(stdout);

        mpz_t dec; mpz_init_set_str(dec, "0", 10);
        out = fopen("./output.txt", "r");
        
        for (int i = 0; i < ITERS; i++) {
            char buff[MAX_SIZE + 5] = {0};
            fgets(buff,MAX_SIZE + 5,out);
            buff[MAX_SIZE] = '\0';

            mpz_t part;
            mpz_init_set_str(part, buff, 10);
            mpz_add(dec,dec,part);
            mpz_clear(part);

            if (i + 1 == ITERS) {printf("-------------------\nCorrect digits (lower bound): %d\n-------------------\n", MAX_SIZE - strlen(buff));}
        }
        FILE* finalout = fopen("./final_output.txt","w");
        gmp_fprintf(finalout, "%Zd\n",dec);
        mpz_clear(dec);
        fclose(finalout);
    }
    clock_t end = clock();
    double elapsed_time = (end-start)/(double)CLOCKS_PER_SEC;

    printf("Process completed in: %lf seconds\n", elapsed_time);

    fclose(out);
    mpz_clear(ten); 

    return 0;
}