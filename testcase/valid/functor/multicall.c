int add(int a, int b){
    return a + b;
}

int fib(int i){
    if(i <= 2) {return 1;}
    return fib(i-1) + fib(i-2);
}

int main (){
    return fib(5) == add(114514, 1919810);

    // well, there is a bug in parsing return value ignored or void call
    fib(11); //todo, fix this
}
