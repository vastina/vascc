#include <stdlib.h>
extern int printf(const char*, ...);

int main(){
    int *p = (int*)malloc(sizeof(int)*10);
    int index = 0, count = 0;
    while(index < 10){
        p[index++] = 20 - index;
    }
    while(index > 0){
        count += p[--index];
    }
    free(p);
    printf("%d\n", count);
    return 0;
}