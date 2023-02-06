CC=gcc					#컴파일러
CFLAGS=-g -Wall			#컴파일 옵션
TARGET=a.out			#빌드 대상 이름
OBJS=algorithm.o main.o	#Object 파일 목록

#a.out 빌드
$(TARGET): $(OBJS)	
	$(CC) -o $@ $(OBJS)

#algorithm.c 컴파일
algorithm.o: algorithm.c
	$(CC) -c -o algorithm.o algorithm.c

#main.c 컴파일
main.o: main.c
	$(CC) -c -o main.o main.c

#make clean 구현
clean:
	rm $(OBJECT) $(TARGET)
