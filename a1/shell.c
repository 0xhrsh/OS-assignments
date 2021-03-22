#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h> 
#include <stdbool.h>
#include <signal.h> 


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/
char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void sigintHandler(int sig_num)
{
    signal(SIGINT, sigintHandler);
    printf("Invalid Input. To terminate the program, press Ctrl+D or type 'exit'\n");
    fflush(stdout);
}

void sigtstpHandler(int sig_num)
{
    signal(SIGTSTP, sigintHandler);
    printf("Invalid Input. To terminate the program, press Ctrl+D or type 'exit'\n");
    fflush(stdout);
}

void sigsegvHandler(int sig_num)
{
    signal(SIGSEGV, sigsegvHandler);
    fflush(stdout);
    exit(0);
}


int main(int argc, char* argv[]) {
    // signal(SIGINT, sigintHandler);
    // signal(SIGTSTP, sigtstpHandler);
    // signal(SIGSEGV, sigsegvHandler);

	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
    char cwd[1000];

	FILE* fp;
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {			
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
            
            getcwd(cwd, sizeof(cwd));
			printf("~%s $ ", cwd);
			scanf("%[^\n]", line);
			getchar();
		}
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
        printf(line);
        
        
        bool wt = true;
        int jt = 0;
        
        int p[2];
        if (pipe(p)==-1) { 
            printf("Pipe Failed"); 
            return 1; 
        } 


        for(jt=0;tokens[jt]!=NULL;jt++);
   
        if (jt>0 && strcmp(tokens[jt-1],"&")==0){
            wt = false;
            tokens[jt-1] = NULL;
        }
        

        for(i=0;tokens[i]!=NULL;i++){
            int j = i+1;
            while(tokens[j]!=NULL && strcmp(tokens[j], "|")!=0){
                j++;

            }

            if (strcmp(tokens[i], "exit") == 0){ 
                exit(1);
            } else if (strcmp(tokens[i], "cd")==0){
                chdir(tokens[i+1]);
            } else if (strcmp(tokens[i], "ls")==0){
                struct dirent *de;
                DIR *dr = opendir(".");
                while ((de = readdir(dr)) != NULL){
                    printf("%s ", de->d_name); 
                }
                printf("\n");
                closedir(dr);
            } else if (strcmp(tokens[i], "pwd")==0){
                getcwd(cwd, sizeof(cwd));
                printf("%s\n", cwd);
            } else{
                char **args = (char **)malloc((j-i) * sizeof(char *));

                for(int k = i; k<j; k++){
                    args[k-i] = tokens[k];
                }

                pid_t pid = fork();
                if(pid == -1){
                    printf("Failed forking child.\n");
                } else if (pid == 0){
                    int status = execvp(tokens[i], args);
                    // write(p[1], "random string", strlen("random string"));
                    if (status < 0){
                        printf("Could not execute command\n");
                    }
                    exit(0);
                } else if (wt) {
                    wait(pid);
                }
                // char concat_str[100];
                // read(p[0], concat_str, strlen("random shit")); 
                // printf("Concatenated string %s\n", concat_str);
            }
            i = j;
        }
       
		// Freeing the allocated memory	
	    for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}