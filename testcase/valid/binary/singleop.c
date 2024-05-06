int main(){
    int a = ~0;
    int b = !0;
    int c = -1;
    int d = 4+~4+1;
    int e = 5 -!5;
    int f = 6 + -6;

    return (a+b+c+d+e+f)&0;
}