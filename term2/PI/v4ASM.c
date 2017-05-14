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
    long unsigned int test = 0;
    asm (
        "init:\n\t"
            "movq $200000000, (%%rax, %%rcx, 8)\n\t"
            "incq %%rcx\n\t"
        "cmpq %%rdx, %%rcx\n\t"
        "jb init\n\t"
        "xorq %%rcx, %%rcx\n\t"
        "outer:\n\t"
            "movq %%rdx, %%r8\n\t"/*j*/
            "decq %%r8\n\t"
            "pushq %%rdx\n\t"
            "movq $0, %%r9\n\t"/*quotient*/
            "movq $0, %%r10\n\t"/*sum*/
            "inner:\n\t"
                "movq (%%rax, %%r8, 8), %%r11\n\t"/*remainders[j]*/
                "pushq %%rax\n\t"
                "movq $1000000000, %%rax\n\t"
                "mulq %%r11\n\t"/*remainders[j]*SCALE*/
                "movq %%rax, %%r11\n\t"
                "movq %%r8, %%rax\n\t"
                "incq %%rax\n\t"
                "mulq %%r9\n\t"
                "movq %%rax, %%r10\n\t"
                "addq %%r11, %%r10\n\t"/*sum = remainders[j]+quotient*(j+1)*/
                "movq %%r10, %%rax\n\t"
                "movq %%r8, %%rbx\n\t"
                "addq %%rbx, %%rbx\n\t"
                "incq %%rbx\n\t"
                "xorq %%rdx, %%rdx\n\t"
                "divq %%rbx\n\t"
                "movq %%rax, %%r9\n\t"/*quotient = sum / (2*j+1)*/
                "popq %%rax\n\t"
                "movq %%rdx, (%%rax, %%r8, 8)\n\t"/*remainders[j] = sum - quotient*(2*j+1)*/
                "decq %%r8\n\t"
            "cmpq $0, %%r8\n\t"
            "jge inner\n\t"
        "movq $1000000000, %%rbx\n\t"
        "pushq %%rax\n\t"
        "movq %%r10, %%rax\n\t"
        "xorq %%rdx, %%rdx\n\t"
        "divq %%rbx\n\t"
        "movq %%rax, %%r9\n\t"/*quotient = sum / SCALE;*/
        "popq %%rax\n\t"
        "movq %%rdx, (%%rax)\n\t" /*remainders[0] = sum - quotient*SCALE;*/
        "popq %%rdx\n\t"
        "movq %%r9, (%%rsi, %%rcx, 8)\n\t"/*answ[i] = quotient;*/ 
        "incq %%rcx\n\t" 
        "cmpq %%rdi, %%rcx\n\t"
        "jb outer\n\t"
        :"=c"(i)/* no outputs */
        :"a"(remainders),"c"(i), "d"(array_length), "D"(digits_amount), "S"(answ)/* inputs */
        :/* clobbered */
    );
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