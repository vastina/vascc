int main(){
  int res;
  // by the way, asm code support .string "vas\n" "123" too, no cast needed
  res = printf("vas\n" "123") + printf("gnu\n");

  return 0;
}