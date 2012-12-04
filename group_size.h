#ifndef __GROUP_SIZE_H__
#define __GROUP_SIZE__

#define MAX_LIST_SIZE 32

typedef unsigned char  BOOL;          // boollean 
typedef unsigned char uint8;

void viewInit(unsigned char mouseID);
BOOL nodeAdd(const uint8 node);
int nodeFind(const unsigned char node);
void viewMerge(unsigned char *list);

extern unsigned char gView[MAX_SIZE];  //�洢�����ڵ��ȫ����ͼ����ת��
extern unsigned char gSize;             //ȫ����ͼ�нڵ����

#endif
