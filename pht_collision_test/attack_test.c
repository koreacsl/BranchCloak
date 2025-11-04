#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/branchutils.h"
#include "./branches.c"

unsigned int secret[10000];
unsigned int attack[10000];
int pht_collision(int idx){
    uint64_t start = 0, end = 0;
    randomize_pht();

    branch_one(1); // Taken
    branch_one(1); // Taken

    int latency = branch_two(secret[idx]); // Predicted as Taken..??
   
    if (latency < BRANCH_MISS){
        attack[idx] = 1;
    }
    else attack[idx] = 0;
}

int main(int argc, const char * * argv){
    srand(time(NULL));
    BRANCH_MISS = detect_branch_prediction_threshold();
    printf("BRANCH MISS THRESHOLD: %d\n", BRANCH_MISS);
    printf("PC of Branch 1: %p\nPC of Branch 2: %p\n", branch_one, branch_two);
    
    for (int i=0;i<10000;i++){
        secret[i] = rand() % 2;
    }

    for (int i=0;i<10000;i++){
        pht_collision(i);
    }

    int count=0, ones=0;
    for (int i=0;i<10000;i++){
        if (attack[i] == secret[i]){
            count++;
        }
        if (secret[i] == 1){
            ones++;
        }
    }
    printf("Collision Generated: %d/10000, One: %d\n", count, ones);
}