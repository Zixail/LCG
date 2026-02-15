#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ULL unsigned long long

#define INPUT "input.txt"
#define OUTPUT "output.txt"

void get_c(char *);
void get_a(char *);
void lcg(char *);
void floid(char *);
void test(char *);

//  Чтение комманды из  файла INPUT
char * takeCommand(){
    FILE * fp = fopen(INPUT, "r");
    size_t size = 16;
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
void executeCommand(char * command){
    char * comms [5] = {"get_c", "get_a", "lcg", "test", "floid"};
    void (*func[5])(char *) = {get_c, get_a, lcg, test, floid};

    char * sep = strchr(command, ' ');
    size_t length = sep - command;

    for (int i = 0; i < 5; ++i){
        if (strncmp(command, comms[i], length) == 0){
            func[i](command);
            return;
        }
    }
    FILE * fp = fopen(OUTPUT, "w");
    fprintf(fp, "incorrect command\n");
    fclose(fp);
}

//  Нахождение всех простых делителей числа
ULL * getPrime(ULL m, int* length){
    int counts = 0;
    int size = 8;
    ULL* prime = calloc(size, sizeof(ULL));

    ULL tmp = m;
    for(ULL i = 2; i <= tmp; ++i){
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

//  Нахождение всех взаимно простых числа
void getRelative(ULL *prime, int length, ULL cmin, ULL cmax, ULL m){
    FILE* fp = fopen(OUTPUT, "w");
    if (cmax >= m) cmax = m;
    for(ULL i = cmin; i < cmax; ++i){
        char flag = 1;
        for(int j = 0; j < length; ++j){
            if (i % prime[j] == 0){
                flag = 0;
                break;
            }
        }
        if (flag == 1){
            fprintf(fp, "%llu\n", i);
        }
    }
    fclose(fp);
}

//  Функция для парсинга аргументов по заданному шаблону
int getArg(char * command, int length, char * tmpl[], ULL args[]){

    for(int i = 0; i < length; ++i){
        char * ptr = strstr(command, tmpl[i]);
        if (ptr == NULL) return 1;
        ptr = strchr(ptr, '=') + 1;
        char * end;
        args[i] = strtoull(ptr, &end, 10);
    }
    return 0;
}

//  Подбор всех взаимно простых
void get_c(char * command){
    char * tmpl[] = {" cmin", " cmax", " m"};
    ULL args[3] = {0};
    if (getArg(command, 3, tmpl, args)){
        FILE* fp = fopen(OUTPUT, "w");
        fprintf(fp, "incorrect command");
        fclose(fp);
        return;
    }

    ULL cmin = args[0];
    ULL cmax = args[1];
    ULL m    = args[2];

    int length;
    ULL * prime = getPrime(m, &length);

    getRelative(prime, length, cmin, cmax, m);

    free(prime);
}

//  Подбор минимального делящегося на все простые делители
void get_a(char * command)
{   
    char * tmpl[] = {" m"};
    ULL args[1] = {0};
    if (getArg(command, 1, tmpl, args)){
        FILE* fp = fopen(OUTPUT, "w");
        fprintf(fp, "incorrect command");
        fclose(fp);
        return;
    }
    ULL m = args[0];

    int len = 0;
    ULL a = 1;
    ULL *primes = getPrime(m, &len);
    for (int i = 0; i < len; i++) {
        a *= primes[i];
    }
    a += 1;
    
    FILE *fp = fopen(OUTPUT, "w");
    if (a == 2){
        fprintf(fp, "no solution");
    }
    else {
        fprintf(fp, "%llu\n", a);
    }
    fclose(fp);
    free(primes);
}

//  Генерации числа по прошлому
ULL nextNumber(ULL a, ULL x0, ULL c, ULL m){
    ULL x = (a * x0 + c) % m;
    return x;
}

//  Генерация чисел 
void lcg(char * command){
    char * tmpl[] = {" a", " x0", " c", " m", " n"};
    ULL args[5] = {0};
    if (getArg(command, 5, tmpl, args)){
        FILE* fp = fopen(OUTPUT, "w");
        fprintf(fp, "incorrect command");
        fclose(fp);
        return;
    }

    ULL a = args[0];
    ULL x0 = args[1];
    ULL c = args[2];
    ULL m = args[3];
    ULL n = args[4];

    if (n == 0 || a >= m || c >= m || x0 >= m){
        FILE *fp = fopen(OUTPUT, "w");
        fprintf(fp, "no solution");
        fclose(fp);
        return;
    }

    ULL x = x0;
    FILE* fp = fopen(OUTPUT, "w");

    for(ULL i = 0; i < n; ++i){
        x = nextNumber(a, x, c, m);
        fprintf(fp, "%llu\n", x);
    }

    fclose(fp);
    
}

//  Определение периода  ЛКГ алгоритмом Флойда
void floid(char * command){
    char * tmpl[] = {" a", " x0", " c", " m"};
    ULL args[4] = {0};
    if (getArg(command, 4, tmpl, args)){
        FILE* fp = fopen(OUTPUT, "w");
        fprintf(fp, "incorrect command");
        fclose(fp);
        return;
    }

    ULL a = args[0];
    ULL x0 = args[1];
    ULL c = args[2];
    ULL m = args[3];

    ULL turtle = x0;
    ULL hare = x0;

    do{
        turtle = nextNumber(a, turtle, c, m);
        hare = nextNumber(a, hare, c, m);
        hare = nextNumber(a, hare, c, m);
    } while (turtle != hare);

    ULL mu = 0;
    turtle = x0;
    while(turtle != hare){
        turtle = nextNumber(a, turtle, c, m);
        hare = nextNumber(a, hare, c, m);
        ++mu;
    }

    ULL lam = 0;
    do{
        hare = nextNumber(a, hare, c, m);
        ++lam;
    } while (hare != turtle);

    FILE* fp = fopen(OUTPUT, "w");
    fprintf(fp, "mu = %llu\n", mu);
    fprintf(fp, "lam = %llu\n", lam);
    fclose(fp);
}

//  Проверка сгенерированной последовательности
void test(char * command){
    char * filename;
    char * ptr = strstr(command, " inp");
    if (ptr == NULL){
        FILE* fp = fopen(OUTPUT, "w");
        fprintf(fp, "incorrect command");
        fclose(fp);
        return;
    }
    ptr = strchr(ptr, '=') + 1;
    while(*ptr == ' '){
        ptr++;
    }
    char * end = strchr(ptr, ' ');
    if (end == NULL){
        end = strchr(ptr, '\0');
    }
    size_t length = end - ptr;
    filename = calloc(length+1, sizeof(char));
    strncpy(filename, ptr, length);

    int size = 8;
    int count = 0;
    ULL *digits = calloc(size, sizeof(ULL));
    ULL cur_num = 0;
    FILE* fp = fopen(filename, "r");
    while (fscanf(fp, "%llu", &cur_num) == 1 && count < 100000) {
        if (size == count) {
            size *= 2;
            ULL *tmp = realloc(digits, size * sizeof(ULL));
            digits = tmp;
        }
        digits[count] = cur_num;
        count++;
    }

    ULL median = digits[count/2];
    int count_zeros = 0;
    int count_ones = 0;
    int R;
    long double expected_value;
    long double Z;
    for (int i = 0; i < count; i++) {
        if (digits[i] < median) {
            digits[i] = 0;
            count_zeros += 1;
        }
        else {
            digits[i] = 1;
            count_ones += 1;
        }

        expected_value = (double)(2*count_ones*count_zeros) / (double)count + 1;
        long double variance_square = (double)(2*count_ones*count_zeros*(2*count_ones*count_zeros - count)) /
                                      (double)((count*count)*(count - 1));

        R = 0;
        for (int j = 1; j < count; j++) {
            if (digits[j] != digits[j-1]) R += 1;
        }

        Z = (R - expected_value) / sqrtl(variance_square);
    }
    fclose(fp);
    FILE* fw = fopen(OUTPUT, "w");
    fprintf(fw, "Наблюдаемое число серий: %d\n", R);
    fprintf(fw, "Ожидаемое число серий: %Lf\n", expected_value);
    fprintf(fw, "Z: %Lf\n", Z);
    if (fabs(Z) < 2) fprintf(fw, "Последовательность случайна");
    if (fabs(Z) >= 2 && fabs(Z) < 3) fprintf(fw, "Последовательность может быть не случайна, стоит проверить точнее");
    if (fabs(Z) >= 3) fprintf(fw, "Последовательность почти наверняка не случайна");

    free(digits);
    fclose(fw);
}


int main(void){
    char * command = takeCommand();
    executeCommand(command);
    free(command);

    return 0;
}