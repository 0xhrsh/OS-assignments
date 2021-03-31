import subprocess
import os
import matplotlib.pyplot as plt

def compile():
    subprocess.call(["g++", "a.cpp"])
    subprocess.call(["g++", "B18CSE016.cpp", "-o", "scheduler"])
    print("Compilation Done!")



def saveData(arr):
    print("Running program for various N")
    
    os.system('mkdir data')
    os.system('mkdir graphs')
    i = 0
    while(i<iterations):
        for n in arr:
            os.system("./a.out {}".format(n))
            os.system("./scheduler < in.txt > data/out{}_{}.txt".format(n, i+1))
        i+=1
    print("Programs ran successfully")

#att
def make_att_graphs():
    for mode in modes:
        for algo in sch_algos:    
            plt.plot(arr, att[mode][algo], label = name_algos[algo], marker='o')

        plt.xlabel('Value of N')
        plt.ylabel('{} ATT (over 10 Samples)'.format(names_modes[mode]))
        plt.title('{} ATT (over 10 samples) vs N'.format(names_modes[mode]))
        plt.legend()
        plt.savefig('graphs/{}_ATT.png'.format(names_modes[mode]))
        plt.close()

# awt
def make_awt_graphs():
    for mode in modes:
        for algo in sch_algos:    
            plt.plot(arr, awt[mode][algo], label = name_algos[algo], marker='o')

        plt.xlabel('Value of N')
        plt.ylabel('{} AWT (over 10 Samples)'.format(names_modes[mode]))
        plt.title('{} AWT (over 10 samples) vs N'.format(names_modes[mode]))
        plt.legend()
        plt.savefig('graphs/{}_AWT.png'.format(names_modes[mode]))
        plt.close()

#art
def make_art_graphs():
    for mode in modes:
        for algo in sch_algos:    
            plt.plot(arr, art[mode][algo], label = name_algos[algo], marker='o')

        plt.xlabel('Value of N')
        plt.ylabel('{} ART (over 10 Samples)'.format(names_modes[mode]))
        plt.title('{} ART (over 10 samples) vs N'.format(names_modes[mode]))
        plt.legend()
        plt.savefig('graphs/{}_ART.png'.format(names_modes[mode]))
        plt.close()


if __name__ == '__main__':
    
    compile()
    arr = [10, 20, 30, 40, 50]
    iterations = 10
    saveData(arr)
    
    # Initialising stuff
    att = {}
    awt = {}
    art= {}

    sch_algos = ["FCFS", "NPSJF", "PSJF", "RR", "Priority"]
    name_algos_list = ["First come first serve", "NP Shortest job first", "P shortest job first", "Round robin (δ = 2)", "Priority Based"]
    name_algos = {}
    i = 0
    for algo in sch_algos:
        name_algos[algo] = name_algos_list[i]
        i+=1
    
    modes = [0, 1, 2]
    names_modes = ["Average", "Maximum", "Minimum"]
    mode_val = [0.0, -1.0, 100000.0]

    for mode in modes:
        att[mode] = {}
        awt[mode] = {}
        art[mode] = {}
        for algo in sch_algos:
            
            att[mode][algo] = [mode_val[mode]] * len(arr)
            awt[mode][algo] = [mode_val[mode]] * len(arr)
            art[mode][algo] = [mode_val[mode]] * len(arr)


    # reading and organize data, so graphs can be made easily
    print("Reading and compiling data!")
    for n in range(len(arr)):
        i = 0
        while(i<iterations):
            f = open("data/out{}_{}.txt".format(arr[n], i+1))
            out = f.read().split("\n")[2:-1]
            for algo in out:
                nums = algo.split("\t")
            
                temp_att = float(nums[-3])
                temp_awt = float(nums[-2])
                temp_art = float(nums[-1])
                
                att[0][nums[0]][n] += temp_att/iterations
                awt[0][nums[0]][n] += temp_awt/iterations
                art[0][nums[0]][n] += temp_art/iterations

                att[1][nums[0]][n] = max(temp_att, att[1][nums[0]][n])
                awt[1][nums[0]][n] = max(temp_awt, awt[1][nums[0]][n])
                art[1][nums[0]][n] = max(temp_art, art[1][nums[0]][n])

                att[2][nums[0]][n] = min(temp_att, att[2][nums[0]][n])
                awt[2][nums[0]][n] = min(temp_awt, awt[2][nums[0]][n])
                art[2][nums[0]][n] = min(temp_art, art[2][nums[0]][n])
        
            i += 1
    
    print("Making graphs!") 
    make_art_graphs()
    make_awt_graphs()
    make_att_graphs()

