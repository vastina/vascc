int func1(){
    int func2(){
        return 0;
    }

    return func2();
}

int main(){
    return func1();
}

// so vcc just happens to work in this case