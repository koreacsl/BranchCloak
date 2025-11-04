#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <regex.h>

int main(int argc, const char * * argv){
    regex_t state_elf_alloc_flag, state_asm, state_start, state_end, state_jump;
    // const char *align = "\tpushq\t%rax\n\tpushq\t%rdx\n\t.p2align 15, 0x90\n\tmfence\n\trdtsc\n\tmfence\n\tpopq\t%rdx\n\tpopq\t%rax\n";
    const char *align = "\t.p2align 15, 0x90\n\tmfence\n\trdtsc\n\tmfence\n";
    const char *elf_alloc_flag = "\t.section .note.gnu.property, \"a\"\n";
    const char *regex_elf_alloc_flag = "\\.section\\s+\\.note\\.gnu\\.property,\\s+#alloc";
    const char *regex_asm = ".+\.s$";
    const char *regex_start = "SAFE[0-9]+_start:$";
    const char *regex_end = "SAFE[0-9]+_end:$";
    const char *regex_jump = "(je|jz|jne|jnz|jg|jnle|jge|jnl|jl|jnge|jle|jng|ja|jnbe|jae|jnb|jb|jnae|jbe|jna)";
    regcomp(&state_elf_alloc_flag, regex_elf_alloc_flag, REG_EXTENDED);
    regcomp(&state_asm, regex_asm, REG_EXTENDED);
    regcomp(&state_start, regex_start, REG_EXTENDED);
    regcomp(&state_end, regex_end, REG_EXTENDED);
    regcomp(&state_jump, regex_jump, REG_EXTENDED);
    int status;
    int modified = 0;

    // for (int i = 0; i < argc; i++){
    //     printf("%d %s\n", i, argv[i]);
    // }

    int asm_idx = 0;
    for (int i=0;i<argc;i++){
        status = regexec(&state_asm, argv[i], 0, NULL, 0);
        if (status == 0){
            printf("ASM file: %s\n", argv[i]);
            asm_idx = i;
        }
    }
    if (asm_idx == 0){
        printf("No asm files!\n");
        goto CALL_AS;
    }
    FILE *fp = fopen(argv[asm_idx], "r");
    FILE *fp2 = fopen("tmp.s", "w");

    int safe = 0;
    char line[1023];
    char parsed[1023];
    char cmd[1023] = "/usr/bin/x86_64-linux-gnu-as";
    while (fgets(line, sizeof(line), fp) != NULL ) {
        sscanf(line, "%s", parsed);
        // printf("%s -> %s\n", line, parsed);
        if (safe == 0){
            status = regexec(&state_start, parsed, 0, NULL, 0);
            if (status == 0){
                printf("[*] SAFE start\n");
                safe = 1;
            }
        }
        else {
            status = regexec(&state_end, parsed, 0, NULL, 0);
            if (status == 0){
                printf("[*] SAFE end\n");
                safe = 0;
            }
            else{
                // align branch
                status = regexec(&state_jump, parsed, 0, NULL, 0);
                if (status == 0){
                    printf("[*] branch aligned\n");
                    modified += 1;
                    fputs(align, fp2);
                }
            }
        }

        // modify ELF #alloc flag
        status = regexec(&state_elf_alloc_flag, line, 0, NULL, 0);
        if (status == 0){
            printf("[*] elf alloc flag modified\n");
            fputs(elf_alloc_flag, fp2);
        }
        else fputs(line, fp2);
	}

    fclose(fp);
    fclose(fp2);

CALL_AS:
    for (int i = 1; i < argc; i++){
        // if (i == asm_idx){
        //     strcat(cmd, " tmp.s");    
        // }
        // else {
            strcat(strcat(cmd, " "), argv[i]);
        // }
    }

    if (modified >= 1){
        printf("[*] %d branched aligned\n", modified);
        system("cp tmp.s tmp_modified.s");
    }
    
    if (asm_idx != 0){
        char cmd2[255]="cp tmp.s ";
        strcat(cmd2, argv[asm_idx]);
        printf("%s\n", cmd2);
        system(cmd2);
        system("rm tmp.s");
    }

    printf("%s\n", cmd);
    system(cmd);

    return 0;
}
