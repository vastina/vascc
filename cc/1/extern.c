extern int printf(const char* format, ...);

extern int aaa;
void modify(){ aaa = 114514; }

int main(){
  printf("%d\n", aaa);
  modify();
  printf("%d\n", aaa);

  return 0;
}