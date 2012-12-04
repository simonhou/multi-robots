#ifndef __GROUP_SIZE_H__
#define __GROUP_SIZE__

#define MAX_LIST_SIZE 32

typedef unsigned char  BOOL;          // boollean 
typedef unsigned char uint8;

void viewInit(unsigned char mouseID);
BOOL nodeAdd(const uint8 node);
int nodeFind(const unsigned char node);
void viewMerge(unsigned char *list);

extern unsigned char gView[MAX_SIZE];  //存储整个节点的全局视图，并转发
extern unsigned char gSize;             //全局视图中节点个数

#endif
