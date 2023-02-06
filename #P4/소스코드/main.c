#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algorithm.h"

#define BUFSIZE 1024	//입력 버퍼 사이즈
#define PAGELEN 500	//페이지 스트링 길이 

static int page_str[PAGELEN];	//페이지 스트링
static int page_rw[PAGELEN];	//페이지 스트링 RW비트

void menu_c(int sel);	//C.데이터 입력 방식 선택 구현 함수

int main(){
	printf("Page Replacement Algorithm Simulator\n\n");	//프로그램 이름 출력
	
	//A.페이지 교체 알고리즘 선택 메뉴
	printf("A. Page Replacement 알고리즘 시뮬레이터를 선택하시오.(최대 3개)\n");	//메뉴 설명. 최대 3개임을 명시
	printf("(1)Optimal (2)FIFO (3)LIFO (4)LRU (5)LFU (6)SC (7)ESC (8)ALL\n");	//총 8개의 경우의 수
	
	int num_a[4];			//최대 3개의 선택한 숫자를 저장, 배열의 네번째는 4개 이상의 입력이 들어왔는지 확인을 위한 자리
	while(1){			//올바른 알고리즘이 선택될 때까지 반복하여 재입력
		for(int i=0; i<4; i++)	//저장 배열 -1로 초기화
			num_a[i] = -1;
		int a = 0;		//선택한 수를 넣을 배열의 인덱스를 가리키는 변수
		char buf[BUFSIZE];	//입력 버퍼		
		int err = 0;		//에러 여부를 가리키는 변수
		// 선택하는 알고리즘 입력, string형으로 받아 입력 버퍼에 저장
		printf("Input: ");	
		fgets(buf, BUFSIZE, stdin);
		//버퍼에 저장한 입력을 띄어쓰기, 콤마, 탭, 띄어쓰기를 기준으로 구분한 첫번째 입력을 token에 저장.
		char *token;
		token = strtok(buf, " ,\t\n");
		while(token != NULL && a < 4){			//버퍼가 완전히 비워지고, a가 3이하일 때까지 반복
			int temp = atoi(token);			//token 안의 입력을 정수형으로 바꿔 temp에 저장
			// 오류 출력 후 재입력
			// temp = 0 : 0, int형을 변환할 수 없는 값
			// temp = 1 : 1
			// temp < 1 or temp > 7 : 1~8 사이가 아닌 값
			if(temp == 0 || temp == -1 || (temp < 1 || temp > 8)){
				printf("Wrong Input. 1~7 사이의 숫자를 선택하시오.\n");
				err = 1;	//에러가 일어남을 저장
				break;		//재입력을 받기 위해 while문 break
			}
			num_a[a++] = temp;			//에러가 나지 않았을 경우 num_a에 선택된 숫자를 저장하고, 선택된 수를 넣을 인덱스를 하나 올림
			token = strtok(NULL, " ,\t\n");	//버퍼의 다음 입력을 구분하여 token에 저장
		}
		//에러 여부 검사, 에러가 났으면 재입력
		if(err==1)
			continue;
		//8이 다른 수와 함께 입력 됐을 경우 재입력
		//첫 번째가 8이고 두 번째에 입력이 저장됐을 경우
		//두 번째, 세 번재가 8인 경우
		if((num_a[0]==8 && num_a[1]!=-1) || num_a[1]==8 || num_a[2]==8){
			printf("Wrong Input. 8은 다른 숫자와 선택할 수 없습니다.\n");
			continue;
		}
		//입력이 없는 경우 재입력
		if(num_a[0] == -1){
			printf("Wrong Input. 최소 한 개의 숫자를 선택하시오.\n");
			continue;
		}
		//3개 이상 입력한 경우 재입력, 3개 이상 입력되었으면 배열의 네 번째의 값이 바뀜
		else if(num_a[3] != -1){
			printf("Wrong Input. 최대 세 개의 숫자를 선택하시오.\n");
			continue;
		}
		break;
	}
	
	//B.페이지 프레임 개수 입력
	int b, num_b = 0;	//b:scanf의 리턴값 저장, num_b:scanf로 정수를 입력받음
	printf("\nB. 페이지 프레임의 개수를 입력하시오.(3~10)\n");	//메뉴 설명, 3부터 10사이의 값이 들어와야 함을 명시
	while(1){	//올바른 프레임 개수가 선택될 때까지 반복하여 재입력
		//num_b에 입력받은 정보를 저장. scanf는 입력이 원하는 방식(정수)이면 1을 리턴
		printf("Input: ");
		b = scanf("%d", &num_b);
		while(getchar()!='\n');	//입력 버퍼 비움
		if(b == 1){	//입력받은 정보가 정수이면 실행
			if(num_b>= 3 && num_b<=10)	//입력받은 정수가 3이상 10이하이면 while문 탈출
				break;
			else				//입력받은 정수가 3미만 10초과이면 에러 출력 후 재입력
				printf("Wrong Input. 3~10 사이의 숫자를 선택하시오.\n");
		}		//입력받은 정보가 정수가 아니면 에러 출력 후 재입력
		else
			printf("Wrong Input. 3~10 사이의 숫자를 선택하시오.\n");
	}
	
	//C.데이터 입력 방식 선택
	int c, num_c = 0;	//c:scanf의 리턴값 저장, num_c:scanf로 정수를 입력받음
	printf("\nC. 데이터의 입력 방식을 선택하시오.(1,2)\n");	//메뉴 설명, 1또는 2가 입력되어야 함을 명시
	printf("(1)랜덤하게 생성\n(2)사용자 생성 파일 오픈\n");
	while(1){	//올바른 데이터 입력 방식이 선택될 때까지 반복하여 재입력
		//num_c에 입력받은 정보를 저장. scanf는 입력이 원하는 방식(정수)이면 1을 리턴
		printf("Input: ");
		c = scanf("%d", &num_c);
		while(getchar()!='\n');	//입력 버퍼 비움
		if(c == 1){	//입력받은 정보가 정수이면 실행
			if(num_c==1 || num_c==2)	//입력받은 정수가 1또는 2이면 while문 탈출
				break;
			else				//입력받은 정수가 1또는 2가 아니면 에러 출력 후 재입력
				printf("Wrong Input. 1 또는 2를 선택하시오. \n");
		}
		else	//입력받은 정보가 정수가 아니면 에러 출력 후 재입력
			printf("Wrong Input. Enter 1 또는 2를 선택하시오.\n");
	}
	
	menu_c(num_c);		//참조 페이지 스트링 생성, 선택된 입력 방식으로 데이터 생성 혹은 파일 오픈
	
	//시뮬레이션 결과를 저장할 파일 open
	FILE *fp;
	fp = fopen("save.txt", "w");
	
	for(int i=0; i<3; i++){	//num_a를 인덱스 0~2를 돌며 선택한 알고리즘 실행
		if(num_a[i]==-1)				//-1: 더 이상 선택된 알고리즘이 없음
			break;
		else if(num_a[i]==1)				//1: Optimal 알고리즘 수행
			opt(num_b, page_str, fp);
		else if(num_a[i]==2)				//2: FIFO 알고리즘 수행
			fifo(num_b, page_str, fp);
		else if(num_a[i]==3)				//3: LIFO 알고리즘 수행
			lifo(num_b, page_str, fp);
		else if(num_a[i]==4)				//4: LRU 알고리즘 수행
			lru(num_b, page_str, fp);
		else if(num_a[i]==5)				//5: LFU 알고리즘 수행
			lfu(num_b, page_str, fp);
		else if(num_a[i]==6)				//6: SC 알고리즘 수행
			sc(num_b, page_str, fp);
		else if(num_a[i]==7)				//7: ESC 알고리즘 수행
			esc(num_b, page_str, page_rw, fp);
		else if(num_a[i]==8){				//모든 알고리즘 수행
			opt(num_b, page_str, fp);
			fifo(num_b, page_str, fp);
			lifo(num_b, page_str, fp);
			lru(num_b, page_str, fp);
			lfu(num_b, page_str, fp);
			sc(num_b, page_str, fp);
			esc(num_b, page_str, page_rw, fp);
		}
	}
	fclose(fp);	//시뮬레이션 결과를 저장한 파일 close
	
	printf("D. 종료.\n");	//종료
	return 0;
}

//C. 데이터 입력 방식 구현
void menu_c(int sel){
	//랜덤 생성 선택시
	if(sel == 1){
		printf("Ramdomly generate.\n");	//선택한 입력 방식 출력
		srand(time(NULL));			//난수 생성
		for(int i=0; i<PAGELEN; i++){		//페이지 스트링 길이만큼 저장
			page_str[i] = rand()%30+1;	//1~30 정수 랜덤하게 저장
			page_rw[i] = rand()%2;		//0,1 램덤하게 저장, 0:R, 1:W
		}
	}
	//사용자 생성 파일 오픈 선택시
	else if(sel == 2){
		FILE *fp;		//생성 파일 포인터
		char file_name[32];	//오픈할 파일 이름 변수
		
		printf("Open file. Input file name.");	//선택한 입력 방식 출력
		do{
			//파일 이름 입력
			printf("\nInput: ");
			scanf("%s", file_name);
		}while((fp = fopen(file_name, "r")) == NULL);	//입력받은 파일이 정상적으로 오픈될 때까지 반복하여 파일 이름 재입력, open
		
		printf("\nOpen %s\n\n", file_name);	//입력한 파일 오픈 메시지
		int temp;	//입력 스트링 번호 버퍼
		char temp_2;	//입력 스트링 RW 버퍼
		for(int i=0; i<PAGELEN; i++){		//입력 스트링 개수만큼 저장
			fscanf(fp, "%d(%c) ", &temp, &temp_2);	//입력 스트링 번호와 RW여부를 read하여 버퍼에 저장
			page_str[i] = temp;				//버퍼의 입력 스트링 번호 저장
			if(temp_2=='r')				//버퍼가 r을 가리키면 RW비트에 0 저장
				page_rw[i]=0;
			else						//버퍼가 w를 가리키면 RW비트에 1 저장
				page_rw[i]=1;	
		}
		fclose(fp);	//생성 파일 close
		
	}
}
