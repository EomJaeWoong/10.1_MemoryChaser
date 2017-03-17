#include <iostream>
#include <stdlib.h>
#include "Memory.h"

//�ִ� �޸� ���� ��
#define MAX_MEMORY 100

//�޸� �Ҵ� ��ũ�� �Լ� ����
#define MemoryAlloc(TYPE, AMOUNT) _MemoryAlloc<TYPE>(AMOUNT, __FILE__, __LINE__)

//�Ҵ� ���� ���� �޸� ����ü ����
typedef struct _Memory{
	char fileName[128];
	int size;
	int line;
	void *memoryPtr;
} Memory;

//�޸� �迭 ����
Memory mem[MAX_MEMORY];

//�Ҵ���� ����
static int countAlloc = 0;
//�Ҵ���� �� �޸� ũ��
static int countSize = 0;

//------------------------------------------------------------
//�޸� �迭 �ʱ�ȭ
//------------------------------------------------------------
void initMemoryArray();


//------------------------------------------------------------
//�޸� �Ҵ�
//------------------------------------------------------------
template <typename T>
T* _MemoryAlloc(int amount, char* fileName, int line)
{
	//�޸� �迭 ������� �ε����� ã��
	int SpaceIndex = findEmptySpace();

	//�޸� �Ҵ�
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
//�޸� ��ȯ
// - �޸� �迭������ ����
// - �޸� ���� ��ȯ
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
// ��� �޸� ��Ȳ ���
//------------------------------------------------------------
void printAllMemory();


//------------------------------------------------------------
//�޸� �迭�� ����ִ� ������ ã�´�
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
//�޸� �迭 �ʱ�ȭ
//------------------------------------------------------------
void initMemoryArray()
{
	for (int iCnt = 0; iCnt < MAX_MEMORY; iCnt++)
		mem[iCnt].memoryPtr = NULL;
}


//------------------------------------------------------------
// ��ȯ���� ���� �޸𸮵��� �������� ���
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
//�޸� �迭�� ����ִ� ������ ã�´�
//------------------------------------------------------------
int findEmptySpace()
{
	for (int iCnt = 0; iCnt < MAX_MEMORY; iCnt++){
		if (mem[iCnt].memoryPtr == NULL)
			return iCnt;
	}
	return -1;
}