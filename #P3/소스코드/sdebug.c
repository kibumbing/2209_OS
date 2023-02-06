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
  
  //PNUM, �� fork()�� ������ ���μ����� ������ŭ loop�� ��
  for(n = 0; n < PNUM; n++){

	//fork()�� ���μ����� �����ϰ�, ������ �ð��� start�� ����
	pid = fork();
	start = uptime();

	//counter 0���� �ʱ�ȭ
	counter = 0;
	int print_done = 0;

	if(pid < 0)
	  break;

	//���� ������ ���μ����� �ڽ� ���μ����� ��� if������ ������ ����
	if(pid == 0){
	  
	  //������ ���μ����� weight���� �ο�
	  //weight���� n+1��, ������ ������ ���� 1���� ������
	  weightset(n+1);
	  
	  //counter ���� TOTAL_COUNTER�̸� loop ���� �� ���μ��� ����
	  do{
		//���μ��� ������ �� ���� ��µ��� �ʾҰ�, counter���� PRINT_CYCLE�̸� ���μ��� ���
		if(print_done != 1 && counter == PRINT_CYCLE){
		  printf(1, "PID: %d, WEIGHT: %d, TIMES: %d ms\n", getpid(), n+1, (uptime()-start)*10);
		  
		  //���μ��� ������ ��µ��� �˸�
		  print_done = 1;
		}
		//���μ����� ����� ��, counter ���� clock_tick���� 1�� ����
		counter++;
	  }while(counter < TOTAL_COUNTER);
	  
	  //���μ��� ����
	  printf(1, "PID: %d terminated\n", getpid());
	  exit();
	}
  }
  
  //������ ��� �ڽ� ���μ����� ���Ḧ ��ٸ�
  for(; n > 0; n--){
	//�ڽ� ���μ����� ���� ��� wait()�� -1�� ��ȯ, ����
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
