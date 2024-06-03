//#include <stdio.h>

int main(){
  // extern int printf(const char*fotmat, ...);
  printf(
"                        _     _                 \n"
" __   __   __ _   ___  | |_  (_)  _ __     __ _ \n"
" \\ \\ / /  / _` | / __| | __| | | | '_ \\   / _` |\n"
"  \\ V /  | (_| | \\__ \\ | |_  | | | | | | | (_| |\n"
"   \\_/    \\__,_| |___/  \\__| |_| |_| |_|  \\__,_|\n"
                                                
  );

  int  i, j, k, l, m;	
	// char c = '*';
	
	printf("\n");printf("\n");printf("\n");printf("\n");
	
	i = 1;
	while(i <= 3){
		j = 1;
		while (j <= 32-2*i){
			printf(" ");
			j = j+1;
		}
		k = 1;
		while (k <= 4*i+1){
			printf("*");
			k = k+1;
		}
		l = 1;
		while (l <= 13-4*i){
			printf(" ");
			l = l+1;
		}
		m = 1;
		while (m <= 4*i+1){
			printf("*");
			m = m+1;
		}
		printf("\n");
		i = i+1;
	}
	// for (i=1; i<=3; i++) {
	// 	for (j=1; j<=32-2*i; j++)		printf(" ");
	// 	for (k=1; k<=4*i+1; k++)		printf("*");
	// 	for (l=1; l<=13-4*i; l++)		printf(" ");
	// 	for (m=1; m<=4*i+1; m++)		printf("*");
	// 	printf("\n");
	// }
	
	i = 1;
	while (i <= 3) {
		j = 1;
		while (j <= 25) {
			printf(" ");
			j = j+1;
		}
		k = 1;
		while (k <= 29) {
			printf("*");
			k = k+1;
		}
		printf("\n");
		i = i+1;
	}
	// for (i=1; i<=3; i++) {
	// 	for (j=1; j<=24+1; j++)			printf(" ");
	// 	for (k=1; k<=29; k++)			printf("*");
	// 	printf("\n");
	// }
	
	i = 7;
	while (i >= 1) {
		j = 1;
		while (j <= 40-2*i) {
			printf(" ");
			j = j+1;
		}
		k = 1;
		while (k <= 4*i-1) {
			printf("*");
			k = k+1;
		}
		printf("\n");
		i = i-1;
	}
	// for (i=7; i>=1; i--) {
	// 	for (j=1; j<=40-2*i; j++)		printf(" ");
	// 	for (k=1; k<=4*i-1; k++)		printf("*");
	// 	printf("\n");
	// }
	
	i = 1;
	while (i <= 39){
		printf(" ");
		i = i+1;
	}
	printf("*\n");
	//for (i=1; i<=39; i++)				printf(" ");
	//printf("*\n");
	
	printf("\n");printf("\n");printf("\n");printf("\n");

  return 0;
  // auth：Kuuo
  // link：https://www.zhihu.com/question/20187195/answer/34478043
}
