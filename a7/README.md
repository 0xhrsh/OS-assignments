# Assignment 7

## Compiling and Executing:
- ```g++ main.cpp && ./a.out > Result.txt```

## Interpreting Output
- ```P0: page 9, TLB miss → page fault → free frame: 0 allocated to it.```: This means
Process 0 requested for page 9 which lead to a TBL miss and a page fault. A free frame (frame 0) was found and was allocated to the page.

- ```P0: page 9, TLB miss → page table valid → with frame no. 0```: This means
Process 0 requested for page 9 which lead to a TBL miss. The page was found in the page table and frame 0 was allocated to it.

- ```P0: page 3, TLB miss → page fault → frame: 0 re-allocated to it. (previously alloc to pg: 9)```: This means
Process 0 requested for page 3 which lead to a TBL miss and a page fault. Frame 0 was then allocated to it, (where frame 0 was previously allocated to page 9)

- ```P0: page 3, TLB hit with frame no. 0```: This means

Process 0 requested for page 3, it was a TBL hit and the page can be found on frame 0.

## Assumptions
- 

## Bugs
- No known bugs.
