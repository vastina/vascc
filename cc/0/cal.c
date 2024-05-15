#include <stdlib.h>

long g(){
    long a = rand();
    long b = rand();
    return a > b;
}

long ge(){
    long a = rand();
    long b = rand();
    return a >= b;
}

long l(){
    long a = rand();
    long b = rand();
    return a < b;
}

long le(){
    long a = rand();
    long b = rand();
    return a <= b;
}