extern "C" unsigned factorial(unsigned);

int main(int argc, char **argv) {
  return factorial(3)*7 == 42;
}

/*
This may help: clang --help | grep -w -- -[Sc]
*/
/*
clang++ -S -emit-llvm main.cpp
clang++ -c -emit-llvm main.cpp
*/
/*
llvm-link fac.bc main.bc -o linked.bc
llc --march=x86-64 linked.bc
*/
