#include <stdio.h>

void opt(int num, int *page_str, FILE *fp);					//optimal 알고리즘 함수 선언
void fifo(int num, int *page_str, FILE *fp);				//FIFO 알고리즘 함수 선언
void lifo(int num, int *page_str, FILE *fp);				//LIFO 알고리즘 함수 선언
void lru(int num, int *page_str, FILE *fp);					//LRU 알고리즘 함수 선언
void lfu(int num, int *page_str, FILE *fp);					//LFU 알고리즘 함수 선언
void sc(int num, int *page_str, FILE *fp);					//SC 알고리즘 함수 선언
void esc(int num, int *page_str, int *page_rw, FILE *fp);	//ESC 알고리즘 함수 선언
