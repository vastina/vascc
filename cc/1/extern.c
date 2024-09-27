extern int printf(const char* format, ...);

extern int aaa;
void modify(){ aaa = 114514; }

extern void print_num(int a, int b, int c);
// this is an UB actually

int main(){
  printf("%d\n", aaa);
  modify();
  printf("%d\n", aaa);

  print_num(1, 2, 3);

  return 0;
}