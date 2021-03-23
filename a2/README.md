# Lab Assignment 2

## Question D
Note: Make sure you are in directory d before running any of the following command.

### Compilation
- You can directly run ```./run.sh```. This will do all the compilation necessary.

Or you can manually compile:

- Compile ```convert.c``` to ```convert.out``` using command ```gcc convert.c -o convert.out```
- Compile ```count.c``` to ```count.out``` using command ```gcc count.c -o count.out```
- Compile ```main.c``` to ```driver``` using command ```gcc main.c -o driver```

### Execute
You can run the code by the following command: ```./driver f1.txt f2.txt```

IMPORTANT: If you are having problems with opening the file ```f2.txt```, please run the following command: ```sudo chmod 777 f2.txt``` (This error may occur if you don't have permissions to view the file)

PS: For the above command, the program ```driver``` will take input from file ```f1.txt``` and print the output to ```f2.txt```. (Feel free to change the file names)

## Question E
Note: Make sure you are in directory e before running any of the following command.

### Compilation
You can compile using the command ```gcc main.c```

### Execute
You can execute using the command ```./a.out```

#### Assumption
- If both C1 and C2 cross the score 50 at the same time, then the game is called a draw!