import random
import sys
import subprocess
import os

def generate():
    random.seed()
    result = "void randomize_pht(){\n"
    result += '\tasm volatile("cmp %' + 'rcx, %' + 'rcx; '
    for i in range(100000):
        r = random.randint(0, 3)
        if r == 0:
            result += 'je pht' + str(i) + '; '
            result += 'pht' + str(i) + ': '

        elif r == 1:
            result += 'je pht' + str(i) + '; nop; '
            result += 'pht' + str(i) + ': '

        elif r == 2:
            result += 'jne pht' + str(i) + '; '
            result += 'pht' + str(i) + ': '

        elif r == 3:
            result += 'jne pht' + str(i) + '; nop; '
            result += 'pht' + str(i) + ': '

    result += 'nop;");\n}'
    f = open("randomize_pht.c", "w")
    f.write(result)
    f.close()

if len(sys.argv) > 1:
    print("Generating " + sys.argv[1] + " PRC...")
    while(1):
        generate()
        os.system("make PRC")
        res = subprocess.check_output(["./check_PRC.out"], shell=True, encoding='utf-8')
        if sys.argv[1] in res:
            print(sys.argv[1]+" PRC found, exit process")
            break

else:
    print("Usage: python3 generate_rand.py (SN/WN/WT/ST)")
    print("Generating random PRC...")
    generate()