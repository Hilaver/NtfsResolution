#pragma once

#pragma pack(1)
#pragma warning(disable : 4996) 

#include "stdafx.h"

/*


【FAT32分区的结构】

【[保留扇区,其中第一个扇区为DBR]||||[FAT1]||||[FAT2]||||[数据区]】



*/

//fat32相关
typedef struct _FAT32_Sector
{
	UINT32	   Sectors_per_FAT_FAT32;						//FAT32中FAT表占用总扇区数
	UINT16 Extend_Flag;									//0-3位表示活动FAT数，7位：0表示在运行时FAT映射到所有FAT
																	//1表示只有一个FAT是活动的，其他位保留
	UINT16 FS_Version;									//文件系统版本，高字节表示主要修订号，低直接表示次要修订号
	UINT32	   Root_Cluster_Number;							//根目录簇号，一般取值为2 
	UINT16 FS_Info_Sector;								//文件系统扇区号，一般取1
	UINT16 Backup_Sector;								//备份引导扇区，一般取值为6
	BYTE	   Reserved_Sector[12];		//保留扇区	
}FAT32_Sector, *pFAT32_Sector;
typedef struct _Basic_BPB
{
	UINT16	Bytes_per_Sector;					//每个扇区字节数，可取minReadSize，1024，2048，4096，通常取minReadSize
	BYTE		Sectors_per_Cluster;				//每簇扇区数，可取1，2，4，8，16，32，64，128，FAT32最多跟踪
													//268,435,445个簇
	UINT16	Reserved_Sector;					//保留扇区，表示第一个FAT前的扇区数  【也就是FAT1的偏移】
	BYTE		FATs;								//FAT表个数，通常取2
	UINT16	RootEntry;							//根目录项数，对FAT32，取值必为0
	UINT16	SmallSector;						//小扇区数，对FAT32，取值必为0
	BYTE		Media;								//存储介质描述，F8表示硬盘，F0表示3.5软盘
	UINT16	Sector_per_FAT_FAT16;				//针对FAT12/16的每个FAT扇区数，对FAT32，取值为0
	UINT16	Sector_per_Track;					//每道扇区数，描述磁盘物理结构
	UINT16	Heads;								//磁头数
	UINT32		Hidden_Sector;						//该块硬盘前用于存放引导代码及分区表的扇区数  【隐藏扇区】
	UINT32		Large_Sector;						//总扇区数，若SmallSector为0，此处表示分区上扇区总数  
															//可用扇区数 = 总扇区数-保留扇区-FAT表占用扇区
	_FAT32_Sector		Fat32_Sector;						//FAT32文件系统扇区信息				
}Basic_BPB, *pBasic_BPB;
typedef	struct _FAT32_Extend_BPB
{
	BYTE Physical_Drive;							//物理驱动器号，0x80表示物理硬盘，0x00表示软盘驱动器
	BYTE Reserved;									//保留
	BYTE Extend_Singure;							//0x28或0x29以供Windows NT识别
	UINT32  Vol_Serial;								//卷序列号，由格式化时随机获得
	BYTE Vol_Label[11];			//卷标示
	BYTE System_ID[8];				//系统ID，根据格式化的格式为FAT32，FAT16等
}FAT32_Extend_BPB, *pFAT32_Extend_BPB;

//FAT32 DBR扇区
typedef struct _FAT32_DBR
{
	BYTE		JumpInstrction[3];		//0x00,跳转指令，通常为EB 58 90，其中58指示了,跳转位置，在X86中，58+2就代表跳转到5A处
	BYTE		OEMID[8];				//0x03，厂商标示和OS版本信息
	_Basic_BPB			BPB;								//0x0B
	_FAT32_Extend_BPB	Extend_BPB;							//0x40
	BYTE Boot_Strap[420];				//文件系统引导代码							//0x5A，引导区代码
	BYTE endSignature[2];					//0x01FE,结束标示
}FAT32_DBR, *pFAT32_DBR;


