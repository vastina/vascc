int main(){
    int a = ~0;
    int b = !0;
    int c = -1;
    int d = 4+~4+1;
    int d2 = 4+(~4)+1;
    int e = 5 -!5;
    int e2 = 5 - (!5);
    int f = 7 + -6;
    int f2 = 7 + (-6);
    int g = 6 - +7;
    int g2 = 6 - (+7);
    int h = ~2 +2;
    int h2 = (~2) + 2;
    int i = -1 - !1 + ~4 * +5 - -1;
    int i2 = (-1) - (!1) + (~4) * (+5) - (-1);

    return (a+b+c+d+e+f+g+h+i+d2+e2+f2+h2+g2+i2)&0;
}