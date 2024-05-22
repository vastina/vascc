extern int scanf(const char* format, ...);
extern int printf(const char* format, ...);

int main(){
  int aaa = 0;
  scanf("%d", &aaa);
  printf("%d\n", aaa);

  return 0;
}