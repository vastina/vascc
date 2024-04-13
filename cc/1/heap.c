#include <stdlib.h>

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
    return count;
}