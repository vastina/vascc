int sr(int a, int b){
    return a >> b;
}

int sl(int a, int b){
    return a << b;
}

/*
gcc -S shift.c
gcc -fdump-tree-all shift.c
*/


int main(){return 0;}