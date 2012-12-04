#include "group_size.h"

unsigned char gView[MAX_SIZE];  //存储整个节点的全局视图，并转发 gView[0]:标识符，gView[1]:size
unsigned char gSize = 0;             //全局视图中节点个数

/*
**全局视图初始化，添加自身ID
**
*/
void viewInit(unsigned char mouseID)
{
  gView[0] = 0xFF;
  for(int i = 1; i < MAX_SIZE; i++)  // 1~30
  {
    // These IDs should not be '0'
    gView[i] = 0;
  }
  gSize = 0;
  nodeAdd(mouseID);
}

/*
**将节点ID添加到gView中，从gView[2]开始
**
*/
BOOL nodeAdd(const uint8 node)
{
  // Is the array full
  if(gSize >= MAX_SIZE)
    return FALSE;
  
  gView[2 + gSize++] = node;       //从第二个节点开始添加ID
  gView[1] = gSize;                //将节点个数存入gView中
  
  return TRUE;
}

int nodeFind(const uint8 node)
{
  // If node is found, return its index 
  for(int i = 2; i < gSize + 2; i++)
  {
    if(gView[i] == node)
      return i;
  }
  
  // If node is not exsited in the list, return -1
  return -1;
}

void nodeDelete(const uint8 node)
{
  int index = nodeFind(node);
  
  if(-1 == index)
    return;
  
  for(uint8 i = (uint8)index + 1; i < gSize + 1; i++)
  {
    gView[i] = gView[i+1];   //节点向前移动一位
  }
  
  gSize--;      //个数减1
}

unsigned char getSize()
{
  return gSize;   //返回整个群体节点数
}

uint8* list_head()
{
  return gView;   //返回数组地址
}

void viewMerge(uint8 *list)
{
  if(list[0] != 0xFF)
        return;
  for(int i = 2; i < list[1] + 2; i++)
  {
    if(-1 == nodeFind(list[i]))   //Node doesn't exist
    {
      nodeAdd(list[i]);
    }
  }
}