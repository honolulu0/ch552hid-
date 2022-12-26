
/********************************** (C) COPYRIGHT *******************************
* File Name          :EXAM11.C
* Author             : WCH
* Version            : V1.0
* Date               : 2017/01/20
* Description        : CH554 C���Ե�U��Ŀ¼�ļ�ö�ٳ���
 ֧��: FAT12/FAT16/FAT32  
*******************************************************************************/

#include "..\..\..\Public\CH554.H"                                                        
#include <stdio.h>
#include <string.h>

//#define DISK_BASE_BUF_LEN		512	/* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�(����ѡ��Ϊ2048����4096��֧��ĳЩ��������U��),Ϊ0���ֹ�ڱ��ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
#define FOR_ROOT_UDISK_ONLY		 1// ֻ����DP/DM�˿ڵ�U���ļ�����(ʹ���ӳ����CH554UFI/X),��֧��HUB��U�̲���
//����Ҫ����LIB���ļ�

//#define NO_DEFAULT_ACCESS_SECTOR	1		/* ��ֹĬ�ϵĴ���������д�ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_DISK_CONNECT		1		/* ��ֹĬ�ϵļ����������ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_FILE_ENUMER		1		/* ��ֹĬ�ϵ��ļ���ö�ٻص�����,���������б�д�ĳ�������� */

#include "..\..\USB_LIB\CH554UFI.H"
#include "..\..\USB_LIB\CH554UFI.C"
#include "..\..\..\Public\DEBUG.H"
#include "..\..\USB_LIB\USBHOST.H"

// ���ӳ��򷵻�״̬��
#define	ERR_SUCCESS			  0x00	// �����ɹ�
#define	ERR_USB_CONNECT		0x15	/* ��⵽USB�豸�����¼�,�Ѿ����� */
#define	ERR_USB_DISCON		0x16	/* ��⵽USB�豸�Ͽ��¼�,�Ѿ��Ͽ� */
#define	ERR_USB_BUF_OVER	0x17	/* USB��������������������̫�໺������� */
#define	ERR_USB_DISK_ERR	0x1F	/* USB�洢������ʧ��,�ڳ�ʼ��ʱ������USB�洢����֧��,�ڶ�д�����п����Ǵ����𻵻����Ѿ��Ͽ� */
#define	ERR_USB_TRANSFER	0x20	/* NAK/STALL�ȸ����������0x20~0x2F */
#define	ERR_USB_UNSUPPORT	0xFB
#define	ERR_USB_UNKNOWN		0xFE

// ��ȡ�豸������
UINT8C	SetupGetDevDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_DEVICE, 0x00, 0x00, sizeof( USB_DEV_DESCR ), 0x00 };
// ��ȡ����������
UINT8C	SetupGetCfgDescr[] = { USB_REQ_TYP_IN, USB_GET_DESCRIPTOR, 0x00, USB_DESCR_TYP_CONFIG, 0x00, 0x00, 0x04, 0x00 };
// ����USB��ַ
UINT8C	SetupSetUsbAddr[] = { USB_REQ_TYP_OUT, USB_SET_ADDRESS, USB_DEVICE_ADDR, 0x00, 0x00, 0x00, 0x00, 0x00 };
// ����USB����
UINT8C	SetupSetUsbConfig[] = { USB_REQ_TYP_OUT, USB_SET_CONFIGURATION, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
// ����˵�STALL
UINT8C	SetupClrEndpStall[] = { USB_REQ_TYP_OUT | USB_REQ_RECIP_ENDP, USB_CLEAR_FEATURE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

UINT8X	UsbDevEndp0Size;	/* USB�豸�Ķ˵�0�������ߴ� */

//USB�豸�����Ϣ��,CH554���֧��2���豸
#define	ROOT_DEV_DISCONNECT		0
#define	ROOT_DEV_CONNECTED		1
#define	ROOT_DEV_FAILED			2
#define	ROOT_DEV_SUCCESS		3

UINT8X	RxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0000 ;  // IN, must even address
UINT8X	TxBuffer[ MAX_PACKET_SIZE ] _at_ 0x0040 ;  // OUT, must even address
#define	pSetupReq	((PXUSB_SETUP_REQ)TxBuffer)
bit		FoundNewDev;

#pragma NOAREGS

/* ������״̬,�����������ʾ������벢ͣ�� */
void	mStopIfError( UINT8 iError )
{
	if ( iError == ERR_SUCCESS ) return;  /* �����ɹ� */
	printf( "Error: %02X\n", (UINT16)iError );  /* ��ʾ���� */
/* ���������,Ӧ�÷����������Լ�CH554DiskStatus״̬,�������CH554DiskReady��ѯ��ǰU���Ƿ�����,���U���ѶϿ���ô�����µȴ�U�̲����ٲ���,
   ���������Ĵ�������:
   1������һ��CH554DiskReady,�ɹ����������,����Open,Read/Write��
   2�����CH554DiskReady���ɹ�,��ôǿ�н���ͷ��ʼ����(�ȴ�U�����ӣ�CH554DiskReady��) */
	while ( 1 ) {
//		LED_TMP=0;  /* LED��˸ */
//		mDelaymS( 100 );
//		LED_TMP=1;
//		mDelaymS( 100 );
	}
}

void main( ) 
{
		UINT8	s,i;
		UINT8	 *pCodeStr;
    UINT16 j;
    CfgFsys();
    mDelaymS(5);                                                              //�޸���Ƶ�Լ���ʱ�ȴ���Ƶ�ȶ�
	  mInitSTDIO( );                                                            //��ʼ������0Ϊ���ü����ͨ�����ڼ����ʾ���� */
		InitUSB_Host( );
		CH554LibInit( );                                                          //��ʼ��CH554�������֧��U���ļ�
		FoundNewDev = 0;
		printf( "Wait Device In\n" );
		while ( 1 ) 
		{
				s = ERR_SUCCESS;
				if ( UIF_DETECT )                                                     // �����USB��������ж�����
			  {  
				  UIF_DETECT = 0;                                                     // ���жϱ�־
					s = AnalyzeRootHub( );                                              // ����ROOT-HUB״̬
					if ( s == ERR_USB_CONNECT ) 
					FoundNewDev = 1;
		    }
				if ( FoundNewDev || s == ERR_USB_CONNECT ) 
				{                                                                     // ���µ�USB�豸����
					FoundNewDev = 0;
					mDelaymS( 200 );                                                    // ����USB�豸�ղ�����δ�ȶ�,�ʵȴ�USB�豸���ٺ���,������ζ���
					s = InitRootDevice( );                                              // ��ʼ��USB�豸
					if ( s == ERR_SUCCESS ) 
					{
						 printf( "Start UDISK_demo @CH554UFI library\n" );
						// U�̲������̣�USB���߸�λ��U�����ӡ���ȡ�豸������������USB��ַ����ѡ�Ļ�ȡ������������֮�󵽴�˴�����CH554�ӳ���������ɺ�������
						 CH554DiskStatus = DISK_USB_ADDR;
						 for ( i = 0; i != 10; i ++ ) 
						 {
							 printf( "Wait DiskReady\n" );
							 s = CH554DiskReady( );
							 if ( s == ERR_SUCCESS ) break;
							 mDelaymS( 50 );
						 }
						 if ( CH554DiskStatus >= DISK_MOUNTED )                           //U��׼����
						 {  
/* ���ļ� */
							 printf( "Open\n" );
		           strcpy( mCmdParam.Open.mPathName, "/C51/CH554HFT.C" );         //����Ҫ�������ļ�����·��
		           s = CH554FileOpen( );                                          //���ļ�
							 if ( s == ERR_MISS_DIR ) {printf("�����ڸ��ļ������г���Ŀ¼�����ļ�\n");pCodeStr = "/*"; }
							 else pCodeStr = "/C51/CH554*";                                 //CH554HFT.C�ļ����������г�\C51��Ŀ¼�µ���CH554��ͷ���ļ�
							 printf( "List file %s\n", pCodeStr );                           
							 for ( j = 0; j < 10000; j ++ )                                 //�޶�10000���ļ�,ʵ����û������ 
							 {  
									 strcpy( mCmdParam.Open.mPathName, pCodeStr );              //�����ļ���,*Ϊͨ���,�����������ļ�������Ŀ¼
									 i = strlen( mCmdParam.Open.mPathName );
									 mCmdParam.Open.mPathName[ i ] = 0xFF;                      //�����ַ������Ƚ��������滻Ϊ���������,��0��254,�����0xFF��255��˵�����������CH554vFileSize������
									 CH554vFileSize = j;                                        //ָ������/ö�ٵ����
									 i = CH554FileOpen( );                                      //���ļ�,����ļ����к���ͨ���*,��Ϊ�����ļ������� 
									 /* CH554FileEnum �� CH554FileOpen ��Ψһ�����ǵ����߷���ERR_FOUND_NAMEʱ��ô��Ӧ��ǰ�߷���ERR_SUCCESS */
									 if ( i == ERR_MISS_FILE ) break;                           //��Ҳ��������ƥ����ļ�,�Ѿ�û��ƥ����ļ��� 
									 if ( i == ERR_FOUND_NAME )                                 //��������ͨ�����ƥ����ļ���,�ļ�����������·������������� 
									 {  
										 printf( "  match file %04d#: %s\n", (unsigned int)j, mCmdParam.Open.mPathName );  /* ��ʾ��ź���������ƥ���ļ���������Ŀ¼�� */
										 continue;                                                /* ����������һ��ƥ����ļ���,�´�����ʱ��Ż��1 */
									 }
									 else                                                       //����
									 {  
										 mStopIfError( i );
										 break;
									 }	 
		            }
			        i = CH554FileClose( );                                          //�ر��ļ�
		          printf( "U����ʾ���\n" );
		       }
		       else 
		       {
		          printf( "U��û��׼���� ERR =%02X\n", (UINT16)s );
		       }
		     }
		     else
		     {
		        printf("��ʼ��U��ʧ�ܣ������U������\n");
		     }
       }
		   mDelaymS( 100 );  // ģ�ⵥƬ����������
		   SetUsbSpeed( 1 );  // Ĭ��Ϊȫ��
    }
}

