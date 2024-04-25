int divint(int a, int b){
    return a/b;
}

long long divlong(long long a, long long b){
    return a/b;
}

float divfloat(float a, float b){
    return a/b;
}

double divdouble(double a, double b){
    return a/b;
}

int main(){
    int a1 = divint(114514, 1919810) /90;
    long long a2 = divlong(31415926, 217);
    float a3 = divfloat(3.14, 2.17);
    double a4 = divdouble(0.1, 0.2);

    return (a1+a2)-(a3+a4);
}
