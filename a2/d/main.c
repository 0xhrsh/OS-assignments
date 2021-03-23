#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include<sys/wait.h>
#include <fcntl.h>

extern int errno ;

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Incorrect argument format. Please provide 2 file names\n");
        exit(1);
    }
    int f1, f2;

    if ((f1 = open(argv[1], O_RDONLY)) == -1){
        fprintf(stderr, "Error opening %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }dup2(f1, 0); // Reading from file1

    if ((f2 = open(argv[2], O_WRONLY | O_TRUNC | O_CREAT)) == -1){
        fprintf(stderr, "Error opening %s: %s\n", argv[2], strerror(errno));
        exit(1);
    }dup2(f2, 1); // Writing to f2

    int fd[0];
    pipe(fd); 

    if (fork() == 0) { // First Child process
        dup2(fd[0], 0); // Reading from pipe
        close(fd[1]); // This child won't write in pipe

        if(execl("./count.out", NULL)==-1){
            fprintf(stderr, "Please compile count.c to count.out before running.\n");
            exit(1);
        }

        close(fd[0]);
        fprintf(stderr, "Read pipe closed\n");
        
    }
    else { // Parent process

        if (fork() == 0){ // Second Child
            dup2(fd[1], 1); // Writing to pipe
            close(fd[0]); // This child won't read from pipe

            if(execl("./convert.out", NULL)==-1){
                fprintf(stderr, "Please compile convert.c to convert.out before running.\n");
                putchar(EOF);
                exit(1);
            }

            close(fd[1]);
            fprintf(stderr, "Write pipe closed\n");

        } else{
            close(fd[1]);
            close(fd[0]);

            wait(NULL);
            wait(NULL);
            
            close(f1);
            close(f2);
        }
    }
    return 0;
}