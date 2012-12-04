/****************************************************************
 * file:	main.c
 * date:	2012-12-03
 * author:      Hou Jun
 * description: Main file of the project, implementation of gossip
 ***************************************************************/
#include "baselib.h"

#include "delay.h"
#include "nrf24l01.h"
#include "led.h"
#include "motor.h"
#include "keys.h"
#include "ISR.h"

#include "key_led_test.h"
#include "wireless_comm_test.h"
#include "walk_test.h"
#include "motor_encoder_test.h"
/*********************************************************************************************************
  全局变量定义
*********************************************************************************************************/

#define MAX_SIZE 32
#define TRUE  1
#define FALSE 0
#define NULL  0

typedef unsigned char  BOOL;          // boollean 
typedef unsigned char uint8;



extern unsigned long  LeftMotorStep ;
extern unsigned long  RightMotorStep ; 
extern L01Buf L01RcvBuf;

unsigned char robotID = 9;
unsigned long distence = 0;
unsigned char TestNum=0;


unsigned char receive[32];      //接收其它节点的数据包
unsigned char gView[MAX_SIZE];  //存储整个节点的全局视图，并转发
unsigned char gSize = 0;             //全局视图中节点个数


// 条件变量定义
unsigned char car_num = '1';
unsigned char info_cmd = '1';
unsigned char run_cmd = '2';
unsigned char forward_cmd = '3';
unsigned char backward_cmd = '4';
unsigned char turnleft_cmd = '5';
unsigned char turnright_cmd = '6';
unsigned char stop_cmd = '7';

void sysInit()
{
    //配置系统时钟
     
    SysCtlClockSet(SYSCTL_SYSDIV_1  | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_8MHZ); 
    IntMasterEnable();
}

unsigned char Scan(void)
{
  unsigned char rcvData[32];
  if(L01_Buf_Read(&L01RcvBuf,rcvData)==1)
  {
      int index;
      for(index=0; index < 32; index++)
      {
        receive[index] = rcvData[index];
      }
      return 1;
  }
  return 0;
}


void delay(unsigned long int d){        
  for(; d; --d);  
}

void TurnOnLed(unsigned long ID)
{
  unsigned long led_0 = ID & 0x01;
  unsigned long led_1 = (ID >> 1)& 0x01 ;
  unsigned long led_2 = (ID >> 2) & 0x01 ;
  if(led_0)
  {
    led_0 = 0x00;
  }
  else led_0 = 0xff;
  if(led_1)
  {
    led_1 = 0x00;
  }else led_1 = 0xff;
  if(led_2)
  {
    led_2 = 0x00;
  }else led_2 = 0xff;
  GPIOPinWrite(GPIO_PORTF_BASE, LED0, led_0);
  GPIOPinWrite(GPIO_PORTF_BASE, LED1, led_1);
  GPIOPinWrite(GPIO_PORTF_BASE, LED2, led_2);
}

void viewInit(unsigned char mouseID);
BOOL nodeAdd(const uint8 node);
int nodeFind(const unsigned char node);
void viewMerge(unsigned char *list);

int main(void) 
{   
  // unsigned int i,j;///
   //unsigned char rcvData[32];
    sysInit();
    delay_ms(50);
    initLed();
    initMotor();
    initPWM();
    initKey();
    initSW();
   // initInfrared();
    
    initNrf24l01();
    initTimer();

    viewInit(robotID);
        
    TX_Mode(SSI1_BASE);
    RX_Mode(SSI0_BASE);

    while(1)
    {
        NRF24L01_TxPacket(SSI1_BASE,gView);
        delay(15000);
        if( Scan() == 1 )
        {
          viewMerge(receive);        
        
        }
        TurnOnLed(gSize);
        
    }

//    while(1)
//    {
//      if(Scan()== 1)
//      {
//        if(car_num == receive[0])
//        {
//          unsigned char response[32] = {0};
//          if(info_cmd == receive[1])
//          {
//            response[0] = 'd';
//            response[1] = car_num;
//            response[2] = info_cmd;
//            NRF24L01_TxPacket(SSI1_BASE,response);
//            delay_ms(8);NRF24L01_TxPacket(SSI1_BASE,txbuf);
//          }
//          else if(run_cmd == receive[1])
//          {
//            forward();
//          }
//          else if(forward_cmd == receive[1])
//          {
//            forward();
//          }
//          else if(backward_cmd == receive[1])
//          {
//            backward();
//          }
//          else if(turnleft_cmd == receive[1])
//          {
//            turnLeft();
//            delay(150000);
//            forward();
//          }
//          else if(turnright_cmd == receive[1])
//          {
//            turnRight();
//            delay(150000);
//            forward();
//          
//          }
//          else if(stop_cmd == receive[1])
//          {
//            stop();
//          }
//        }
//      }
//    }
}

/*
**全局视图初始化，添加节点
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

BOOL nodeAdd(const uint8 node)
{
  // Is the array full
  if(gSize >= MAX_SIZE)
    return FALSE;
  
  gView[2 + gSize++] = node;       //从第二个节点开始添加ID
  gView[1] = gSize;                //将节点个数存入gView中
  
  return TRUE;
}

int nodeFind(const unsigned char node)
{
  // If node is found, return its index (start from 0)
  for(int i = 2; i < gSize + 2; i++)
  {
    if(gView[i] == node)
      return i;
  }
  
  // If node is not exsited in the list, return -1
  return -1;
}

void nodeDelete(const unsigned char node)
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

void viewMerge(unsigned char *list)
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
