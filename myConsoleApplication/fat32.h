#pragma once

#pragma pack(1)
#pragma warning(disable : 4996) 

#include "stdafx.h"

/*


��FAT32�����Ľṹ��

��[��������,���е�һ������ΪDBR]||||[FAT1]||||[FAT2]||||[������]��



*/

//fat32���
typedef struct _FAT32_Sector
{
	UINT32	   Sectors_per_FAT_FAT32;						//FAT32��FAT��ռ����������
	UINT16 Extend_Flag;									//0-3λ��ʾ�FAT����7λ��0��ʾ������ʱFATӳ�䵽����FAT
																	//1��ʾֻ��һ��FAT�ǻ�ģ�����λ����
	UINT16 FS_Version;									//�ļ�ϵͳ�汾�����ֽڱ�ʾ��Ҫ�޶��ţ���ֱ�ӱ�ʾ��Ҫ�޶���
	UINT32	   Root_Cluster_Number;							//��Ŀ¼�غţ�һ��ȡֵΪ2 
	UINT16 FS_Info_Sector;								//�ļ�ϵͳ�����ţ�һ��ȡ1
	UINT16 Backup_Sector;								//��������������һ��ȡֵΪ6
	BYTE	   Reserved_Sector[12];		//��������	
}FAT32_Sector, *pFAT32_Sector;
typedef struct _Basic_BPB
{
	UINT16	Bytes_per_Sector;					//ÿ�������ֽ�������ȡminReadSize��1024��2048��4096��ͨ��ȡminReadSize
	BYTE		Sectors_per_Cluster;				//ÿ������������ȡ1��2��4��8��16��32��64��128��FAT32������
													//268,435,445����
	UINT16	Reserved_Sector;					//������������ʾ��һ��FATǰ��������  ��Ҳ����FAT1��ƫ�ơ�
	BYTE		FATs;								//FAT�������ͨ��ȡ2
	UINT16	RootEntry;							//��Ŀ¼��������FAT32��ȡֵ��Ϊ0
	UINT16	SmallSector;						//С����������FAT32��ȡֵ��Ϊ0
	BYTE		Media;								//�洢����������F8��ʾӲ�̣�F0��ʾ3.5����
	UINT16	Sector_per_FAT_FAT16;				//���FAT12/16��ÿ��FAT����������FAT32��ȡֵΪ0
	UINT16	Sector_per_Track;					//ÿ����������������������ṹ
	UINT16	Heads;								//��ͷ��
	UINT32		Hidden_Sector;						//�ÿ�Ӳ��ǰ���ڴ���������뼰�������������  ������������
	UINT32		Large_Sector;						//������������SmallSectorΪ0���˴���ʾ��������������  
															//���������� = ��������-��������-FAT��ռ������
	_FAT32_Sector		Fat32_Sector;						//FAT32�ļ�ϵͳ������Ϣ				
}Basic_BPB, *pBasic_BPB;
typedef	struct _FAT32_Extend_BPB
{
	BYTE Physical_Drive;							//�����������ţ�0x80��ʾ����Ӳ�̣�0x00��ʾ����������
	BYTE Reserved;									//����
	BYTE Extend_Singure;							//0x28��0x29�Թ�Windows NTʶ��
	UINT32  Vol_Serial;								//�����кţ��ɸ�ʽ��ʱ������
	BYTE Vol_Label[11];			//���ʾ
	BYTE System_ID[8];				//ϵͳID�����ݸ�ʽ���ĸ�ʽΪFAT32��FAT16��
}FAT32_Extend_BPB, *pFAT32_Extend_BPB;

//FAT32 DBR����
typedef struct _FAT32_DBR
{
	BYTE		JumpInstrction[3];		//0x00,��תָ�ͨ��ΪEB 58 90������58ָʾ��,��תλ�ã���X86�У�58+2�ʹ�����ת��5A��
	BYTE		OEMID[8];				//0x03�����̱�ʾ��OS�汾��Ϣ
	_Basic_BPB			BPB;								//0x0B
	_FAT32_Extend_BPB	Extend_BPB;							//0x40
	BYTE Boot_Strap[420];				//�ļ�ϵͳ��������							//0x5A������������
	BYTE endSignature[2];					//0x01FE,������ʾ
}FAT32_DBR, *pFAT32_DBR;


