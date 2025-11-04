#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


int test(int secret){
        asm volatile("SAFE1_start:":::"rax","rdx");
        if (secret){
                asm volatile("nop;":::"rax","rdx");
        }
        else {
                asm volatile("nop;":::"rax","rdx");
        }
        asm volatile("SAFE1_end:":::"rax","rdx");
        return 1;
}

int branch_one(unsigned int value){
        uint64_t start, d;
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
}

int main(int argc, const char * * argv){
    test("hello world");
    printf("hello world\n");
    return  0;
}