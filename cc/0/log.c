#include <stdlib.h>

long logor(){
  long a = rand();
  long b = rand();
  return a || b;
}

long logand(){
  long a = rand();
  long b = rand();
  return a && b;
}

int main(){
  return 0;
}
// use optimize to see non-jmp asm