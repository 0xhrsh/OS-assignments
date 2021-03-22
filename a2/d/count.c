#include <stdio.h>
#include <stdlib.h>

int main(){
    int count = 0;
    char c;
    
    while((c = getchar()) != 255 && c != EOF){
        if((c < 'a' || c > 'z')&&(c < 'A' || c > 'Z')) count++;
        putchar(c);
    }

    fprintf(stderr, "Number of non-alphabetical characters: %d\n", count);
    exit(0);

    return 0;

}