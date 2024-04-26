extern int printf(const char*, ...);

#define print_f(...) printf(__VA_ARGS__)

int main(){
    return print_f("vastina");
}