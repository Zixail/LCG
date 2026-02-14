#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ULL unsigned long long

#define BUFF 16
#define INPUT "input.txt"
#define OUTPUT "output.txt"

void get_c(char *);
void get_a(char *);
void lcg(char *);
void test(char *);

//  Чтение комманды из  файла INPUT
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

//  Печать в файл OUTPUT 
void writeResult(char * result){
    FILE * fp = fopen(OUTPUT, "w");
    fputs(result, fp);
}

//  Функция определения прочитанной комманды и её запуска
void executeCommand(char * command){
    char * comms [4] = {"get_c", "get_a", "lcg", "test"};
    void (*func[4])(char *) = {get_c, get_a, lcg, test};

    char * sep = strchr(command, ' ');
    size_t length = sep - command;

    for (int i = 0; i < 4; ++i){
        if (strncmp(command, comms[i], length) == 0){
            func[i](command);
        }
    }
}

ULL * getPrime(ULL m, int* length){
    int counts = 0;
    int size = 8;
    ULL* prime = calloc(size, sizeof(ULL));

    ULL tmp = m;
    for(ULL i = 2; i < tmp; ++i){
        if (tmp % i == 0){
            tmp /= i;
            if (counts == size){
                size *= 2;
                prime = realloc(prime, size * sizeof(ULL));
            }
            prime[counts++] = i;
            while(tmp % i == 0) tmp /= i;
        }
    }
    *length = counts;
    return prime;
}

ULL * getRelative(ULL *prime, int length, ULL cmin, ULL cmax, ULL m, int* length2){
    int counts = 0;
    int size = 8;
    ULL* relative = calloc(size, sizeof(ULL));
    if (cmax >= m) cmax = m - 1;
    for(ULL i = 2; i < cmax; ++i){
        char flag = 1;
        for(int j = 0; j < length; ++i){
            if (i % prime[j] == 0){
                flag = 0;
                break;
            }
            if (flag == 1){
                if (counts == size){
                    size *= 2;
                    relative = realloc(relative, size * sizeof(ULL));
                }
                relative[counts++] = i;
            }
        }
    }
    *length2 = counts;
    return relative;
}

//  Функция для парсинга аргументов по заданному шаблону
void getArg(char * command, int length, char * tmpl[], ULL args[]){

    for(int i = 0; i < length; ++i){
        char * ptr = strstr(command, tmpl[i]);
        ptr = strchr(ptr, '=') + 1;
        char * end;
        args[i] = strtoull(ptr, &end, 10);
    }
}

//  Подбор всех взаимно простых
void get_c(char * command){
    char * tmpl[] = {" cmin", " cmax", " m"};
    ULL args[3] = {0};
    getArg(command, 3, tmpl, args);

    ULL cmin = args[0];
    ULL cmax = args[1];
    ULL m    = args[2];

    int length;
    ULL * prime = getPrime(m, &length);

    int length2;
    ULL * relative = getRelative(prime, length, cmin, cmax, m, &length2);

    FILE* fp = fopen("output.txt", "w");
    for(int i = 0; i < length2; ++i){
        fprintf(fp, "%llu\n", relative[i]);
    }

}

//  Подбор минимального делящего на все простые делители
void get_a(char * command){
    char * tmpl[] = {" m"};
    ULL args[1] = {0};
    getArg(command, 1, tmpl, args);

    // Блок кода алгоритма

}

//  Генерация чисел 
void lcg(char * command){
    char * tmpl[] = {" a", " x0", " c", " m", " n"};
    ULL args[5] = {0};
    getArg(command, 5, tmpl, args);

    // Блок кода алгоритма
    
}

//  Проверка сгенерированной последовательности
void test(char * command){
    char * filename;
    char * ptr = strstr(command, " inp");
    ptr = strchr(ptr, '=') + 1;
    char * end = strchr(ptr, ' ');
    if (end == NULL){
        end = strchr(ptr, '\0');
    }
    size_t length = end - ptr;
    filename = malloc(length+1);
    strncpy(filename, ptr, length);

    // Блок кода алгоритма

}

int main(void){
    char * command = takeCommand();
    executeCommand(command);

    return 0;
}