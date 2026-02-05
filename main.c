#include <stdio.h>
#include <stdlib.h>

#define BUFF 16
#define INPUT "input.txt"
#define OUTPUT "output.txt"

//  Функция для чтения комманды из INPUT
char * takeCommand(){
    FILE * fp = fopen(INPUT, "r");
    size_t size = BUFF;
    size_t received = 0;
    char * command = calloc(size + 1, sizeof(char));

    while(fgets(command + received, size - received + 1, fp) > 0){
        received = size;
        size *= 2;
        command = realloc(command, size + 1);
    }

    return command;
}

int main(void){
    char * command = takeCommand();
    printf("%s\n", command);

    return 0;
}