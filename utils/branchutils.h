
#ifndef _CACHEUTILS_H_
#define _CACHEUTILS_H_

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include "randomize_pht.c"

/* ============================================================
 *                    User configuration
 * ============================================================ */
size_t BRANCH_MISS = 150;

#define TEST_DISTRIBUTION       0

/* ============================================================
 *                  User configuration End
 * ============================================================ */

// ---------------------------------------------------------------------------
uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile("mfence");
  asm volatile("rdtsc" : "=a"(a), "=d"(d));
  a = (d << 32) | a;
  asm volatile("mfence");
  return a;
}

int test_branch(unsigned int value){ // (1:Taken, 0:Not-taken)
    uint64_t start, end, d;
    asm volatile(
        "cmp $0x1, %%rcx;"
        ".p2align 15, 0x90;"
        "mfence;"
        "rdtsc;"
        "mfence;"
        "je label_test;"
        "label_test:"
        "nop;"
        :"=a"(start), "=d"(d):"c"(value));
    start = (d << 32) | start;
    end = rdtsc();
    return (int)(end - start);
}

// ---------------------------------------------------------------------------
void mfence() { asm volatile("mfence"); }

// ---------------------------------------------------------------------------
int branch_miss_t() {
    randomize_pht(); //1-level predictor
    test_branch(1); //Taken
    test_branch(1); //Taken
    uint64_t start = 0, end = 0;

    int result = test_branch(0); //Not-taken
    return result;
}

// ---------------------------------------------------------------------------
int branch_hit_t() {
    randomize_pht(); //1-level predictor
    test_branch(1); //Taken
    test_branch(1); //Taken
    uint64_t start = 0, end = 0;

    int result = test_branch(1); //Taken
    return result;
}


// ---------------------------------------------------------------------------
size_t result[2][1000];
size_t detect_branch_prediction_threshold() {
    size_t hit_time = 0, miss_time = 0, i, count = 1000;

    for (i = 0; i < count; i++) {
#if TEST_DISTRIBUTION
        result[0][i] = branch_hit_t();
        hit_time += result[0][i];
#else
        hit_time += branch_hit_t();
#endif
    }
    for (i = 0; i < count; i++) {
#if TEST_DISTRIBUTION
        result[1][i] = branch_miss_t();
        miss_time += result[1][i];
#else
        miss_time += branch_miss_t();
#endif
    }
    hit_time /= count;
    miss_time /= count;

#if TEST_DISTRIBUTION
    FILE *fp = fopen("branch_hit_miss_distribution.txt", "w");
    for (int i=0;i<2;i++){
        for (int j=0;j<1000;j++){
            fprintf(fp, "%d ", result[i][j]);
        }
        fprintf(fp, "\n");
    }
#endif

    printf("Hit: %d, Miss: %d\n", hit_time, miss_time);

    return (hit_time + miss_time * 2) / 3;
}

#endif