extern int* printf(const char*, ...);

int While(int a){
    while(a>5){
        a -= 5;
    }

    return a;
}

int Loop(int a){

IF:
    if(a>5) goto STMT;
    //else goto DONE;
    else return a;
    {
STMT:
        a -= 5;
        goto IF;
    }
// DONE:
//     return a;
}

int main(){
    int res1 = While(114514);
    int res2 = Loop(1919810);

    printf("While: %d\n", res1);
    printf("Loop: %d\n", res2);

    return 0;
}