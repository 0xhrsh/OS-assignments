#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include<time.h>
#include<sys/wait.h>

#define BUFSIZE 10
#define LINE printf("\n===============================\n")

int nums[26];
char* colors[] = {"Violet", "Indigo", "Blue", "Green", "Yellow", "Orange", "Red"};

void sighup1(){ 
    signal(SIGHUP, sighup1);
    printf("C1: I have Won\n"); 
} 
  
void sigint1(){ 
    signal(SIGINT, sigint1); /* reset signal */
    printf("C1: I have Lost\n"); 
}

void sighup2(){ 
    signal(SIGHUP, sighup2);
    printf("C2: I have Won\n"); 
} 
  
void sigint2(){ 
    signal(SIGINT, sigint2); /* reset signal */
    printf("C2: I have Lost\n"); 
}


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

    pid_t pid1, pid2;
    

    if((pid1 = fork()) == 0){ // C1 Process
        signal(SIGHUP, sighup1); // winner
        signal(SIGINT, sigint1); // loser

        close(fd2[0]); // Closing C2 Pipe
        close(fd2[1]);

        close(fd1[0]); // This child won't read from pipe

        while(true){
            sleep(1);
            write(fd1[1], colors[rand()%7], BUFSIZE);
        }

    } else {
        if((pid2 = fork()) == 0){ // C2 Process
            signal(SIGHUP, sighup2); // winner
            signal(SIGINT, sigint2); // loser

            close(fd1[0]); // Closing C1 Pipe
            close(fd1[1]);

            close(fd2[0]); // This child won't read from pipe
        
            srand(time(0));

            while(true){
                sleep(1);
                write(fd2[1], colors[rand()%7], BUFSIZE);
            }

        } else{ // Parent Process
            int score1 = 0, score2 = 0;
            int winner = -1;

            sleep(1);
            srand(time(0));

            close(fd1[1]); // This child won't write in pipe
            close(fd2[1]); // This child won't write in pipe

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
                    winner = 1;
                    printf("\nC1 wins\n");
                    break;
                } else if(y[0] == p[0]){
                    LINE;
                    winner = 2;
                    printf("\nC2 wins\n");
                    break;
                } else {
                    score1 += abs(nums[x[0] - 'A'] - nums[p[0] - 'A']);
                    score2 += abs(nums[y[0] - 'A'] - nums[p[0] - 'A']);
                    printf("Score: C1:%d, C2:%d\n\n", score1, score2);
                }

                if(score1 >= 50 && score2 <50){
                    LINE;
                    winner = 1;
                    printf("\nC1 wins with score: %d\n", score1);
                    break;
                } else if(score2 >= 50 && score1 < 50){
                    LINE;
                    winner = 2;
                    printf("\nC2 wins with score: %d\n", score2);
                    break;
                } else if(score1 >= 50 && score2 >= 50){
                    LINE;
                    winner = 0;
                    printf("Draw!\n");
                    break;
                }
                sleep(1);
            }
            close(fd1[0]); // closing the reading channels
            close(fd2[0]);

            if(winner == 1){
                kill(pid1, SIGHUP);
                kill(pid2, SIGINT);
            } else if(winner == 2){
                kill(pid1, SIGINT);
                kill(pid2, SIGHUP);
            } else {
                kill(pid1, SIGHUP);
                kill(pid2, SIGHUP);
            }
            
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
        }
    }
    return 0;   
}