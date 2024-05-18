int main(){
  int res;
  //extern int printf(const char * format, ...);
  // by the way, asm code support .string "vas\n" "123" too, no cast needed
  res = printf("vas\n" "123") ;
  res = 
  (
    printf("vas\n") 
    && 
    (
      (
        ( 
          printf("vas\n") 
          & 
          printf("vas\n")
        )
        |
        printf("vas\n")
      )
      ||
      (
        printf("vas\n")
        *
        printf("vas\n")
        /
        printf("vas\n")
      )
      *
      ( 
        printf("vas\n") 
        & 
        printf("vas\n")
      )
    )
  )
  -
  (
    printf("vas\n")
    +
    printf("gnu\n")
  )
;

  return printf("use echo$?\n"
    "res : %d\n", res);
}