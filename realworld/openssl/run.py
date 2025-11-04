import random
import sys
import subprocess
import os
import numpy as np
np.set_printoptions(suppress=True)

functioncallnum=18640000
# pagefault, branch miss rate, cache miss rate, execution time
arr_align = np.empty((4, 0))
arr_raw = np.empty((4, 0))
if len(sys.argv) == 2:
    print("Testing OpenSSL...", end='\r')
    for i in range(100):
        print("Testing OpenSSL... " + str(i), end='\r')
        tmp_align = np.array([])
        tmp_raw = np.array([])
        res = subprocess.check_output(["./run.sh " + sys.argv[1] + " >/dev/null"], shell=True, encoding='utf-8')

        f = open("result_perf_aligned.txt","r")
        while True:
            line = f.readline()
            if not line: break
            
            if "page-faults" in line: # tmp[0]
                tmp_align = np.append(tmp_align, int(line.split()[0].replace(",","")))

            elif "branch-misses" in line: # tmp[1]
                tmp_align = np.append(tmp_align, int(line.split()[0].replace(",","")))

            elif "branches" in line: # tmp[2]
                tmp_align = np.append(tmp_align, int(line.split()[0].replace(",","")))

            elif "cache-misses" in line: # tmp[3]
                tmp_align = np.append(tmp_align, int(line.split()[0].replace(",","")))

            elif "cache-references" in line: # tmp[4]
                tmp_align = np.append(tmp_align, int(line.split()[0].replace(",","")))

            elif "seconds time elapsed" in line: # tmp[5]
                tmp_align = np.append(tmp_align, functioncallnum / float(line.split()[0]))

        tmp_align[1] = float(tmp_align[1]) / float(tmp_align[2]) * 100
        tmp_align[2] = float(tmp_align[3]) / float(tmp_align[4]) * 100
        tmp_align = np.delete(tmp_align, [3, 4])
        tmp_align = tmp_align.reshape((4, 1))
        arr_align = np.append(arr_align, tmp_align.reshape((4, 1)), axis=1)
        f.close()
        f = open("result_perf_raw.txt","r")
        while True:
            line = f.readline()
            if not line: break
            
            if "page-faults" in line: # tmp[0]
                tmp_raw = np.append(tmp_raw, int(line.split()[0].replace(",","")))

            elif "branch-misses" in line: # tmp[1]
                tmp_raw = np.append(tmp_raw, int(line.split()[0].replace(",","")))

            elif "branches" in line: # tmp[2]
                tmp_raw = np.append(tmp_raw, int(line.split()[0].replace(",","")))

            elif "cache-misses" in line: # tmp[3]
                tmp_raw = np.append(tmp_raw, int(line.split()[0].replace(",","")))

            elif "cache-references" in line: # tmp[4]
                tmp_raw = np.append(tmp_raw, int(line.split()[0].replace(",","")))

            elif "seconds time elapsed" in line: # tmp[5]
                tmp_raw = np.append(tmp_raw, functioncallnum / float(line.split()[0]))

        tmp_raw[1] = tmp_raw[1] / tmp_raw[2] * 100
        tmp_raw[2] = tmp_raw[3] / tmp_raw[4] * 100
        tmp_raw = np.delete(tmp_raw, [3,4])
        tmp_raw = tmp_raw.reshape((4, 1))
        arr_raw = np.append(arr_raw, tmp_raw.reshape((4, 1)), axis=1)
        f.close()
    print(arr_align)
    print(arr_raw)

    f = open("result.txt","w")
    f.write("***** OpenSSL *****\n")
    f.write("[*] aligned\n")
    f.write("page-faults, branch misprediction rate, cache miss rate, iterations per second\n")
    f.write(str(np.mean(arr_align[0]))+'('+str(np.std(arr_align[0]))+'), ')
    f.write(str(np.mean(arr_align[1]))+'('+str(np.std(arr_align[1]))+'), ')
    f.write(str(np.mean(arr_align[2]))+'('+str(np.std(arr_align[2]))+'), ')
    f.write(str(np.mean(arr_align[3]))+'('+str(np.std(arr_align[3]))+')\n\n')

    f.write("[*] raw\n")
    f.write("page-faults, branch misprediction rate, cache miss rate, execution time\n")
    f.write(str(np.mean(arr_raw[0]))+'('+str(np.std(arr_raw[0]))+'), ')
    f.write(str(np.mean(arr_raw[1]))+'('+str(np.std(arr_raw[0]))+'), ')
    f.write(str(np.mean(arr_raw[2]))+'('+str(np.std(arr_raw[0]))+'), ')
    f.write(str(np.mean(arr_raw[3]))+'('+str(np.std(arr_raw[0]))+')\n')
    f.close()

else:
    print("Usage: python3 run.py [sudo passwd]")
