#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"

//페이지 스트링 길이
#define PAGELEN 500

//Optimal 함수 정의
void opt(int num, int *page_str, FILE* fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, fp:출력 저장 파일 포인터
	printf("Optimal\n");		//Optimal 메시지 출력	
	fprintf(fp, "Optimal\n");	//Optimal 메시지 저장
	
	int fault = 0;			//fault 수 저장 0:페이지폴트 1:히트
	int **arr = (int**)calloc(num, sizeof(int*));			//페이지프레임, num만큼 동적으로 생성, 모든 변수 0으로 초기화
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(2, sizeof(int));		//2차원 배열로 구현하여 arr[i][0]에 번호, arr[i][1]에 우선순위 저장
	int *arr_queue = (int*)calloc(num, sizeof(int));		//두 번째 정렬 기준으로 FIFO 사용을 위한 큐

	//전체 페이지스트링 검사
	for(int k=0; k<PAGELEN; k++){
		int exist = 0;		//fault여부 체크
		for(int i=0; i<num; i++){	//페이지프레임 검사
			if(arr[i][0]==page_str[k]){	//페이지프레임에 검사하는 스트링 번호가 존재할 경우
				exist = 1;		//fault가 나지 않음
				break;			//검사 종료
			}
			arr[i][1] = 0;			//존재하지 않을 경우 모든 우선순위 초기화
		}

		if(exist == 0){	//fault가 날 경우
			int done = 0;	//페이지프레임의 빈 자리 여부
			for(int i=0; i<num; i++){	//페이지프레임검사
				if(arr[i][0]==0){	//빈 자리가 있을 경우
					arr[i][0] = page_str[k];	//빈 자리를 해당 스트링으로 교체
					arr_queue[i] = page_str[k];	//큐에 스트링 저장하여 순서 기억
					done = 1;			//빈자리가 있다는 것을 체크
					break;				//검사 종료
				}
			}

			if(done == 0){	//fault가 나고 빈자리가 없을 경우
				int count = 1;	//각 프레임의 번호에 해당하는 미래의 페이지 스트링을 찾으면 1씩 증가
				for(int i=k; i<PAGELEN; i++){	//미래의 페이지스트링 검사
					if(count <= num){	//모든 프레임의 번호을 찾을 때까지 탐색
						for(int j=0; j<num; j++){	//모든 페이지프레임 검사
							if(arr[j][0] == page_str[i] && arr[j][1] == 0)	//현재 페이지프레임과 번호가 같고, 현재 페이지 프레임을 아직 찾은 적이 없는 경우
								arr[j][1] = count++;				//해당 프레임에 우선순위 저장
						}	
					}
					else			//모든 프레임을 찾았을 경우 종료
						break;
				}	//모든 페이지스트링을 검사
				for(int i=0; i<num; i++){	//큐의 index
					int done_2 = 0;	//교체 여부 확인
					for(int j=0; j<num; j++){	//모든 프레임 우선순위 저장
						//큐에 저장된 FIFO를 지키며 탐색, 먼저 들어온 프레임을 먼저 검사하기 때문에, 같은 우선순위(미래에 존재하지 않는) 프레임이 2개 이상일 경우 FIFO에 따라 처리
						if(arr_queue[i]==arr[j][0] && (arr[j][1]==0 || arr[j][1]>=num)){	//미래에서 번호를 찾지 못했거나, 가장 마지막에 나오는 경우
							arr[j][0] = page_str[k];		//해당 프레임 교체
							//큐 업데이트
							for(int l=i; l<num-1; l++)
								arr_queue[l] = arr_queue[l+1];
							arr_queue[num-1] = page_str[k];
							done_2 = 1;	//교체 확인
							break;		//교체가 이루어졌으면 검사 종료
						}
					}
					if(done_2==1)	//교체가 이루어졌으면 검사 종료
						break;
				}
			}
			fault++;	//페이지 폴트 수 1 증가
		}
		printf("%d:	", page_str[k]);		//검사할 페이지스트링 번호 출력
		fprintf(fp, "%d:	", page_str[k]);	//검사할 페이지스트링 번호 저장
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i][0] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d	", arr[i][0]);
				fprintf(fp, "|%d	", arr[i][0]);
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|	");
			}
		}
		if(exist == 0){	//페이지폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴트 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("OPT Page Fault: %d\n\n", fault);		//전체 페이지폴트 수 출력
	fprintf(fp, "OPT Page Fault: %d\n\n", fault);	//전체 페이지폴트 수 저장
	
	//동적 할당 해제
	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
	free(arr_queue);
}
//FIFO 함수 정의
void fifo(int num, int *page_str, FILE *fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, fp:출력 저장 파일 포인터
	printf("FIFO\n");		//FIFO 메시지 출력
	fprintf(fp, "FIFO\n");		//FIFO 메시지 저장

	int fault = 0;			//fault 수 저장
	int idx = 0;			//큐 구현을 위한 인덱스, 교체한 페이지의 수
	
	int *arr = (int*)calloc(num, sizeof(int));	//페이지프레임, num만큼 동적으로 생성, 모든 변수 0으로 초기화
	
	//전체 페이지스트링 검사
	for(int k=0; k<PAGELEN; k++){
		int exist = 0;		//fault여부 체크 0:페이지폴트 1:히트
		for(int i=0; i<num; i++){	//페이지프레임 검사
			if(arr[i]==page_str[k]){	//페이지프레임에 검사하는 스트링 번호가 존재할 경우
				exist = 1;		//히트
				break;			//검사 종료
			}
		}
		
		//FIFO는 페이지의 교체가 차례대로 이루어지기 때문에, 배열을 원형 큐로 구현, idx를 num으로 나눠 교체할 배열의 인덱스를 가리킴.
		if(exist == 0){	//fautt가 날 경우
			int done = 0;	//페이지프레임의 빈 자리 여부
			if(arr[idx%num]==0){	//빈 자리가 있을 경우
				arr[idx%num] = page_str[k];	//빈 자리를 해당 스트링으로 교체
				done = 1;			//빈자리가 있다는 것을 체크
			}
			if(done == 0)	//fault가 나고 빈자리가 없을 경우
				arr[idx%num] = page_str[k];	//가장 오래된 페이지 교체
			fault++;	//페이지 폴트 수 1 증가
			idx++;		//교체한 index 1 증가
 		}
		printf("%d:	", page_str[k]);		//검사할 페이지스트링 번호 출력
		fprintf(fp, "%d:	", page_str[k]);	//검사할 페이지스트링 번호 저장
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d	", arr[i]);
				fprintf(fp, "|%d	", arr[i]);
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|	");
			} 
		}
		if(exist==0){	//페이지폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴터 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("FIFO Page Fault: %d\n\n", fault);		//전체 페이지폴트 수 출력
	fprintf(fp, "FIFO Page Fault: %d\n\n", fault);	//전체 페이지폴트 수 저장
	
	free(arr);	//동적 할당 해제
}
//LIFO 함수 정의
void lifo(int num, int *page_str, FILE *fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, fp:출력 저장 파일 포인터
	printf("LIFO\n");	//LIFO 메시지 출력
	fprintf(fp, "LIFO\n");	//LIFO 메시지 저장
	
	int fault = 0;		//fault 수 저장
	int *arr = (int*)calloc(num, sizeof(int));	//페이지프레임, num만큼 동적으로 생성, 모든 변수 0으로 초기화
	
	//전체 페이지스트링 검사
	for(int k=0; k<PAGELEN; k++){
		int exist = 0;	//fault 여부 체크
		for(int i=0; i<num; i++){	//페이지프레임 검사
			if(arr[i]==page_str[k]){	//페이지프레임에 검사하는 스트링 번호가 존재할 경우
				exist = 1;		//fault가 나지 않음
				break;			//검사 종료
			}
		}

		if(exist == 0){	//fault가 날 경우
			int done = 0;	//페이지프레임의 빈 자리 여부
			for(int i=0; i<num; i++){	//페이지프레임 검사
				if(arr[i]==0){	//빈 자리가 있을 경우
					arr[i] = page_str[k];	//빈 자리를 해당 스트링으로 교체
					done = 1;		//빈자리가 있다는 것을 체크
					break;			//검사 종료
				}
			}
			if(done == 0)		//fault가 나고 빈자리가 없을 경우
				arr[num-1] = page_str[k];	//마지막에 교체된 페이지 교체
			fault++;	//페이지 폴트 수 1 증가
		}
		printf("%d:	", page_str[k]);		//검사할 페이지스트링 번호 출력
		fprintf(fp, "%d:	", page_str[k]);	//검사할 페이지스트링 번호 저장
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d	", arr[i]);
				fprintf(fp, "|%d	", arr[i]);
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|	");
			}
		}
		if(exist==0){	//페이지폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴트 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("LIFO Page Fault: %d\n\n", fault);		//전체 페이지폴트 수 출력
	fprintf(fp, "LIFO Page Fault: %d\n\n", fault);	//전체 페이지폴트 수 저장
	
	//동적 할당 해제
	free(arr);
}
//LRU 함수 정의
void lru(int num, int *page_str, FILE *fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, fp: 출력 저장 파일 포인터
	printf("LRU(Least Recent Used)\n");		//LRU 메시지 출려
	fprintf(fp, "LRU(Least Recent Used)\n");	//LRU 메시지 저장

	int fault = 0;	//fault 수 저장
	int **arr = (int**)calloc(num, sizeof(int*));	//페이지프레임, num만큼 동적으로 생성, 모든 변수 0으로 초기화
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(2, sizeof(int));	//2차원 배열로 구현하여 arr[i][0]에 번호, arr[i][1]에 최근에 사용한 시점 저장

	int time = -1;	//현재 시점
	
	//전체 페이지 스트링 검사
	for(int k=0; k<PAGELEN; k++){
		time++;	//시점 변화, 1 증가
		int exist = 0;	//fault 여부 체크 0:페이지폴트 1:히트
		for(int i=0; i<num; i++){	//페이지 프레임 검사
			if(arr[i][0]==page_str[k]){	//페이지프레임에검사하는 스트링 번호가 존재할 경우
				arr[i][1] = time;	//최근에 사용한 시점 업데이트
				exist = 1;		//히트
				break;			//검사 종료
			}
		}

		if(exist == 0){	//fault가 날 경우
			int done = 0;	//페이지 프레임의 빈 자리 여부
			for(int i=0; i<num; i++){	//페이지프레임검사
				if(arr[i][0]==0){	//빈 자리가 있을 경우
					arr[i][0] = page_str[k];	//빈 자리를 해당 스트링으로 교체
					arr[i][1] = time;		//최근 사용한 시점 업데이트
					done = 1;			// 빈자리가 있다는 것을 체크
					break;				//검사종료
				}
			}

			if(done == 0){	//fault가 나고 빈자리가 없을 경우
				int idx = 0;	//가장 사용한 시점이 빠른 인덱스 저장
				for(int i=1; i< num; i++){	// 페이지 검사
					if(arr[idx][1] > arr[i][1])	// 만약 해당 인덱스의 시점이 더 빠른 경우
						idx = i;		// 인덱스 업데이트
				}
				arr[idx][0] = page_str[k];	//해당 인덱스의 페이지 교체
				arr[idx][1] = time;		//해당 인덱스의 페이지 사용 시점 업데이트
			}
			fault++;	// 페이지 폴트 수 1 증가
		}
		printf("%d:	", page_str[k]);		//검사할 페이지스트링 번호 출력
		fprintf(fp, "%d:	", page_str[k]);	// 검사할 페이지스트링 번호 저장
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i][0] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d	", arr[i][0]);
				fprintf(fp, "|%d	", arr[i][0]);
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|	");
			}
		}
		if(exist==0){	//페이지폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴트 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("LRU Page Fault: %d\n\n", fault);		//전체 페이지폴트 수 출력
	fprintf(fp, "LRU Page Fault: %d\n\n", fault);	//전체 페이지폴트 수 저장
	//동적 할당 해제
	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
}
//LFU 함수 정의
void lfu(int num, int *page_str, FILE *fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, fp:출력 저장 파일 포인터
	printf("LFU (Least Frequently Used)\n");	//LFU 메시지 출력
	fprintf(fp, "LFU (Least Frequently Used)\n");	//LFU 메시지 저장

	int fault = 0;	//fault 수 저장
	int **arr = (int**)calloc(num, sizeof(int*));		//페이지프레임, num만큼 동적으로 생성, 모든 변수 0으로 초기화
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(2, sizeof(int));	//2차원 배열로 구현하여 arr[i][0]에 번호, arr[i][1]에 참조 횟수 저장
	int *arr_queue = (int*)calloc(num, sizeof(int));	//두 번째 정렬 기준으로 FIFO 사용을 위한 큐
	//전체 페이지스트링 검사
	for(int k=0; k<PAGELEN; k++){
		int exist = 0;	//fault여부 체크
		for(int i=0; i<num; i++){	//페이지프레임 검사
			if(arr[i][0]==page_str[k]){	//페이지프레임에 검사하는 스트링 번호가 존재할 경우
				arr[i][1] += 1;	//참조 횟수 1 증가
				exist = 1;		//fault가 나지 않음
				break;			//검사 종료
			}
		}

		if(exist == 0){	//fault가 날 경우
			int done = 0;	//페이지 프레임의 빈 자리 여부
			for(int i=0; i<num; i++){	//페이지프레임 검사
				if(arr[i][0]==0){	//빈 자리가 있을 경우
					arr[i][0] = page_str[k];	//빈 자리를 해당 스트링으로 교체
					arr[i][1] = 0;			//참조 횟수수 0으로 초기화	
					arr_queue[i]=page_str[k];	//큐에 스트링 저장하여 순서 기억
					done = 1;			//빈자리가 있다는 것을 체크
					break;				//검사 종료
				}
			}

			if(done == 0){	//fault가 날 경우
				int idx = 0;	//가장 참조 횟수가 적은 페이지 인덱스 저장
				for(int i=0; i<num; i++){	//큐의 index
					for(int j=1; j< num; j++){	//모든 프레임 검사
						//큐를 사용하여 FIFO를 바탕으로 탐색, 먼저 들어온 프레임을 먼저 감사하기 때문에, 같은 참조 횟수를 가진 프레임을 FIFO에 따라 처리
						if(i==0 && arr_queue[i]==arr[j][0])	//큐의 가장 먼저온 프레임의 인덱스로 횟수가 가장 적은 프레임 초기화
							idx = j;
						else if(arr_queue[i]==arr[j][0] && arr[idx][1] > arr[j][1])	//해당 인덱스의 프레임의 참조 횟수가 적으면 업데이트
							idx = j;
					}
				}
				
				arr[idx][0] = page_str[k];	//참조 횟수가 가장 적은 프레임 교체
				arr[idx][1] = 0;		//참조 횟수 초기화
				//큐 업데이트
				for(int i=idx; i<num-1; i++)
					arr_queue[i] = arr_queue[i+1];
				arr_queue[num-1] = page_str[k];
			}
			fault++;	//페이지 폴트 수 1 증가
		}
		printf("%d:	", page_str[k]);		//검사할 페이지스트링 번호 출력
		fprintf(fp, "%d:	", page_str[k]);	//검사할 페이지스트링 번호 저장
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i][0] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d	", arr[i][0]);
				fprintf(fp, "|%d	", arr[i][0]);
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|	");
			}
		}
		if(exist==0){	//페이지폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴트 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("LFU Page Fault: %d\n\n", fault);		//전체 페이지폴트 수 출력
	fprintf(fp, "LFU Page Fault: %d\n\n", fault);	//전체 페이지폴트 수 저장
	//동적 할당 해제
	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
	free(arr_queue);
}
//SC 함수 정의
void sc(int num, int *page_str, FILE *fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, fp:출력 저장 파일 포인터
	printf("SC (Second Chance / One handed Clock)\n");		//SC 메시지 출력
	fprintf(fp, "SC (Second Chance / One handed Clock)\n");	//SC 메시지 저장

	int fault = 0;	//fault 수 저장
	int idx = 0;	//검사할 인덱스, 원형 큐 구현을 위한 인덱스, 교체한 페이지의 수
	int **arr = (int**)calloc(num, sizeof(int*));		//페이지프레임, num만큼 동적으로 생성, 모든 변수 0으로 초기화
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(2, sizeof(int));	//2차원 배열로 구현하여 arr[i][0]에 번호, arr[i][1]에 Reference bit 저장
	//전체 페이지스트링 검사
	for(int k=0; k<PAGELEN; k++){
		int exist = 0;	//fault 여부 체크
		for(int i=0; i<num; i++){	//페이지프레임 검사
			if(arr[i][0]==page_str[k]){	//페이지프레임에 검사하는 스트링 번호가 존재할 경우
				arr[i][1] = 1;		//Reference bit 1로 설정
				exist = 1;		//히트
				break;			//검사 종료
			}
		}
		if(exist == 0){	// fault가 날 경우
			int done = 0;	//페이지프레임 빈 자리 여부
			if(arr[idx%num][0]==0){	//빈 자리가 있을 경우
				arr[idx%num][0] = page_str[k];	//빈 자리를 해당 스트링으로 교체
				arr[idx%num][1] = 1;			//Reference bit 1으로 초기화
				idx++;					//검사할 큐 인덱스 1 증가
				done = 1;				//빈자리가 있다는 것을 체크
			}
			if(done==0){					//fault가 나고 빈자리가 없을 경우
				while(1){			//교체가 진행될 때까지 순회
					if(arr[idx%num][1]==0){	//Reference bit가 0인 경우
						arr[idx%num][0] = page_str[k];	//해당 프레임 교체
						arr[idx%num][1] = 1;			//R 비트 초기화
						idx++;					//검사할 큐 인덱스 1 증가
						break;					//검사 탈출
					}
					else				//Referece bit가 1인 경우
						arr[idx%num][1] = 0;	//Referece bit 0으로 업데이트
					idx++;				//검사할 큐 인덱스 1 증가
				}
			}
			fault++;	//페이지 폴트 수 1 증가
		}	
		printf("%d:	", page_str[k]);		//검사할 페이지스트링 번호 출력
		fprintf(fp, "%d:	", page_str[k]);	//검사할 페이지스트링 번호 저장
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i][0] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d", arr[i][0]);
				fprintf(fp, "|%d", arr[i][0]);
				if(arr[i][1]==1){	//R 비트가 1인 경우만 * 출력
					printf("*	");
					fprintf(fp, "*	");
				}
				else{			//R비트가 0인 경우
					printf("	");
					fprintf(fp, "	");
				}
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|	");
			}
		}
		if(exist==0){	//페이지 폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴트 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("SC Page Fault: %d\n\n", fault);		//전체 페이지 폴트 수 출력
	fprintf(fp, "SC Page Fault: %d\n\n", fault);	//전체 페이지 폴트 수 저장
	
	//동적 할당 해제
	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
}
//ESC 함수 정의
void esc(int num, int *page_str, int *page_rw, FILE *fp){	//인자 num:페이지프레임 개수, page_str:페이지스트링, page_rw:페이지스트링 RW비트, fp:출력 저장 파일 포인터
	printf("ESC (Enhanced Second Chance / Two handed clork)\n");		//ESC 메시지 출력
	fprintf(fp, "ESC (Enhanced Second Chance / Two handed clork)\n");	//ESC 메시지 출력

	int fault = 0;	//fault 수 저장
	int idx = 0;	//검사할 인덱스, 원형 큐 구현을 위한 인덱스, 교체한 페이지의 수
	int **arr = (int**)calloc(num, sizeof(int*));	//페이지프레임, num 만큼 동적으로 생성, 모든 변수 0으로 초기화
	for(int i=0; i<num; i++)
		arr[i] = (int*)calloc(3, sizeof(int));	//2차원 배열로 구현하여 arr[i][0]에 번호, arr[i][1]에 R비트, arr[i][2]에 D비트 저장
	//전체 페이지스트링 검사
	for(int k=0; k<PAGELEN; k++){
		int exist = 0;	//fault 여부 체크
		for(int i=0; i<num; i++){	//페이지 프레임 검사
			if(arr[i][0]==page_str[k]){	//페이지프레임에 검사하는 스트링 번호가 존재할 경우
				arr[i][1] = 1;		//R비트 1로 업데이트
				if(page_rw[k]==0)	//Read일 경우
					arr[i][2] = 0;	//D비트 0으로 업데이트
				else			//Write일 경우
					arr[i][2] = 1;	//D비트 1로 업데이트
				exist = 1;
				break;
			}
		}
		if(exist == 0){	//fault가 날 경우
			int done = 0;	//페이지프레임의 빈 자리 여부
			if(arr[idx%num][0]==0){	//빈 자리가 있을 경우
				arr[idx%num][0] = page_str[k];	//빈 자리를 해당 스트링으로 교체
				arr[idx%num][1] = 1;			//R비트 1로 초기화
				if(page_rw[k]==0)			//Read일 경우
					arr[idx%num][2] = 0;		//D비트 0으로 초기화
				else					//Write일 경우
					arr[idx%num][2] = 1;		//D비트 1로 초기화
				idx++;					//검사할 인덱스 1 증가
				done = 1;				//빈 자리가 있다는 것을 체크
			}
			if(done==0){			//fault가 나고 빈자리가 없을 경우
				while(1){			//교체가 진행될 때까지 순회
					if(arr[idx%num][1]==0 && arr[idx%num][2]==0){	//R bit: 0, D bot: 0
						arr[idx%num][0] = page_str[k];	//해당 프레임 교체
						arr[idx%num][1] = 1;			//R 비트 1로 업데이트
						if(page_rw[k]==0)			//Read일 경우
							arr[idx%num][2] = 0;		//D비트 0으로 업데이트
						else					//Write일 경우
							arr[idx%num][2] = 1;		//D비트 1로 업데이트
						idx++;					//검사할 큐 인덱스 1 증가
						break;					//검사 종료
					}
					else if((arr[idx%num][1]==0 && arr[idx%num][2]==1) || (arr[idx%num][1]==1 && arr[idx%num][2]==0)){	//R bit: 0, D bot: 1 or R bit: 1, D bot: 0
						arr[idx%num][1] = 0;	//R비트 1로 업데이트
						arr[idx%num][2] = 0;	//D비트 0으로 업데이트
					}
					else if(arr[idx%num][1]==1 && arr[idx%num][2]==1){	//R bit: 1, D bot: 1
						arr[idx%num][1] = 0;	//R비트 0로 업데이트
						arr[idx%num][2] = 1;	//D비트 1으로 업데이트
					}
					idx++;				//검사할 큐 인덱스 1 증가
				}
				
			fault++;	//페이지 폴트 수 1증가
			}
		}
		//검사할 페이지스트링 번호 및 RW비트 출력 및 저장
		if(page_rw[k]==0){	//페이지스트링 Read
			printf("%d(R):	", page_str[k]);
			fprintf(fp, "%d(R):	", page_str[k]);
		}
		else{			//페이지스트링 Write
			printf("%d(W):	", page_str[k]);
			fprintf(fp, "%d(W):	", page_str[k]);
		}
		//페이지프레임 번호 출력 및 저장
		for(int i=0; i<num; i++){
			if(arr[i][0] != 0){	//프레임이 비어 있지 않은 경우
				printf("|%d(%d%d)	", arr[i][0], arr[i][1], arr[i][2]);
				fprintf(fp, "| %d(%d%d)	", arr[i][0], arr[i][1], arr[i][2]);
			}
			else{			//프레임이 비어 있는 경우
				printf("|	");
				fprintf(fp, "|		");
			}
		}
		
		if(exist==0){	//페이지 폴트가 이뤄진 경우
			printf("|F");		//폴트 출력
			fprintf(fp, "|F");	//폴트 저장
		}
		printf("\n");
		fprintf(fp, "\n");
	}
	printf("ESC Page Fault: %d\n\n", fault);		//전체 페이지폴트 수 출력
	fprintf(fp, "ESC Page Fault: %d\n\n", fault);	//전체 페이지폴트 수 저장
	//동적 할당 해제
	for(int i=0; i<num; i++)
		free(arr[i]);
	free(arr);
}
