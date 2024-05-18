int main(){
  int aaa=12;
  {
    aaa = 1;
  }
  {
    int res;
    res = printf("%d\n", aaa);
  }

  return 0;
}