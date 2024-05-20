

int main(){
  extern int printf(const char* format, ...);
  int y = 150;
  while( y > -150){
    int x = -150;
    while( x < 150){
      int a = x*x + y*y - 100*100;
      int res = a * a * a - x * x * y * y * y;
      if(res <= 0) {
        printf(" ");
      }
      if(res > 0){
        printf("*");
      }
      x = x + 5;
    }
    y = y - 10;
  }

  for ( float y = 1.5f; y > -1.5f; y -= 0.1f ) {
    for ( float x = -1.5f; x < 1.5f; x += 0.05f ) {
      float a = x * x + y * y - 1;
      printf( a * a * a - x * x * y * y * y <= 0.0f ? "*" : " " );
    }
  }
  printf( "\n" );
}