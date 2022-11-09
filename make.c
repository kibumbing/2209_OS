#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
	int str[1000];
	srand(time(NULL));
	for(int i=0; i<1000; i++)
		str[i] = rand()%30+1;

	FILE *fp;
	fp = fopen("page.dat", "w");
	fwrite(str, sizeof(int), 1000, fp);
	fclose(fp);
	return 0;
}
