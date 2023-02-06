#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PAGELEN 500

int main(){
	//페이지스트링 생성
	int str[PAGELEN];
	srand(time(NULL));					//난수 설정
	for(int i=0; i<PAGELEN; i++)
		str[i] = rand()%30+1;			//1~30 랜덤으로 페이지스트링 번호 설정
		
	char st[PAGELEN][10];		
	for(int i=0; i<PAGELEN; i++)
		sprintf(st[i], "%d", str[i]);	//페이지스트링 번호 스트링 형태로 변환
	
	//프레임스트링 RW
	int rw[PAGELEN];
	for(int i=0; i<PAGELEN; i++)
		rw[i] = rand()%2;				//0 or 1을 랜덤으로 설정. 0:Read 1:Write
	
	
	FILE *fp;							//페이지스트링을 저장할 파일 포인터
	fp = fopen("string.txt", "w");		//string.txt.로 새로운 파일 open
	for(int i=0; i<PAGELEN; i++){
		//페이지스트링 번호 쓰기
		fputs(st[i], fp);
		//페이지스트링 RW 쓰기
		fputs("(", fp);
		if(rw[i]==0)
			fputs("r", fp);
		else
			fputs("w", fp);
		fputs(")", fp);
		fputs(" ", fp);
	}
	fclose(fp);							//파일 close
	return 0;
}
