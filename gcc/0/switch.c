void add_(int a){
    static int num = 0;
    switch (a) {
        case 1:
            num += 2;
            break;
        case 3:
            num = num*num;
            break;
        case 5:
            num = num/2;
            break;
        case 7:
            num = num*2;
            break;
        case 9:
            num = num|2;
            break;
        default:
            num = 0;
            break;
    }
}

void add(int a){
    static int num = 0;
    switch (a) {
        case 1:
            num += 2;
            break;
        case 3:
            num = num*num;
            break;
        case 15:
            num = num/2;
            break;
        case 7:
            num = num*2;
            break;
        case 9:
            num = num|2;
            break;
        case 75:
            num = num&0x7fffffff;
            break;
        case 999:
            num = 999;
            break;
        default:
            num = 0;
            break;
    }
}

int main(){
    add_(1);
    add(1);
    add_(1);

    return 0;
}