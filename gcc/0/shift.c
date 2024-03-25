int sr(int a, int b){
    return a >> b;
}

int sl(int a, int b){
    return a << b;
}

/*
gcc -S shift.c
gcc -fdump-tree-gimple shift.c
-fdump-tree-all is too long to show
*/


int main(){return 0;}