#include <iostream>
#include <stdlib.h>
#include "Memory.h"

//최대 메모리 저장 수
#define MAX_MEMORY 100

//메모리 할당 매크로 함수 정의
#define MemoryAlloc(TYPE, AMOUNT) _MemoryAlloc<TYPE>(AMOUNT, __FILE__, __LINE__)

//할당 받은 동적 메모리 구조체 정의
typedef struct _Memory{
	char fileName[128];
	int size;
	int line;
	void *memoryPtr;
} Memory;

//메모리 배열 정의
Memory mem[MAX_MEMORY];

//할당받은 갯수
static int countAlloc = 0;
//할당받은 총 메모리 크기
static int countSize = 0;

//------------------------------------------------------------
//메모리 배열 초기화
//------------------------------------------------------------
void initMemoryArray();


//------------------------------------------------------------
//메모리 할당
//------------------------------------------------------------
template <typename T>
T* _MemoryAlloc(int amount, char* fileName, int line)
{
	//메모리 배열 빈공간의 인덱스를 찾음
	int SpaceIndex = findEmptySpace();

	//메모리 할당
	if (SpaceIndex != -1){
		T* ptr = new T[amount];

		mem[SpaceIndex].memoryPtr = ptr;
		mem[SpaceIndex].size = sizeof(T)*amount;
		strcpy_s(mem[SpaceIndex].fileName, 127, fileName);
		mem[SpaceIndex].line = line;

		countAlloc++;
		countSize += sizeof(T)*amount;
		return ptr;
	}
	return NULL;
}


//------------------------------------------------------------
//메모리 반환
// - 메모리 배열에서의 삭제
// - 메모리 실제 반환
//------------------------------------------------------------
template <typename R>
void ReleaseMemory(R ptr)
{
	for (int iCnt = 0; iCnt < MAX_MEMORY; iCnt++){
		if (mem[iCnt].memoryPtr == ptr){
			mem[iCnt].memoryPtr = NULL;
			delete[] ptr;
		}
	}
}

//------------------------------------------------------------
// 모든 메모리 현황 출력
//------------------------------------------------------------
void printAllMemory();


//------------------------------------------------------------
//메모리 배열의 비어있는 공간을 찾는다
//------------------------------------------------------------
int findEmptySpace();


void main()
{
	initMemoryArray();

	char *c = MemoryAlloc(char, 2);
	int *i = MemoryAlloc(int, 1);
	short *s = MemoryAlloc(short, 8);
	long *l = MemoryAlloc(long, 3);
	float *f = MemoryAlloc(float, 5);
	double *d = MemoryAlloc(double, 4);

	ReleaseMemory(c);
	ReleaseMemory(i);
	ReleaseMemory(s);
	ReleaseMemory(f);

	printAllMemory();
}

//------------------------------------------------------------
//메모리 배열 초기화
//------------------------------------------------------------
void initMemoryArray()
{
	for (int iCnt = 0; iCnt < MAX_MEMORY; iCnt++)
		mem[iCnt].memoryPtr = NULL;
}


//------------------------------------------------------------
// 반환되지 않은 메모리들의 상세정보를 출력
//------------------------------------------------------------
void printAllMemory()
{	
	for (int iCnt = 0; iCnt < MAX_MEMORY; iCnt++){
		if (mem[iCnt].memoryPtr != NULL){
			printf("No Release Memory : 0x%08x, Memory Size : %d\n", mem[iCnt].memoryPtr, mem[iCnt].size);
			printf("File Name : %s, Line : %d\n\n", mem[iCnt].fileName, mem[iCnt].line);
		}
	}

	printf("All Size : %d\n", countSize);
	printf("All Size : %d\n", countAlloc);
}



//------------------------------------------------------------
//메모리 배열의 비어있는 공간을 찾는다
//------------------------------------------------------------
int findEmptySpace()
{
	for (int iCnt = 0; iCnt < MAX_MEMORY; iCnt++){
		if (mem[iCnt].memoryPtr == NULL)
			return iCnt;
	}
	return -1;
}