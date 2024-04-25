extern int printf(const char*, ...);

int main(){
    int a = 0xf1145140;
    printf("%d\n", a>>31);

    unsigned b = (unsigned)a;
    printf("%u\n", b>>31);

    return 0;
}