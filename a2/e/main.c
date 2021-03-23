#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include<time.h>

#define BUFSIZE 10
#define LINE printf("\n===============================\n")

int nums[26];
char* colors[] = {"Violet", "Indigo", "Blue", "Green", "Yellow", "Orange", "Red"};

int main(){

    nums['V' - 'A'] = 1;
    nums['I' - 'A'] = 2;
    nums['B' - 'A'] = 3;
    nums['G' - 'A'] = 4;
    nums['Y' - 'A'] = 5;
    nums['O' - 'A'] = 6;
    nums['R' - 'A'] = 7;

    int fd1[2];
    int fd2[2];

    pipe(fd1);
    pipe(fd2);
    

    if(fork() == 0){ // C1 Process
        close(fd2[0]); // Closing C2 Pipe
        close(fd2[1]);

        while(true){
            sleep(1);
            write(fd1[1], colors[rand()%7], BUFSIZE);
        }

    } else {
        if(fork() == 0){ // C2 Process
            close(fd1[0]); // Closing C1 Pipe
            close(fd1[1]);
        
            srand(time(0));

            while(true){
                sleep(1);
                write(fd2[1], colors[rand()%7], BUFSIZE);
            }

        } else{ // Parent Process
            int score1 = 0, score2 = 0;

            sleep(1);
            srand(time(0));

            while(true){
                char* p = colors[rand()%7]; // equal to som random number
                char x[BUFSIZ], y[BUFSIZ];
                read(fd1[0], x, BUFSIZE);
                read(fd2[0], y, BUFSIZE);

                printf("C1 chose: %s, C2 chose: %s, P chose: %s\n", x, y, p);
                if(x[0] == p[0] && y[0] == p[0]){
                    printf("Round Ignored\n\n");
                }else if(x[0] == p[0]){
                    LINE;
                    printf("\nC1 wins\n");
                    break;
                } else if(y[0] == p[0]){
                    LINE;
                    printf("\nC2 wins\n");
                    break;
                } else {
                    score1 += abs(nums[x[0] - 'A'] - nums[p[0] - 'A']);
                    score2 += abs(nums[y[0] - 'A'] - nums[p[0] - 'A']);
                    printf("Score: C1:%d, C2:%d\n\n", score1, score2);
                }

                if(score1 > 50 && score2 <=50){
                    LINE;
                    printf("\nC1 wins with score: %d\n", score1);
                    break;
                } else if(score2 > 50 && score1 <= 50){
                    LINE;
                    printf("\nC2 wins with score: %d\n", score2);
                    break;
                } else if(score1 > 50 && score2 > 50){
                    LINE;
                    printf("Draw!\n");
                    break;
                }
                sleep(1);
            }
        }
    }
    return 0;   
}