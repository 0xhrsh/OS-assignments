m = 1

import subprocess
import os
# subprocess.call(["g++", "a.cpp"])
# tmp=subprocess.call(["g++", "b.cpp", "-o", "scheduler"])
# print("Compilation done!")

arr = [10, 20, 30, 40, 50]
arr= [10]
i = 0
# os.system('mkdir data')

# while(i<m):
#     for n in arr:
#         os.system("./a.out 10")
#         os.system("./scheduler < in.txt > data/out{}_{}.txt".format(n, i+1))
#     i+=1


att = {}
awt = {}
art= {}

for n in arr:
    i = 0
    while(i<m):
        f = open("data/out{}_{}.txt".format(n, i+1))
        out = f.read().split("\n")[2:]
        for algo in out:
            nums = algo.split("\t")
            
        print(out)
    i += 1
