#include <stdio.h>
#include <stdlib.h>

int main(){
    char c;
    
    while((c = getchar()) != 255 && c != EOF){
        if(c >= 'a' && c <= 'z'){
            c = c - 'a' + 'A';
        }
        else if(c >= 'A' && c <= 'Z'){
            c = c + 'a' - 'A';
        }
        putchar(c);
    }

    exit(0);
    return 0;

}