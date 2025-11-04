#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "./branchutils.h"
#include "./f_rdmsr.c"

#define test_n 1000
int target_branch(unsigned int value){ // (1:Taken, 0:Not-taken)
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

int pht_collision(int num){
    uint64_t start = 0, end = 0;
    randomize_pht(); //1-level predictor

    int ret = 0;
    for (int i=0;i<2;i++){
        int result = target_branch(num);

        if (result < BRANCH_MISS){
            ret += 1;
        }
    }
    return ret;
}

int msr(int num){
    uint32_t start = 0, end = 0;
    randomize_pht(); //1-level predictor
    
    mfence();
    target_branch(1);
    start = rdmsr_on_cpu(0xc1, 0);
    mfence();
    target_branch(num);
    mfence();

    end = rdmsr_on_cpu(0xc1, 0);

    mfence();

//    printf("%d %d\n", start, end);
    int result = (int)(end - start);

    printf("%d ", result);
    return 1;
}

int main(int argc, const char * * argv){
    BRANCH_MISS = detect_branch_prediction_threshold();
    printf("BRANCH MISS THRESHOLD: %d\n", BRANCH_MISS);

    int arr[5] = {0, };
    for (int i=0;i<test_n;i++){
        /* TT Probe */
        int TT = pht_collision(1);
        /* NN Probe */
        int NN = pht_collision(0);

        if (TT == 2 && NN == 0){
            arr[0]++;
        }
        else if (TT == 2 && NN == 1){
            arr[1]++;
        }
        else if (TT == 1 && NN == 2){
            arr[2]++;
        }
        else if (TT == 0 && NN == 2){
            arr[3]++;
        }
        else {
            arr[4]++;
        }
    }
    for (int i=0;i<5;i++){
        if (arr[i] > test_n * 0.85){
            switch(i){
                case 0:
                    printf("ST: %d\n", arr[i]);
                    break;
                case 1:
                    printf("WT: %d\n", arr[i]);
                    break;
                case 2:
                    printf("WN: %d\n", arr[i]);
                    break;
                case 3:
                    printf("SN: %d\n", arr[i]);
                    break;
                default:
                    printf("error\n");
                    break;
            }
        }
    }
    printf("%d %d %d %d %d", arr[0],arr[1],arr[2],arr[3],arr[4]);
    return 0;
}
