int main() {
  int input = 114514, time = 0;
  while( true ){
    printf("current : %d\n", input);
    if( input%2 == 0 ){
      input = input / 2;
    }
    
    if( input == 1 ){
      break;
    }
    if( !(input%2 == 0) ){
      input = 3*input + 1;
    }
    time = time+1;
  }

  printf("res : %d\n", time);
  return 0;
}