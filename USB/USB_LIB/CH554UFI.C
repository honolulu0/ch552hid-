/* 2014.09.09
*****************************************
**   Copyright  (C)  W.ch  1999-2015   **
**   Web:      http://wch.cn           **
*****************************************
**  USB-flash File Interface for CH554 **
**  KC7.0@MCS51                        **
*****************************************
*/
/* CH554 U�������ļ�ϵͳ�ӿ�, ֧��: FAT12/FAT16/FAT32 */

//#define DISK_BASE_BUF_LEN		512	/* Ĭ�ϵĴ������ݻ�������СΪ512�ֽ�,Ϊ0���ֹ�ڱ��ļ��ж��建��������Ӧ�ó�����pDISK_BASE_BUF��ָ�� */
/* �����Ҫ���ô������ݻ������Խ�ԼRAM,��ô�ɽ�DISK_BASE_BUF_LEN����Ϊ0�Խ�ֹ�ڱ��ļ��ж��建����,����Ӧ�ó����ڵ���CH554LibInit֮ǰ��������������õĻ�������ʼ��ַ����pDISK_BASE_BUF���� */

//#define NO_DEFAULT_ACCESS_SECTOR	1		/* ��ֹĬ�ϵĴ���������д�ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_DISK_CONNECT		1		/* ��ֹĬ�ϵļ����������ӳ���,���������б�д�ĳ�������� */
//#define NO_DEFAULT_FILE_ENUMER		1		/* ��ֹĬ�ϵ��ļ���ö�ٻص�����,���������б�д�ĳ�������� */

//#include "CH554.H"
#include "CH554UFI.H"

CMD_PARAM_I	mCmdParam;						/* ������� */
#if		DISK_BASE_BUF_LEN > 0
UINT8X	DISK_BASE_BUF[ DISK_BASE_BUF_LEN ] _at_ 0x0080;	/* �ⲿRAM�Ĵ������ݻ�����,����������Ϊһ�������ĳ��� */
#endif

/* ���³�����Ը�����Ҫ�޸� */

#ifndef	NO_DEFAULT_ACCESS_SECTOR		/* ��Ӧ�ó����ж���NO_DEFAULT_ACCESS_SECTOR���Խ�ֹĬ�ϵĴ���������д�ӳ���,Ȼ�������б�д�ĳ�������� */
//if ( use_external_interface ) {  // �滻U�������ײ��д�ӳ���
//    CH554DiskStatus=DISK_MOUNTED;  // ǿ�ƿ��豸���ӳɹ�(ֻ������ļ�ϵͳ)
//}

#pragma ot(8,size)

UINT8	CH554ReadSector( UINT8 SectCount, PUINT8X DataBuf )  /* �Ӵ��̶�ȡ������������ݵ��������� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extReadSector( CH554vLbaCurrent, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		pCBW -> mCBW_DataLen0 = 0;  /* ���ݴ��䳤�� */
		pCBW -> mCBW_DataLen1 = SectCount << CH554vSectorSizeB - 8;
		pCBW -> mCBW_DataLen2 = 0;
		pCBW -> mCBW_DataLen3 = 0;
		pCBW -> mCBW_Flag = 0x80;
		pCBW -> mCBW_LUN = CH554vCurrentLun;
		pCBW -> mCBW_CB_Len = 10;
		pCBW -> mCBW_CB_Buf[ 0 ] = SPC_CMD_READ10;
		pCBW -> mCBW_CB_Buf[ 1 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 2 ] = *(PUINT8)&CH554vLbaCurrent;
		pCBW -> mCBW_CB_Buf[ 3 ] = *( (PUINT8)&CH554vLbaCurrent + 1 );
		pCBW -> mCBW_CB_Buf[ 4 ] = *( (PUINT8)&CH554vLbaCurrent + 2 );
		pCBW -> mCBW_CB_Buf[ 5 ] = *( (PUINT8)&CH554vLbaCurrent + 3 );
		pCBW -> mCBW_CB_Buf[ 6 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 7 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 8 ] = SectCount;
		pCBW -> mCBW_CB_Buf[ 9 ] = 0x00;
		CH554BulkOnlyCmd( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CH554IntStatus == ERR_SUCCESS ) {
			return( ERR_SUCCESS );
		}
		CH554IntStatus = CH554AnalyzeError( retry );
		if ( CH554IntStatus != ERR_SUCCESS ) {
			return( CH554IntStatus );
		}
	}
	return( CH554IntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}

#ifdef	EN_DISK_WRITE
UINT8	CH554WriteSector( UINT8 SectCount, PUINT8X DataBuf )  /* ���������еĶ�����������ݿ�д����� */
{
	UINT8	retry;
//	if ( use_external_interface ) return( extWriteSector( CH554vLbaCurrent, SectCount, DataBuf ) );  /* �ⲿ�ӿ� */
	for( retry = 0; retry < 3; retry ++ ) {  /* �������� */
		pCBW -> mCBW_DataLen0 = 0;  /* ���ݴ��䳤�� */
		pCBW -> mCBW_DataLen1 = SectCount << CH554vSectorSizeB - 8;
		pCBW -> mCBW_DataLen2 = 0;
		pCBW -> mCBW_DataLen3 = 0;
		pCBW -> mCBW_Flag = 0x00;
		pCBW -> mCBW_LUN = CH554vCurrentLun;
		pCBW -> mCBW_CB_Len = 10;
		pCBW -> mCBW_CB_Buf[ 0 ] = SPC_CMD_WRITE10;
		pCBW -> mCBW_CB_Buf[ 1 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 2 ] = *(PUINT8)&CH554vLbaCurrent;
		pCBW -> mCBW_CB_Buf[ 3 ] = *( (PUINT8)&CH554vLbaCurrent + 1 );
		pCBW -> mCBW_CB_Buf[ 4 ] = *( (PUINT8)&CH554vLbaCurrent + 2 );
		pCBW -> mCBW_CB_Buf[ 5 ] = *( (PUINT8)&CH554vLbaCurrent + 3 );
		pCBW -> mCBW_CB_Buf[ 6 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 7 ] = 0x00;
		pCBW -> mCBW_CB_Buf[ 8 ] = SectCount;
		pCBW -> mCBW_CB_Buf[ 9 ] = 0x00;
		CH554BulkOnlyCmd( DataBuf );  /* ִ�л���BulkOnlyЭ������� */
		if ( CH554IntStatus == ERR_SUCCESS ) {
			mDelayuS( 200 );  /* д��������ʱ */
			return( ERR_SUCCESS );
		}
		CH554IntStatus = CH554AnalyzeError( retry );
		if ( CH554IntStatus != ERR_SUCCESS ) {
			return( CH554IntStatus );
		}
	}
	return( CH554IntStatus = ERR_USB_DISK_ERR );  /* ���̲������� */
}
#endif
#endif  // NO_DEFAULT_ACCESS_SECTOR

#ifndef	NO_DEFAULT_DISK_CONNECT			/* ��Ӧ�ó����ж���NO_DEFAULT_DISK_CONNECT���Խ�ֹĬ�ϵļ����������ӳ���,Ȼ�������б�д�ĳ�������� */
/* �������Ƿ����� */
UINT8	CH554DiskConnect( void )
{
	USB_DEV_AD &= 0x7F;
	if ( USB_DEV_AD == USB_DEVICE_ADDR ) {  /* ����USB�豸 */
		if ( UHOST_CTRL & bUH_PORT_EN ) {  /* USB�豸������δ��� */
			return( ERR_SUCCESS );  /* USB�豸�Ѿ�������δ��� */
		}
		else if ( USB_MIS_ST & bUMS_DEV_ATTACH ) {  /* USB�豸���� */
mDiskConnect:
			CH554DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
			return( ERR_SUCCESS );  /* �ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
		}
		else {  /* USB�豸�Ͽ� */
mDiskDisconn:
			CH554DiskStatus = DISK_DISCONNECT;
			return( ERR_USB_DISCON );
		}
	}
#ifndef	FOR_ROOT_UDISK_ONLY
	else if ( USB_DEV_AD > 0x10 && USB_DEV_AD <= 0x14 ) {  /* �ⲿHUB�Ķ˿��µ�USB�豸 */
		if ( UHOST_CTRL & bUH_PORT_EN ) {  /* �ⲿHUB������δ��� */
			TxBuffer[ MAX_PACKET_SIZE - 1 ] = USB_DEV_AD;  /* ���� */
			USB_DEV_AD = USB_DEVICE_ADDR - 1 + ( USB_DEV_AD >> 4 );  /* ����USB�����˵�USB��ַָ��HUB */
			CH554IntStatus = HubGetPortStatus( TxBuffer[ MAX_PACKET_SIZE - 1 ] & 0x0F );  /* ��ѯHUB�˿�״̬,������TxBuffer�� */
			if ( CH554IntStatus == ERR_SUCCESS ) {
				if ( TxBuffer[2] & (1<<(HUB_C_PORT_CONNECTION-0x10)) ) {  /* ��⵽HUB�˿��ϵĲ���¼� */
					CH554DiskStatus = DISK_DISCONNECT;  /* �ٶ�ΪHUB�˿��ϵ�USB�豸�Ͽ� */
					HubClearPortFeature( TxBuffer[ MAX_PACKET_SIZE - 1 ] & 0x0F, HUB_C_PORT_CONNECTION );  /* ���HUB�˿������¼�״̬ */
				}
				USB_DEV_AD = TxBuffer[ MAX_PACKET_SIZE - 1 ];  /* ����USB�����˵�USB��ַָ��USB�豸 */
				if ( TxBuffer[0] & (1<<HUB_PORT_CONNECTION) ) {  /* ����״̬ */
					if ( CH554DiskStatus < DISK_CONNECT ) {
						CH554DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
					}
					return( ERR_SUCCESS );  /* USB�豸�Ѿ����ӻ��߶Ͽ����������� */
				}
				else {
//					CH554DiskStatus = DISK_DISCONNECT;
//					return( ERR_USB_DISCON );
					CH554DiskStatus = DISK_CONNECT;
					return( ERR_HUB_PORT_FREE );  /* HUB�Ѿ����ӵ���HUB�˿���δ���Ӵ��� */
				}
			}
			else {
				USB_DEV_AD = TxBuffer[ MAX_PACKET_SIZE - 1 ];  /* ����USB�����˵�USB��ַָ��USB�豸 */
				if ( CH554IntStatus == ERR_USB_DISCON ) {
//					CH554DiskStatus = DISK_DISCONNECT;
//					return( ERR_USB_DISCON );
					goto mDiskDisconn;
				}
				else {
					CH554DiskStatus = DISK_CONNECT;  /* HUB����ʧ�� */
					return( CH554IntStatus );
				}
			}
		}
		else if ( USB_MIS_ST & bUMS_DEV_ATTACH ) {  /* USB�豸����,�ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
//			CH554DiskStatus = DISK_CONNECT;  /* �����Ͽ��� */
//			return( ERR_SUCCESS );  /* �ⲿHUB��USB�豸�Ѿ����ӻ��߶Ͽ����������� */
			goto mDiskConnect;
		}
		else {  /* �ⲿHUB�Ͽ� */
			CH554DiskStatus = DISK_DISCONNECT;
		}
	}
#endif
	else {
//		CH554DiskStatus = DISK_DISCONNECT;
//		return( ERR_USB_DISCON );
		goto mDiskDisconn;
	}
}
#endif  // NO_DEFAULT_DISK_CONNECT

#pragma ot(8,speed)

#ifndef	NO_DEFAULT_FILE_ENUMER			/* ��Ӧ�ó����ж���NO_DEFAULT_FILE_ENUMER���Խ�ֹĬ�ϵ��ļ���ö�ٻص�����,Ȼ�������б�д�ĳ�������� */
void xFileNameEnumer( void )			/* �ļ���ö�ٻص��ӳ��� */
{
/* ���ָ��ö�����CH554vFileSizeΪ0xFFFFFFFF�����FileOpen����ôÿ������һ���ļ�FileOpen������ñ��ص�����
   �ص�����xFileNameEnumer���غ�FileOpen�ݼ�CH554vFileSize������ö��ֱ�����������ļ�����Ŀ¼�����������ǣ�
   �ڵ���FileOpen֮ǰ����һ��ȫ�ֱ���Ϊ0����FileOpen�ص�������󣬱�������CH554vFdtOffset�õ��ṹFAT_DIR_INFO��
   �����ṹ�е�DIR_Attr�Լ�DIR_Name�ж��Ƿ�Ϊ�����ļ�������Ŀ¼������¼�����Ϣ������ȫ�ֱ�������������
   ��FileOpen���غ��жϷ���ֵ�����ERR_MISS_FILE��ERR_FOUND_NAME����Ϊ�����ɹ���ȫ�ֱ���Ϊ����������Ч�ļ�����
   ����ڱ��ص�����xFileNameEnumer�н�CH554vFileSize��Ϊ1����ô����֪ͨFileOpen��ǰ���������������ǻص��������� */
#if		0
	UINT8			i;
	UINT16			FileCount;
	PX_FAT_DIR_INFO	pFileDir;
	PUINT8			NameBuf;
	pFileDir = (PX_FAT_DIR_INFO)( pDISK_BASE_BUF + CH554vFdtOffset );  /* ��ǰFDT����ʼ��ַ */
	FileCount = (UINT16)( 0xFFFFFFFF - CH554vFileSize );  /* ��ǰ�ļ�����ö�����,CH554vFileSize��ֵ��0xFFFFFFFF,�ҵ��ļ�����ݼ� */
	if ( FileCount < sizeof( FILE_DATA_BUF ) / 12 ) {  /* ��黺�����Ƿ��㹻���,�ٶ�ÿ���ļ�����ռ��12���ֽڴ�� */
		NameBuf = & FILE_DATA_BUF[ FileCount * 12 ];  /* ���㱣�浱ǰ�ļ����Ļ�������ַ */
		for ( i = 0; i < 11; i ++ ) NameBuf[ i ] = pFileDir -> DIR_Name[ i ];  /* �����ļ���,����Ϊ11���ַ�,δ�����ո� */
//		if ( pFileDir -> DIR_Attr & ATTR_DIRECTORY ) NameBuf[ i ] = 1;  /* �ж���Ŀ¼�� */
		NameBuf[ i ] = 0;  /* �ļ��������� */
	}
#endif
}
#endif  // NO_DEFAULT_FILE_ENUMER

#pragma ot(8,size)

UINT8	CH554LibInit( void )  /* ��ʼ��CH554�����,�����ɹ�����0 */
{
	if ( CH554GetVer( ) < CH554_LIB_VER ) return( 0xFF );  /* ��ȡ��ǰ�ӳ����İ汾��,�汾̫���򷵻ش��� */
#if		DISK_BASE_BUF_LEN > 0
	pDISK_BASE_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ������ݻ����� */
	pDISK_FAT_BUF = & DISK_BASE_BUF[0];  /* ָ���ⲿRAM�Ĵ���FAT���ݻ�����,������pDISK_BASE_BUF�����Խ�ԼRAM */
/* ���ϣ������ļ���ȡ�ٶ�,��ô�������������е���CH554LibInit֮��,��pDISK_FAT_BUF����ָ����һ�������������pDISK_BASE_BUFͬ����С�Ļ����� */
#endif
	CH554DiskStatus = DISK_UNKNOWN;  /* δ֪״̬ */
	CH554vStartLba = 0;  /* Ĭ��Ϊ�Զ�����FDD��HDD */
	return( ERR_SUCCESS );
}