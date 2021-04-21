# Assignment 5

## Compile and Run:
- You can simply run using the command: ```./run.sh``` [Note: Run ```sudo chmod +x run.sh``` in case of any permission error]
- You can also run it manually using the following command: ```g++ main.cpp -pthread && ./a.out -b=2 -c=1 -w=1 > logs.txt``` [Here b, c, w are the number of barbers, chairs, capacity of waiting room respectively.]
- After completing the above steps, you will be prompted to add the number of customers.
- Email anand.2@iitj.ac.in in case of any errors.

## Bugs:
- No known bugs.
- The solution is free from deadlock and starvation! (since no semaphores are in a circular loop)

## Assumptions:
- Max numbers of barbers and customers is 500.
- Cutting Hair and cleaning chair both consume 1 sec. [this value can be changed in line 6 of classes.cpp]
- All Customers are coming at t0, this can be changed by uncommenting line 45 of main.cpp. [Warning: Uncommenting that line can make the program very slow for large number of customers]

## Important: 
- The program is able to do all the required tasks
- Synchronized printing of the messages is also implemented!