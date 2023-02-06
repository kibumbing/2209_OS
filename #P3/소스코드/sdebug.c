#include "types.h"
#include "stat.h"
#include "user.h"

#define PNUM 5
#define PRINT_CYCLE 10000000
#define TOTAL_COUNTER 500000000

void
sdebug_func(void)
{
  int n, pid;
  int start;
  int counter;
  
  printf(1, "start sdebug command\n");
  
  //PNUM, 즉 fork()로 생성한 프로세스의 개수만큼 loop를 돎
  for(n = 0; n < PNUM; n++){

	//fork()로 프로세스를 생성하고, 생성한 시간을 start에 저장
	pid = fork();
	start = uptime();

	//counter 0으로 초기화
	counter = 0;
	int print_done = 0;

	if(pid < 0)
	  break;

	//만약 생성된 프로세스가 자식 프로세스일 경우 if문안의 내용을 수행
	if(pid == 0){
	  
	  //생성된 프로세스의 weight값을 부여
	  //weight값은 n+1로, 생성된 순서에 따라 1부터 증가함
	  weightset(n+1);
	  
	  //counter 값이 TOTAL_COUNTER이면 loop 종료 후 프로세스 종료
	  do{
		//프로세스 정보가 한 번도 출력되지 않았고, counter값이 PRINT_CYCLE이면 프로세스 출력
		if(print_done != 1 && counter == PRINT_CYCLE){
		  printf(1, "PID: %d, WEIGHT: %d, TIMES: %d ms\n", getpid(), n+1, (uptime()-start)*10);
		  
		  //프로세스 정보가 출력됨을 알림
		  print_done = 1;
		}
		//프로세스가 수행될 때, counter 값은 clock_tick마다 1씩 증가
		counter++;
	  }while(counter < TOTAL_COUNTER);
	  
	  //프로세스 종료
	  printf(1, "PID: %d terminated\n", getpid());
	  exit();
	}
  }
  
  //생성한 모든 자식 프로세스의 종료를 기다림
  for(; n > 0; n--){
	//자식 프로세스가 없는 경우 wait()은 -1을 반환, 종료
	if(wait() < 0)
	  exit();
  }
  if(wait() != -1){
	exit();
  }

  printf(1, "end sdebug command\n");
}

int main(void){
  sdebug_func();
  exit();
}
