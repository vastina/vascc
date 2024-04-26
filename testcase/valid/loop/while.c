int main (){
    int a = 114514, count = false;
    while(a && true){
        a = a&(a-1);
        count = count +1;
    }

    return count;
}