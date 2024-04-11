int addint(int a, int b){
    return a+b;
}

long long addlong(long long a, long long b){
    return a+b;
}

float addfloat(float a, float b){
    return a+b;
}

double adddouble(double a, double b){
    return a+b;
}

int main(){
    int a1 = addint(114514, 1919810);
    long long a2 = addlong(31415926, 217);
    float a3 = addfloat(3.14, 2.17);
    double a4 = adddouble(0.1, 0.2);

    return a1+a2+a3+a4;
}
