#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF 16
#define INPUT "input.txt"
#define OUTPUT "output.txt"

char * get_c(char *);
char * get_a(char *);
char * lcg(char *);
char * test(char *);

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

//  Печать в файл
void writeResult(char * result){
    FILE * fp = fopen(OUTPUT, "w");
    fputs(result, fp);
}

//  Функция определения прочитанной комманды и её запуска
char * executeCommand(char * command){
    char * comms [4] = {"get_c", "get_a", "lcg", "test"};
    char * (*func[4])(char *) = {get_c, get_a, lcg, test};

    char * sep = strchr(command, ' ');
    size_t length = sep - command;

    char * out = "no func";
    for (int i = 0; i < 4; ++i){
        if (strncmp(command, comms[i], length) == 0){
            out = func[i](command);
            return 0;
        }
    }
    return out;
}

//  Функция для парсинга аргументов по заданному шаблону
void getArg(char * command, int length, char * tmpl[], unsigned long long args[]){
    for(int i = 0; i < length; ++i){
        char * ptr = strstr(command, tmpl[i]);
        ptr = strchr(ptr, '=') + 1;
        char * end;
        args[i] = strtoull(ptr, &end, 10);
    }
}

//  Подбор всех взаимно простых
char * get_c(char * command){
    char * tmpl[] = {" cmin", " cmax", " m"};
    unsigned long long args[3] = {0};
    getArg(command, 3, tmpl, args);
    char * out = malloc(100 * sizeof(char));
    strcpy(out, "Здесь должен быть вывод");
    return out;
}

//  Подбор минимального делящего на все простые делители
char * get_a(char * command){
    char * tmpl[] = {"m"};
    unsigned long long args[1] = {0};
    getArg(command, 1, tmpl, args);
    char * out = malloc(100 * sizeof(char));
    strcpy(out, "Здесь должен быть вывод");
    return out;
}

//  Генерация чисел 
char * lcg(char * command){
    char * tmpl[] = {" a", " x0", " c", " m", " n"};
    unsigned long long args[5] = {0};
    getArg(command, 5, tmpl, args);
    char * out = malloc(100 * sizeof(char));
    strcpy(out, "Здесь должен быть вывод");
    return out;
}

//  Проверка сгенерированной последовательности
char * test(char * command){
    char * filename;
    char * ptr = strstr(command, "inp");
    ptr = strchr(ptr, '=') + 1;
    char * end = strchr(ptr, ' ');
    if (end == NULL){
        end = strchr(ptr, '\0');
    }
    size_t length = end - ptr;
    filename = malloc(length+1);
    strncpy(filename, ptr, length);
    return filename;
}

int main(void){
    char * command = takeCommand();
    printf("%s\n", command);
    char * out = executeCommand(command);
    writeResult(out);

    return 0;
}