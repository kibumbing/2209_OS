#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFSIZE 1024
#define PAGELEN 50

void lru(int num, int *page_str){

	int fault = 0;
	int **arr;
	arr = (int**)calloc(num, sizeof(int*));
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(2, sizeof(int));

	int time = -1;

	for(int k=0; k<PAGELEN; k++){
		time++;
		int fault_print = 0;
		int exist = 0;
		for(int i=0; i<num; i++){
			if(arr[i][0]==page_str[k]){
				arr[i][1] = time;
				exist = 1;
				break;
			}
		}

		if(exist == 0){
			int done = 0;
			for(int i=0; i<num; i++){
				if(arr[i][0]==0){
					arr[i][0] = page_str[k];
					arr[i][1] = time;
					done = 1;
					break;
				}
			}

			if(done == 0){
				int idx = 0;
				for(int i=1; i< num; i++){
					if(arr[idx][1] > arr[i][1])
						idx = i;
				}
				for(int j=idx; j<num-1; j++){
					arr[j][0]=arr[j+1][0];
					arr[j][1]=arr[j+1][1];
				}
				arr[num-1][0] = page_str[k];
				arr[num-1][1] = time;
				}
			fault++;
			fault_print = 1;
		}

		printf("|");
		for(int i=0; i<num; i++){
			printf("	%d	|", arr[i][0]);
		}
		if(fault_print == 1)
			printf("F");
		printf("\n");
	}
	printf("Page Fault: %d\n", fault);

	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
}

void opt(int num, int *page_str){

	int fault = 0;
	int **arr;
	arr = (int**)calloc(num, sizeof(int*));
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(2, sizeof(int));

	for(int k=0; k<PAGELEN; k++){
		int fault_print = 0;
		int exist = 0;
		for(int i=0; i<num; i++){
			if(arr[i][0]==page_str[k]){
				exist = 1;
				break;
			}
			arr[i][1] = 0;
		}

		if(exist == 0){
			int done = 0;
			for(int i=0; i<num; i++){
				if(arr[i][0]==0){
					arr[i][0] = page_str[k];
					done = 1;
					break;
				}
			}

			if(done == 0){
				int count = 1;
				for(int i=k; i<PAGELEN; i++){
					if(count <= num){
						for(int j=0; j<num; j++){
							if(arr[j][0] == page_str[i] && arr[j][1] == 0)
								arr[j][1] = count++;
						}	
					}
					else
						break;
				}
				for(int i=0; i<num; i++){
					if(arr[i][1] == 0 || arr[i][1]>=num){
						for(int j=i; j<num-1; j++)
							arr[j][0]=arr[j+1][0];
						arr[num-1][0] = page_str[k];
						break;
					}
				}
			}
			fault++;
			fault_print = 1;
		}

		printf("|");
		for(int i=0; i<num; i++){
			printf("	%d	|", arr[i][0]);
		}
		if(fault_print == 1)
			printf("F");
		printf("\n");
	}
	printf("Page Fault: %d\n", fault);

	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
}

void fifo(int num, int *page_str){

	int fault = 0;
	int *arr;
	arr = (int*)calloc(num, sizeof(int));

	for(int k=0; k<PAGELEN; k++){
		int fault_print = 0;
		int exist = 0;
		for(int i=0; i<num; i++){
			if(arr[i]==page_str[k]){
				exist = 1;
				break;
			}
		}

		if(exist == 0){
			int done = 0;
			for(int i=0; i<num; i++){
				if(arr[i]==0){
					arr[i] = page_str[k];
					done = 1;
					break;
				}
			}

			if(done == 0){
				for(int i=0; i<num-1; i++)
					arr[i] = arr[i+1];
				arr[num-1] = page_str[k];
			}
			fault++;
			fault_print = 1;
		}

		printf("|");
		for(int i=0; i<num; i++){
			printf("	%d	|", arr[i]);
		}
		if(fault_print == 1)
			printf("F");
		printf("\n");
	}
	printf("Page Fault: %d\n", fault);

	free(arr);
}

void lifo(int num, int *page_str){
	int fault = 0;
	int *arr;
	arr = (int*)calloc(num, sizeof(int));

	for(int k=0; k<PAGELEN; k++){
		int fault_print = 0;
		int exist = 0;
		for(int i=0; i<num; i++){
			if(arr[i]==page_str[k]){
				exist = 1;
				break;
			}
		}

		if(exist == 0){
			int done = 0;
			for(int i=0; i<num; i++){
				if(arr[i]==0){
					arr[i] = page_str[k];
					done = 1;
					break;
				}
			}

			if(done == 0){
				arr[num-1] = page_str[k];
			}
			fault++;
			fault_print = 1;
		}

		printf("|");
		for(int i=0; i<num; i++){
			printf("	%d	|", arr[i]);
		}
		if(fault_print == 1)
			printf("F");
		printf("\n");
	}
	printf("Page Fault: %d\n", fault);

	free(arr);
}


int *menu_c(int sel){
	static int page_str[PAGELEN];
	if(sel == 1){
		printf("Ramdomly generate.\n");
		srand(time(NULL));
		for(int i=0; i<PAGELEN; i++)
			page_str[i] = rand()%30+1;
	}
	else if(sel == 2){
		printf("Open page.dat\n");
		FILE *fp;
		fp = fopen("page.dat", "r");
		for(int i=0; i<PAGELEN; i++){
			fread(&page_str[i], sizeof(int), 1, fp);
		}
		fclose(fp);
	}
	
	return page_str;
}

int main(){
	int a = 0;
	int num_a[4] = {-1,-1, -1, -1};
	char buf[BUFSIZE];
	char *token;
	printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오.(최대 3개)\n");
	printf("(1)Optimal (2)FIFO (3)LIFO (4)LRU (5)LFU (6)SC (7)ESC (8)ALL\n");
	
	while(1){
		int err = 0;
		printf("Input: ");
		fgets(buf, BUFSIZE, stdin);
		token = strtok(buf, " ,\t\n");
		while(token != NULL && a < 4){
			int temp = atoi(token);
			// 오류 출력 후 재입력
			// temp = 0 : 0, int형을 변환할 수 없는 값
			// temp = 1 : 1
			// temp < 1 or temp > 8 : 1~8 사이가 아닌 값
			if(temp == 0 || temp == -1 || (temp < 1 || temp > 8)){
				printf("Wrong Input. 1~8 사이의 숫자를 선택하시오.\n");
				err = 1;
				break;
			}
			num_a[a++] = atoi(token);
			token = strtok(NULL, " ,\t\n");
		}
		if(err==1)
			continue;
		// 입력이 없는 경우 재입력
		if(num_a[0] == -1){
			printf("Wrong Input. 최소 한 개의 숫자를 선택하시오.\n");
			continue;
		}
		// 3개 이상 입력한 경우 재입력
		else if(num_a[3] != -1){
			printf("Wrong Input. 최대 세 개의 숫자를 선택하시오.\n");
			continue;
		}
		printf("%d %d %d\n", num_a[0], num_a[1], num_a[2]);
		break;
	}

	int b, num_b = 0;
	printf("\nB. 페이지 프레임의 개수를 입력하시오.(3~10)\n");
	while(1){
		printf("Input: ");
		b = scanf("%d", &num_b);
		while(getchar()!='\n');
		if(b == 1){
			if(num_b>= 3 && num_b<=10)
				break;
			else
				printf("Wrong Input. 3~10 사이의 숫자를 선택하시오.\n");
		}
		else
			printf("Wrong Input. 3~10 사이의 숫자를 선택하시오.\n");
	}

	int c, num_c = 0;
	printf("\nC. 데이터의 입력 방식을 선택하시오.(1,2)\n");
	printf("(1)랜덤하게 생성\n(2)사용자 생성 파일 오픈\n");
	while(1){
		printf("Input: ");
		c = scanf("%d", &num_c);
		while(getchar()!='\n');
			if(c == 1){
				if(num_c==1 || num_c==2)
					break;
				else
					printf("Wrong Input. 1 또는 2를 선택하시오. \n");
			}
			else
				printf("Wrong Input. Enter 1 또는 2를 선택하시오.\n");
	}

	int *page_str = menu_c(num_c);
	for(int i=0; i<PAGELEN; i++){
		printf("%d ", page_str[i]);
	}
	printf("\n");

	//opt(num_b, page_str);
	//fifo(num_b, page_str);
	//lifo(num_b, page_str);
	lru(num_b, page_str);

	printf("\nD. 종료.\n");
	return 0;
}
