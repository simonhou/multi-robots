#include "group_size.h"

unsigned char gView[MAX_SIZE];  //�洢�����ڵ��ȫ����ͼ����ת�� gView[0]:��ʶ����gView[1]:size
unsigned char gSize = 0;             //ȫ����ͼ�нڵ����

/*
**ȫ����ͼ��ʼ�����������ID
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
**���ڵ�ID��ӵ�gView�У���gView[2]��ʼ
**
*/
BOOL nodeAdd(const uint8 node)
{
  // Is the array full
  if(gSize >= MAX_SIZE)
    return FALSE;
  
  gView[2 + gSize++] = node;       //�ӵڶ����ڵ㿪ʼ���ID
  gView[1] = gSize;                //���ڵ��������gView��
  
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
    gView[i] = gView[i+1];   //�ڵ���ǰ�ƶ�һλ
  }
  
  gSize--;      //������1
}

unsigned char getSize()
{
  return gSize;   //��������Ⱥ��ڵ���
}

uint8* list_head()
{
  return gView;   //���������ַ
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