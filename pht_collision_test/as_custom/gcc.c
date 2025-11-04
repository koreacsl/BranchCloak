#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <regex.h>

int main(int argc, const char * * argv){
    regex_t state_c, state_start, state_end, state_if, state_already_safe;
    const char *branch = "int __saferand__=rand()%2;\nif (__saferand__){asm volatile(\"nop;\":::\"rax\",\"rdx\");} /*SAFE_BRANCH*/\nif (__saferand__){asm volatile(\"nop;\":::\"rax\",\"rdx\");} /*SAFE_BRANCH*/\n";
    const char *regex_c = ".+\\.c$";
    const char *regex_start = "SAFE[0-9]+_start:";
    const char *regex_end = "SAFE[0-9]+_end:";
    const char *regex_if = "if\\s*\\(.+\\)";
    const char *regex_already_safe = "if\\s*\\(.+\\).+SAFE_BRANCH";
    regcomp(&state_c, regex_c, REG_EXTENDED);
    regcomp(&state_start, regex_start, REG_EXTENDED);
    regcomp(&state_end, regex_end, REG_EXTENDED);
    regcomp(&state_if, regex_if, REG_EXTENDED);
    regcomp(&state_already_safe, regex_already_safe, REG_EXTENDED);
    int status;
    int modified = 0;

    // for (int i = 0; i < argc; i++){
    //     printf("%d %s\n", i, argv[i]);
    // }

    int c_idx = 0;
    /* need to be changed */
    char cmd[65535] = "/usr/bin/gcc-7";
    for (int i=0;i<argc;i++){
        status = regexec(&state_c, argv[i], 0, NULL, 0);
        if (status == 0){
            printf("C file: %s\n", argv[i]);
            c_idx = i;
        }
    }
    if (c_idx == 0){
        printf("No C files!\n");
        goto CALL_GCC;
    }
    FILE *fp = fopen(argv[c_idx], "r");
    FILE *fp2 = fopen("tmp.c", "w");

    int safe = 0;
    char line[1023];
    char parsed[1023];
    while (fgets(line, sizeof(line), fp) != NULL ) {
        // sscanf(line, "%s", parsed);
        // printf("%s -> %s\n", line, parsed);
        if (safe == 0){
            status = regexec(&state_start, line, 0, NULL, 0);
            if (status == 0){
                printf("[*] SAFE start\n");
                safe = 1;
            }
        }
        else {
            status = regexec(&state_end, line, 0, NULL, 0);
            if (status == 0){
                printf("[*] SAFE end\n");
                safe = 0;
            }
            else{
                // align branch
                status = regexec(&state_if, line, 0, NULL, 0);
                if (status == 0){
                    status = regexec(&state_already_safe, line, 0, NULL, 0);
                    if (status != 0){
                        printf("[*] branch inserted\n");
                        modified += 1;
                        fputs(branch, fp2);
                        line[strlen(line) - 1] = '\0';
                        strcat(line, " /*SAFE_BRANCH*/\n");
                    }
                    else {
                        printf("[*] already obscured branches\n");
                    }
                }
            }
        }
        
        fputs(line, fp2);
	}

    fclose(fp);
    fclose(fp2);
CALL_GCC:
    for (int i = 1; i < argc; i++){
        // if (i == c_idx){
        //     strcat(cmd, " tmp.c");    
        // }
        // else {
            strcat(strcat(cmd, " "), argv[i]);
        // }
    }

    if (modified >= 1){
        printf("[*] %d branches aligned\n", modified);
        system("cp tmp.c tmp_modified.c");
    }

    if (c_idx != 0){
        char cmd2[255]="cp tmp.c ";
        strcat(cmd2, argv[c_idx]);
        printf("%s\n", cmd2);
        system(cmd2);
        system("rm tmp.c");
    }

    printf("%s\n", cmd);
    system(cmd);

    return 0;
}
