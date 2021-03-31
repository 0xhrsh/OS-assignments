# Part a:

You can compile part a using: ```g++ a.cpp```.
To run part a, you have to provide n in cmd line argument. For example: ```./a.out 10```
This will create the appropriate output in file ```in.txt```.
The output will in the format of:
```
n
arrival_time1 cpu_burst1 priority1
arrival_time2 cpu_burst2 priority2
so on...
```


# Part b:

you can compile part b using ```g++ b.cpp -o scheduler```
You can run using the command ```./scheduler < in.txt > out.txt```
You will get the appropriate output in the file ```out.txt```.

# Part c:
run: ```python3 c.py```

This will:

- Create 10 output files each with n = 10/20/30/40/50 in the folder ```data/```.
- Run the scheduler program for each of the files in the folder ```data/```.
- Create the output graphs in the folder ```graphs/```.

