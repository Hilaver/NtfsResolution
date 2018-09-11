#pragma once

#pragma pack(1)
#pragma warning(disable : 4996) 

#include "stdafx.h"



#define MBRlen 446
#define minReadSize 512
#define MFTEntrySize 1024
#define SectorSize 512
#define DBRSectorSize 512
#define EBRSectorSize 512
#define MBRSectorSize 512
#define MFTEntryFlag 0x454C4946 //FILE




//DPT表项
typedef struct _PartTableEntry {
	BYTE bootSignature;//引导标志
	BYTE startHead;//CHS寻址方式，起始磁头
	BYTE startSector;//起始扇区，本字节低六位
	BYTE startCylinder;//起始磁道(柱面)，startSector高二位和本字节
	BYTE systemSignature;//分区类型标志
	BYTE endHead;//终止磁头
	BYTE endSector;//终止扇区
	BYTE endCylinder;//终止磁道
	unsigned int startSectorNo;//LBA寻址，起始扇区号
	unsigned int totalSectorsNum;//该分区扇区总数
}PartTableEntry, *pPartTableEntry;

//MBR扇区
typedef struct _MBRSector {
	BYTE MBR[MBRlen];
	PartTableEntry ptEntrys[4];
	BYTE endSignature[2];
}MBRSector, *pMBRSector;


//NTFS DBR扇区
typedef struct _NTFSDBR {
	BYTE JMP[3];	//跳转指令
	BYTE FsID[8];	//文件系统ID
	unsigned short int bytePerSector;	//每扇区字节数
	BYTE secPerCluster;		//每簇扇区数
	BYTE reservedBytes[2];	//2个保留字节
	BYTE zeroBytes[3];	//三个0字节
	BYTE unusedBytes1[2];	//2个未用字节
	BYTE mediaType;//媒体类型
	BYTE unusedBytes2[2];	//2个未用字节
	unsigned short int secPerTrack;	//每磁道扇区数
	unsigned short int Heads;	//磁头数
	unsigned int hideSectors;	//隐藏扇区数
	BYTE unusedBytes3[4];	//4个未用字节
	BYTE usedBytes[4];	//4个固定字节
	unsigned __int64 totalSectors;	//总扇区数
	unsigned __int64 MFT;	//MFT起始簇号
	unsigned __int64 MFTMirror;	//MFTMirror文件起始簇号
	char fileRecord;	//文件记录
	BYTE unusedBytes4[3];	//3个未用字节
	char indexSize;	//索引缓冲区大小
	BYTE unusedBytes5[3];	//未用字节
	BYTE volumeSerialID64[8];	//卷序列号
	unsigned int checkSum;	//校验和
	BYTE bootCode[426];	//引导代码
	BYTE endSignature[2];	//结束标志
}NTFSDBR, *pNTFSDBR;



//MFT表项的结构
// 文件记录头
typedef struct _FILE_RECORD_HEADER
{
	/*+0x00*/	BYTE Type[4];            // 固定值'FILE'
	/*+0x04*/	UINT16 USNOffset;        // 更新序列号偏移, 与操作系统有关
	/*+0x06*/	UINT16 USNCount;         // 固定列表大小Size in words of Update Sequence Number & Array (S)
	/*+0x08*/  UINT64 Lsn;               // 日志文件序列号(LSN)
	/*+0x10*/  UINT16  SequenceNumber;   // 序列号(用于记录文件被反复使用的次数)
	/*+0x12*/  UINT16  LinkCount;        // 硬连接数
	/*+0x14*/  UINT16  AttributeOffset;  // 第一个属性偏移
	/*+0x16*/  UINT16  Flags;            // flags, 00表示删除文件,01表示正常文件,02表示删除目录,03表示正常目录
	/*+0x18*/  UINT32  BytesInUse;       // 文件记录实时大小(字节) 当前MFT表项长度,到FFFFFF的长度+4
	/*+0x1C*/  UINT32  BytesAllocated;   // 文件记录分配大小(字节)
	/*+0x20*/  UINT64  BaseFileRecord;   // = 0 基础文件记录 File reference to the base FILE record
	/*+0x28*/  UINT16  NextAttributeNumber; // 下一个自由ID号
	/*+0x2A*/  UINT16  Pading;           // 边界
	/*+0x2C*/  UINT32  MFTRecordNumber;  // windows xp中使用,本MFT记录号
	/*+0x30*/  UINT16  USN;      // 更新序列号
	/*+0x32*/  BYTE  UpdateArray[0];      // 更新数组
} FILE_RECORD_HEADER, *pFILE_RECORD_HEADER;

//常驻属性和非常驻属性的公用部分
typedef struct _CommonAttributeHeader {
	UINT32 ATTR_Type; //属性类型
	UINT32 ATTR_Size; //属性头和属性体的总长度
	BYTE ATTR_ResFlag; //是否是常驻属性（0常驻 1非常驻）
	BYTE ATTR_NamSz; //属性名的长度
	UINT16 ATTR_NamOff; //属性名的偏移 相对于属性头
	UINT16 ATTR_Flags; //标志（0x0001压缩 0x4000加密 0x8000稀疏）
	UINT16 ATTR_Id; //属性唯一ID
}CommonAttributeHeader,*pCommonAttributeHeader;

//常驻属性 属性头
typedef struct _ResidentAttributeHeader {
	CommonAttributeHeader ATTR_Common;
	UINT32 ATTR_DatSz; //属性数据的长度
	UINT16 ATTR_DatOff; //属性数据相对于属性头的偏移
	BYTE ATTR_Indx; //索引
	BYTE ATTR_Resvd; //保留
	BYTE ATTR_AttrNam[0];//属性名，Unicode，结尾无0
}ResidentAttributeHeader, *pResidentAttributeHeader;

//非常驻属性 属性头
typedef struct _NonResidentAttributeHeader {
	CommonAttributeHeader ATTR_Common;
	UINT64 ATTR_StartVCN; //本属性中数据流起始虚拟簇号 
	UINT64 ATTR_EndVCN; //本属性中数据流终止虚拟簇号
	UINT16 ATTR_DatOff; //簇流列表相对于属性头的偏移
	UINT16 ATTR_CmpSz; //压缩单位 2的N次方
	UINT32 ATTR_Resvd;
	UINT64 ATTR_AllocSz; //属性分配的大小
	UINT64 ATTR_ValidSz; //属性的实际大小
	UINT64 ATTR_InitedSz; //属性的初始大小
	BYTE ATTR_AttrNam[0];
}NonResidentAttributeHeader, *pNonResidentAttributeHeader;

/*下面是索引结构的定义*/

//标准索引头的结构
typedef struct _STD_INDEX_HEADER {
	BYTE SIH_Flag[4];  //固定值 "INDX"
	UINT16 SIH_USNOffset;//更新序列号偏移
	UINT16 SIH_USNSize;//更新序列号和更新数组大小
	UINT64 SIH_Lsn;               // 日志文件序列号(LSN)
	UINT64 SIH_IndexCacheVCN;//本索引缓冲区在索引分配中的VCN
	UINT32 SIH_IndexEntryOffset;//索引项的偏移 相对于当前位置
	UINT32 SIH_IndexEntrySize;//索引项的大小
	UINT32 SIH_IndexEntryAllocSize;//索引项分配的大小
	UINT8 SIH_HasLeafNode;//置一 表示有子节点
	BYTE SIH_Fill[3];//填充
	UINT16 SIH_USN;//更新序列号
	BYTE SIH_USNArray[0];//更新序列数组
}STD_INDEX_HEADER,*pSTD_INDEX_HEADER;

//标准索引项的结构
typedef struct _STD_INDEX_ENTRY {
	UINT64 SIE_MFTReferNumber;//文件的MFT参考号
	UINT16 SIE_IndexEntrySize;//索引项的大小
	UINT16 SIE_FileNameAttriBodySize;//文件名属性体的大小
	UINT16 SIE_IndexFlag;//索引标志
	BYTE SIE_Fill[2];//填充
	UINT64 SIE_FatherDirMFTReferNumber;//父目录MFT文件参考号
	FILETIME SIE_CreatTime;//文件创建时间
	FILETIME SIE_AlterTime;//文件最后修改时间
	FILETIME SIE_MFTChgTime;//文件记录最后修改时间
	FILETIME SIE_ReadTime;//文件最后访问时间
	UINT64 SIE_FileAllocSize;//文件分配大小
	UINT64 SIE_FileRealSize;//文件实际大小
	UINT64 SIE_FileFlag;//文件标志
	UINT8 SIE_FileNameSize;//文件名长度
	UINT8 SIE_FileNamespace;//文件命名空间
	BYTE SIE_FileNameAndFill[0];//文件名和填充
}STD_INDEX_ENTRY,*pSTD_INDEX_ENTRY;



/****下面定义的均是属性体的结构 不包括属性头****/


//STANDARD_INFORMATION 0X10属性体
/*
SI_DOSAttr取值：
	0x0001    只读
	0x0002    隐藏
	0x0004    系统
	0x0020    归档
	0x0040    设备
	0x0080    常规
	0x0100    临时文件
	0x0200    稀疏文件
	0x0400    重解析点
	0x0800    压缩
	0x1000    离线
	0x2000    无内容索引
	0x4000    加密
*/
typedef struct _STANDARD_INFORMATION {
	FILETIME SI_CreatTime;//创建时间
	FILETIME SI_AlterTime;//最后修改时间
	FILETIME SI_MFTChgTime;//文件的MFT修改的时间
	FILETIME SI_ReadTime;//最后访问时间
	UINT32 SI_DOSAttr;//DOS文件属性
	UINT32 SI_MaxVer;//文件可用的最大版本号 0表示禁用
	UINT32 SI_Ver;//文件版本号 若最大版本号为0 则值为0
	UINT32 SI_ClassId;//??
	//UINT64 SI_OwnerId;//文件拥有者ID
	//UINT64 SI_SecurityId;//安全ID
	//UINT64 SI_QuotaCharged;//文件最大可使用的空间配额 0表示无限制
	//UINT64 SI_USN;//文件最后一次更新的记录号
#if 0  
	uint32 QuotaId;
	uint32 SecurityId;
	uint64 QuotaCharge;
	USN Usn;
#endif  
}STANDARD_INFORMATION,*pSTANDARD_INFORMATION;


//ATTRIBUTE_LIST 0X20属性体
typedef struct _ATTRIBUTE_LIST {
	UINT32 AL_RD_Type;
	UINT16 AL_RD_Len;
	BYTE AL_RD_NamLen;
	BYTE AL_RD_NamOff;
	UINT64 AL_RD_StartVCN;//本属性中数据流开始的簇号
	UINT64 AL_RD_BaseFRS;/*本属性记录所属的MFT记录的记录号
						  注意：该值的低6字节是MFT记录号，高2字节是该MFT记录的序列号*/
	UINT16 AL_RD_AttrId;
	//BYTE AL_RD_Name[0];
	UINT16 AlignmentOrReserved[3];
}ATTRIBUTE_LIST,*pATTRIBUTE_LIST;


//FILE_NAME 0X30属性体
typedef struct _FILE_NAME {
	UINT64 FN_ParentFR; /*父目录的MFT记录的记录索引。
							注意：该值的低6字节是MFT记录号，高2字节是该MFT记录的序列号*/
	FILETIME FN_CreatTime;
	FILETIME FN_AlterTime;
	FILETIME FN_MFTChg;
	FILETIME FN_ReadTime;
	UINT64 FN_AllocSz;
	UINT64 FN_ValidSz;//文件的真实尺寸
	UINT32 FN_DOSAttr;//DOS文件属性
	UINT32 FN_EA_Reparse;//扩展属性与链接
	BYTE FN_NameSz;//文件名的字符数
	BYTE FN_NamSpace;/*命名空间，该值可为以下值中的任意一个
						0：POSIX　可以使用除NULL和分隔符“/”之外的所有UNICODE字符，最大可以使用255个字符。注意：“：”是合法字符，但Windows不允许使用。
						1：Win32　Win32是POSIX的一个子集，不区分大小写，可以使用除““”、“＊”、“?”、“：”、“/”、“<”、“>”、“/”、“|”之外的任意UNICODE字符，但名字不能以“.”或空格结尾。
						2：DOS　DOS命名空间是Win32的子集，只支持ASCII码大于空格的8BIT大写字符并且不支持以下字符““”、“＊”、“?”、“：”、“/”、“<”、“>”、“/”、“|”、“+”、“,”、“;”、“=”；同时名字必须按以下格式命名：1~8个字符，然后是“.”，然后再是1~3个字符。
						3：Win32&DOS　这个命名空间意味着Win32和DOS文件名都存放在同一个文件名属性中。*/
	BYTE FN_FileName[0];
}FILE_NAME,*pFILE_NAME;


//VOLUME_VERSION 
typedef struct _VOLUME_VERSION {
	//??
}VOLUME_VERSION,*pVOLUME_VERSION;


//OBJECT_ID 0X40属性体
typedef struct _OBJECT_ID {
	BYTE OID_ObjID[16];//文件的GUID
	BYTE OID_BirthVolID[16];//文件建立时所在卷的ID
	BYTE OID_BirthID[16];//文件的原始ID
	BYTE OID_DomainID[16];//对象所创建时所在域的ID
}OBJECT_ID, *pOBJECT_ID;


//SECRUITY_DESCRIPTOR 0X50属性体
typedef struct _SECRUITY_DESCRIPTOR {
	//??
}SECRUITY_DESCRIPTOR,*pSECRUITY_DESCRIPTOR;


//VOLUME_NAME 0X60属性体
typedef struct _VOLUME_NAME {
	BYTE VN_Name[0];
}VOLUME_NAME,*pVOLUME_NAME;


//VOLUME_INFORMATION 0X70属性体
typedef struct _VOLUME_INFORMATION{
	UINT64 VI_Resvd;
	BYTE VI_MajVer;//卷主版本号
	BYTE VI_MinVer;//卷子版本号
	UINT16 VI_Flags;/*标志位，可以是以下各值组合
					0x0001    脏位，当该值被设置时Windows将会在下次启动时运行chkdsk/F命令。
					0x0002    日志文件改变尺寸
					0x0004    卷挂接时升级
					0x0008    由Windows NT 4挂接
					0x0010    启动时删除USN
					0x0020    修复过的ID
					0x8000    被chkdsk修改过*/
}VOLUME_INFORMATION,*pVOLUME_INFORMATION;


//DATA 0X80属性体
typedef struct _DATA {
	//??
	///*+0x10*/   UINT64 StartVcn;     // LowVcn 起始VCN  起始簇号  
	///*+0x18*/   UINT64 LastVcn;      // HighVcn  结束VCN  结束簇号  
	///*+0x20*/   UINT16 RunArrayOffset;    // 数据运行的偏移  
	///*+0x22*/   UINT16 CompressionUnit;   // 压缩引擎  
	///*+0x24*/   UINT32  Padding0;       // 填充  
	///*+0x28*/   UINT32  IndexedFlag;    // 为属性值分配大小(按分配的簇的字节数计算)  
	///*+0x30*/   UINT64 AllocatedSize;   // 属性值实际大小  
	///*+0x38*/   UINT64 DataSize;     // 属性值压缩大小  
	///*+0x40*/   UINT64 InitializedSize;   // 实际数据大小  
	///*+0x48*/   UINT64 CompressedSize;    // 压缩后大小 
	BYTE D_data[0];
}DATA,*pDATA;



typedef struct _INDEX_ENTRY {
	UINT64 IE_MftReferNumber;/*该文件的MFT参考号。注意：该值的低6字节是MFT记录号，高2字节是该MFT记录的序列号*/
	UINT16 IE_Size;//索引项的大小 相对于索引项开始的偏移量
	UINT16 IE_FileNAmeAttriBodySize;//文件名属性体的大小
	UINT16 IE_Flags;/*标志。该值可能是以下值之一：
					0x00       普通文件项
					0x01       有子项
					0x02       当前项是最后一个目录项
					在读取索引项数据时应该首先检查该成员的值以确定当前项的类型*/
	UINT16 IE_Fill;//填充 无意义
	UINT64 IE_FatherDirMftReferNumber;//父目录的MFT文件参考号
	FILETIME IE_CreatTime;//文件创建时间
	FILETIME IE_AlterTime;//文件最后修改时间
	FILETIME IE_MFTChgTime;//文件记录最后修改时间
	FILETIME IE_ReadTime;//文件最后访问时间

	UINT64 IE_FileAllocSize;//文件分配大小
	UINT64 IE_FileRealSize;//文件实际大小
	UINT64 IE_FileFlag;//文件标志
	UINT8 IE_FileNameSize;//文件名长度
	UINT8 IE_FileNamespace;//文件命名空间
	BYTE IE_FileNameAndFill[0];//文件名和填充
							   //BYTE IE_Stream[0];//目录项数据，结构与文件名属性的数据相同
							   //UINT64 IE_SubNodeFR;//子项的记录索引。该值的低6字节是MFT记录号，高2字节是该MFT记录的序列号
}INDEX_ENTRY,*pINDEX_ENTRY;

typedef struct _INDEX_HEADER {
	UINT32 IH_EntryOff;//第一个目录项的偏移
	UINT32 IH_TalSzOfEntries;//目录项的总尺寸(包括索引头和下面的索引项)
	UINT32 IH_AllocSize;//目录项分配的尺寸
	BYTE IH_Flags;/*标志位，此值可能是以下和值之一：
				  0x00       小目录(数据存放在根节点的数据区中)
				  0x01       大目录(需要目录项存储区和索引项位图)*/
	BYTE IH_Resvd[3];
}INDEX_HEADER,*pINDEX_HEADER;

//INDEX_ROOT 0X90属性体
typedef struct _INDEX_ROOT {
	//索引根
	UINT32 IR_AttrType;//属性的类型
	UINT32 IR_ColRule;//整理规则
	UINT32 IR_EntrySz;//目录项分配尺寸
	BYTE IR_ClusPerRec;//每个目录项占用的簇数
	BYTE IR_Resvd[3];
	//索引头
	INDEX_HEADER IH;
	//索引项  可能不存在
	BYTE IR_IndexEntry[0];
}INDEX_ROOT,*pINDEX_ROOT;

//INDEX_ALLOCATION 0XA0属性体
typedef struct _INDEX_ALLOCATION {
	//UINT64 IA_DataRuns;
	BYTE IA_DataRuns[0];
}INDEX_ALLOCATION,*pINDEX_ALLOCATION;

//BITMAP
typedef struct _MFT_ATTR_BITMAP {
	//??
}MFT_ATTR_BITMAP,*pMFT_ATTR_BITMAP;

//SYMBOL_LINK
typedef struct _SYMBOL_LINK {
	//??
}SYMBOL_LINK,*pSYMBOL_LINK;

//REPARSE_POINT
typedef struct _REPARSE_POINT{
	UINT32 RP_Type;/*重解析数据类型，该值可以是以下值之一
					0x20000000    别名
					0x40000000    最高等待时间
					0x80000000    微软使用
					0x68000005    NSS
					0x68000006    NSS恢复
					0x68000007    SIS
					0x68000008    DFS
					0x88000003    卷挂接点
					0xA8000004   HSM
					0xE8000000   硬连接*/
	UINT16 RP_DatSz;//重解析数据尺寸
	UINT16 RP_Resvd;//
	BYTE RP_Data[0];//	重解析数据
}REPARSE_POINT,*pREPARSE_POINT;

//EA_INFORMATION
typedef struct _EA_INFORMATION {
	UINT16 EI_PackedSz;//	压缩扩展属性尺寸
	UINT16 EI_NumOfEA;//拥有NEED_EA记录的扩展属性个数
	UINT32 EI_UnpackedSz;//未压缩扩展属性尺寸
}EA_INFORMATION,*pEA_INFORMATION;

//EA
typedef struct _EA {
	UINT32 EA_Next;//下一个扩展属性的偏移(本记录的尺寸)
	BYTE EA_Flags;//标志位，值取0x80表示需要EA
	BYTE EA_NamLen;//名字数据的长度(M)
	UINT16 EA_ValLen;//值数据的长度
	BYTE EA_NameVal[0];//名字数据和值数据
}EA,*pEA;

//PROPERTY_SET
typedef struct _PROPERTY_SET {
	//??
}PROPERTY_SET,*pPROPERTY_SET;

//LOGGED_UNTILITY_STREAM
typedef struct _LOGGED_UNTILITY_STREAM {
	//??
}LOGGED_UNTILITY_STREAM,*pLOGGED_UNTILITY_STREAM;