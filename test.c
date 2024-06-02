#include <stdio.h>

#define READTO 10000
int main() {
    printf("Running test on %d digits\n", READTO);
    FILE* my_output = fopen("final_output.txt","r");
    FILE* test_cast = fopen("first10k.txt","r");
    if (!my_output) {printf("Error in opening file: %s\n","final_output.txt");}
    if (!test_cast) {printf("Error in opening file: %s\n","first10k.txt");}
    char mydigits_buff[READTO + 1] = {0};
    char testdigits_buff[READTO + 1] = {0};
    fread(mydigits_buff,sizeof(char),READTO,my_output);
    fread(testdigits_buff,sizeof(char),READTO,test_cast);

    for (int i = 0; i < READTO; i++) {
        if (mydigits_buff[i] != testdigits_buff[i]) {printf("\nDIFFERENCE IN TEST CASES FOUND AT DIGIT: %d\n",i); return 1;}
    }
    printf("Digits up to #%d verified as correct!\n", READTO);
    fclose(my_output);
    fclose(test_cast);
}