/* ���� */
/* �ṩprintf�ӳ������ʱ���� */

#ifndef	__DEBUG_H__
#define __DEBUG_H__

//���庯������ֵ
#ifndef  SUCCESS
#define  SUCCESS  0
#endif
#ifndef  FAIL
#define  FAIL    0xFF
#endif

//���嶨ʱ����ʼ
#ifndef  START
#define  START  1
#endif
#ifndef  STOP
#define  STOP    0
#endif

#define	 FREQ_SYS	     12000000	         //ϵͳ��Ƶ12MHz
#define  OSC_EN_XT     0                 //�ⲿ����ʹ�ܣ�Ĭ�Ͽ����ڲ�����
#ifndef  UART0_BUAD
#define  UART0_BUAD    57600
#define  UART1_BUAD    57600
#endif
#ifndef  DE_PRINTF                      //����Pirntf�������
#define  DE_PRINTF     1
#endif

void	CfgFsys( );                        //CH554ʱ��ѡ�������
void	mDelayuS( UINT16 n );              // ��uSΪ��λ��ʱ
void	mDelaymS( UINT16 n );              // ��mSΪ��λ��ʱ
void    CH554UART0Alter();                 //CH554����0����ӳ�䵽P1.2/P1.3
void	mInitSTDIO( );                      //T1��Ϊ�����ʷ�����
UINT8   CH554UART0RcvByte( );              //CH554 UART0��ѯ��ʽ����һ���ֽ�
void    CH554UART0SendByte(UINT8 SendDat); //CH554UART0����һ���ֽ�

void    CH554WDTModeSelect(UINT8 mode);     //CH554���Ź�ģʽ���� 
void    CH554WDTFeed(UINT8 tim);            //CH554���Ź�ι��

// void CH554SoftReset( );                  //CH554����λ
// void CH554USBDevWakeup( );               //CH554��USB�豸��������
//  
#endif