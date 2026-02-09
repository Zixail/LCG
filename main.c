#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 16
#define INPUT "input.txt"
#define OUTPUT "output.txt"

char * get_c(char *);

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
    fclose(fp);

    return command;
}

//  Функция определения прочитанной комманды и её запуска
void * executeCommand(char * command){
    char * comms [4] = {"get_c", "get_a", "lcg", "test"};
    void (*func[4])(char *) = {get_c};

    char * sep = strchr(command, ' ');
    int length = sep - command;
    printf("%d\n", length);

    for (int i = 0; i < 4; ++i){
        if (strncmp(command, comms[i], length) == 0){
            printf("%s\n", comms[i]);
            func[i](command);
            return 0;
        }
    }
    printf("no func");
}

void getArg(char * command, int length, char * tmpl[], char * args[]){
    for(int i = 0; i < length; ++i){
        char * ptr = strstr(command, tmpl[i]);
        ptr = strchr(ptr, '=') + 1;
        char * sep = strchr(ptr, ' ');
        int length = sep - ptr;
        strncpy(args[i], ptr, length);
        printf("%s\n", args[i]);
    }
}

void get_c(char * command){
    char * tmpl[] = {"cmin", "cmax", "m"};
    char * args[3] = {0};
    getArg(command, 3, tmpl, args);
    strcpy(command, "здесь должен быть вывод результата");
}

void get_a(char * command){
    char * tmpl[] = {"m"};
    char * args[1] = {0};
    getArg(command, 1, tmpl, args);
    strcpy(command, "здесь должен быть вывод результата");
}

void lcg(char * command){
    char * tmpl[] = {"a", "x0", "c", "m", "n"};
    char * args[5] = {0};
    getArg(command, 1, tmpl, args);
    strcpy(command, "здесь должен быть вывод результата");
}

void test(char * command){
    char * tmpl[] = {"inp"};
    char * args[1] = {0};
    getArg(command, 1, tmpl, args);
    strcpy(command, "здесь должен быть вывод результата");
}

int main(void){
    char * command = takeCommand();
    printf("%s\n", command);
    executeCommand(command);

    return 0;
}