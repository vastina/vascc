int fac(int n){
    if(n==1) {return 1;}
    return n*fac(n-1);
}

int main(){
    int a = fac(4) ;
    return 1-(a==24) ;
}