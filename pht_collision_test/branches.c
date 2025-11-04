int branch_one(unsigned int value){ // (1:Taken, 0:Not-taken)
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

int branch_two(unsigned int value){ // (1:Taken, 0:Not-taken)
    uint64_t start, end, d;
    asm volatile(
        "cmp $0x1, %%rcx;"
        ".p2align 15, 0x90;"
        "mfence;"
        "rdtsc;"
        "mfence;"
        "je label1;"
        "label1:"
        "nop;"
        :"=a"(start), "=d"(d):"c"(value));
    start = (d << 32) | start;
    end = rdtsc();
    return (int)(end - start);
}
