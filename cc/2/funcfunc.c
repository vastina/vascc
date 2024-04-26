int func1(){
    int func2(){
        return 0;
    }

    return func2();
}

int main(){
    return func1();
}

// so this is a gnu-c extension