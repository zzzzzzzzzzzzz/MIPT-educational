#include "stdio.h"  
#include "stdlib.h"  
#include "time.h"
  
#define SCALE 1000000000
#define DIGITS_PER_ITERATION 9
#define ARRINIT 200000000 
#define OUTPUT_FORMAT "%.9lu"
  
//based on spigot algo for pi digits  
void pi_digits(int digits_num, FILE * output) {  
    int digits_amount = digits_num / DIGITS_PER_ITERATION + 1;
    const long unsigned int array_length = DIGITS_PER_ITERATION*10*(digits_amount) / 3;
    long unsigned int remainders[array_length];  
    long unsigned int answ[digits_amount];
    int i = 0; 
    for (; i < array_length; i++)
        remainders[i] = ARRINIT;
    for (i = 0; i < digits_amount; i++) {
        int j = array_length-1;
        long unsigned int quotient = 0;
        long unsigned int sum = 0;
        for (; j >= 0; j--) {
            remainders[j] *= SCALE;
            sum = remainders[j]+quotient*(j+1);
            quotient = sum / (2*j+1);
            remainders[j] = sum - quotient*(2*j+1);
        }
        quotient = sum / SCALE;
        remainders[0] = sum - quotient*SCALE;
        answ[i] = quotient;
    } //конец алгоритма
    answ[0] %= SCALE / 10; //начало вывода
    if (digits_num >= DIGITS_PER_ITERATION-1) {
        fprintf(output, "%lu", answ[0]);
        digits_num -= DIGITS_PER_ITERATION-1;
        for (i = 1; i < digits_amount-1; i++) {
            fprintf(output, OUTPUT_FORMAT, answ[i]);
            digits_num -= DIGITS_PER_ITERATION;
        }
        if (digits_num > 0) {
            int final_format = SCALE;
            while (digits_num > 0) {
                final_format /= 10;
                digits_num--;
            }
            fprintf(output, "%lu", answ[i] / final_format);
        } 
    } else {
        answ[0] %= SCALE / 10;
        while (DIGITS_PER_ITERATION - digits_num > 1) {
            answ[0] /= 10;
            digits_num++;
        }
        fprintf(output, "%lu\n", answ[0]);
    }
    fprintf(output, "\n");
}  
  
int main() {  
    int amount = 0;
    scanf("%d", &amount);
    FILE * output = fopen("output.txt", "w");
    clock_t start_time = clock();      
    pi_digits(amount, output);  
    clock_t end_time = clock();
    clock_t result_time = (end_time - start_time);
    fprintf(output ,"\n\nTime %g s", (float)result_time / CLOCKS_PER_SEC);
    fclose(output);
    return 0;  
}