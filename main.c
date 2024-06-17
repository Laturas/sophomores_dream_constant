#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "C:\MinGW\include\gmp.h"
#include <string.h>

#define TET(x) mpz_pow_ui(x,x,x)
#define NEW(i) mpz_t i; mpz_init(i);
#define MAX_SIZE 1300000
//#define MAX_SIZE 50000

mpz_t tenpow;

int memory_killing_long_divide(FILE* out, mpz_t denominator, mpz_t numerator, mpz_t storage) {
    mpz_mul(denominator,denominator,tenpow);
    mpz_tdiv_q(denominator,denominator,numerator);
    mpz_add(storage,storage,denominator);
    if (!mpz_cmp_ui(denominator,0)) {return 1;}
    //gmp_fprintf(out, "%Zd\n",denominator);
    return 0;
}

#define RIGHTSHIFT(x,amt) __gmpz_tdiv_q_2exp(x,x,amt);
#define fus_1 minus_0
#define fus_2 minus_1
#define fus_3 minus_2
#define fus_4 minus_3
#define numerator sum_2
#define denominator sum_1

/// @brief Calculates a partial sum
int calculate(FILE* out, unsigned int start, mpz_t storage) {
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

    mpz_mul(fus_1,minus_0,minus_1);
    mpz_mul(fus_2,minus_2,minus_3);
    mpz_mul(fus_3,minus_4,minus_5);
    mpz_mul(fus_4,minus_6,minus_7);

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

    mpz_add(numerator,sum_4,sum_2);
    RIGHTSHIFT(numerator, (start << 1) - 10);
    RIGHTSHIFT(fus_4,     (start << 1) - 10);
    mpz_mul(denominator,fus_2,fus_4);

    mpz_clear(fus_1); mpz_clear(fus_2);
    mpz_clear(fus_3); mpz_clear(fus_4); 
    mpz_clear(sum_3); mpz_clear(sum_4);

    int return_code = memory_killing_long_divide(out,numerator,denominator,storage);
    
    mpz_clear(numerator); mpz_clear(denominator);
    if (return_code) {return 1;}
    return 0;
}

#define ITERS 75000 // Vestigial structure

int main(int argc, char* argv[]) {
    if (argc != 2) {printf("-r or -w?\n"); return 1;}
    FILE* out;
    clock_t start = clock();
    if (argv[1][1] == 'w') {
        out = fopen("./output.txt", "w");
        mpz_init_set_ui(tenpow, 10);
        mpz_pow_ui(tenpow, tenpow, MAX_SIZE - 1);
        printf("Calculating value utilizing %d digits of precision\n", MAX_SIZE);
        fflush(stdout);

        for (int i = 8; i < ITERS*8+1; i += 64) { 
            // Silly manual loop unrolling >w<
            // This does actually help as it turns out. It reduces disk usage by a factor of 8, speeds up the summing process, and slightly reduces execution time. (about 3 seconds less for 50k digits) 
            NEW(storage);
            mpz_init_set_ui(storage, 0);
            if (calculate(out, i, storage)) {printf("Zero found at iteration %d, exiting early\n",i); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 8, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 8); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 16, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 16); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 24, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 24); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 32, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 32); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 40, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 40); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 48, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 48); gmp_fprintf(out, "%Zd", storage); break;}
            if (calculate(out, i + 56, storage)) {printf("Zero found at iteration %d, exiting early\n",i + 56); gmp_fprintf(out, "%Zd", storage); break;}
            gmp_fprintf(out, "%Zd\n", storage);
            mpz_clear(storage);
        }
    }
    else {
        printf("Summing partial sums\n");
        fflush(stdout);

        mpz_t dec; mpz_init_set_str(dec, "0", 10);
        out = fopen("./output.txt", "r");
        
        int curlen = 0;
        while (1) {
            char buff[MAX_SIZE + 5] = {0};
            fgets(buff,MAX_SIZE + 5,out);
            buff[MAX_SIZE] = '\0';
            int newcurlen = strlen(buff);
            if (newcurlen == 0) {printf("-------------------\nCorrect digits (lower bound): %d\n-------------------\n", MAX_SIZE - curlen); break;}
            curlen = newcurlen;

            mpz_t part;
            mpz_init_set_str(part, buff, 10);
            mpz_add(dec,dec,part);
            mpz_clear(part);
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
    mpz_clear(tenpow); 

    return 0;
}