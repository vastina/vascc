int ___abs___ (int a, int b){
    if(a > b) {return a - b;}
    return b - a;
}

int closet (int x, int y, int z, int target){
    int xl = ___abs___(target, x);
    int yl = ___abs___(y, target);
    int zl = ___abs___(z, target);
    if(xl <= yl && xl <= zl){ return x;}
    if(yl <= xl && yl <= zl){ return y;}
    return z;
}

int main (){
    printf("%d\n", closet(2, 4, 8, 16));

    return 0;
}