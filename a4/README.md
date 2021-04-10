# Assignment 4

## Compiling and Running
- You can run the code using the run.sh script as: ```./run.sh``` (you might have to run ```sudo chmod +x run.sh``` before)
- Or you can run it manually using the command: ```g++ -pthread main.cpp && ./a.out > out.txt```
- You can then see the output in ```out.txt``` file. (don't open the file with vim/nano/other old text editors, while the program is running)
- If you wish to see the output on terminal only, run this: ```g++ -pthread main.cpp && ./a.out```

## Note
- The buffer size is currently 5000, you can change that in line 12 of main.cpp -> ```#define M 5000```
- The quantum for round robin is 1 sec, you can change that in line 13 of main.cpp -> ```#define QUANTUM 1```

## Assumptions
- The maximum number of worker threads can be 200.
- The consumer threads will be stopped if there are no active producers and buffer is empty.

## Bugs
- No known bugs.

