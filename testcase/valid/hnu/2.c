int g(int x){
  if (x <= 0 ) { return 0; }
  if (x == 1 || x == 2) { return 1; }
  return 2*g(x-2) + 3*g(x-3);
  // it was x+3 in the exam, but the program won't end and cause seg fault(stack overflow) when x+3
}

int f(int a, int b) {
  if(0==a) {return 0;}
  return g(b*b);
}

int main(){
  printf("%d\n", g(30));
  printf("%d\n", f(1, 1));
  return 0;
}

// a fucking stupid piece of code from hnu CS funal exam. hhha