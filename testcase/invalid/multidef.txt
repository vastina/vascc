func fff:int(int a, int b){
    int b = a+2;
    return a+ b;
}

func fff:int(int a, int b=0){
    int b = a+2;
    return a+ b;
}


func main:int(){
    int o = fff(4, 5) - 42;

    return (o-o);
}