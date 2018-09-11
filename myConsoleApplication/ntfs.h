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




//DPT����
typedef struct _PartTableEntry {
	BYTE bootSignature;//������־
	BYTE startHead;//CHSѰַ��ʽ����ʼ��ͷ
	BYTE startSector;//��ʼ���������ֽڵ���λ
	BYTE startCylinder;//��ʼ�ŵ�(����)��startSector�߶�λ�ͱ��ֽ�
	BYTE systemSignature;//�������ͱ�־
	BYTE endHead;//��ֹ��ͷ
	BYTE endSector;//��ֹ����
	BYTE endCylinder;//��ֹ�ŵ�
	unsigned int startSectorNo;//LBAѰַ����ʼ������
	unsigned int totalSectorsNum;//�÷�����������
}PartTableEntry, *pPartTableEntry;

//MBR����
typedef struct _MBRSector {
	BYTE MBR[MBRlen];
	PartTableEntry ptEntrys[4];
	BYTE endSignature[2];
}MBRSector, *pMBRSector;


//NTFS DBR����
typedef struct _NTFSDBR {
	BYTE JMP[3];	//��תָ��
	BYTE FsID[8];	//�ļ�ϵͳID
	unsigned short int bytePerSector;	//ÿ�����ֽ���
	BYTE secPerCluster;		//ÿ��������
	BYTE reservedBytes[2];	//2�������ֽ�
	BYTE zeroBytes[3];	//����0�ֽ�
	BYTE unusedBytes1[2];	//2��δ���ֽ�
	BYTE mediaType;//ý������
	BYTE unusedBytes2[2];	//2��δ���ֽ�
	unsigned short int secPerTrack;	//ÿ�ŵ�������
	unsigned short int Heads;	//��ͷ��
	unsigned int hideSectors;	//����������
	BYTE unusedBytes3[4];	//4��δ���ֽ�
	BYTE usedBytes[4];	//4���̶��ֽ�
	unsigned __int64 totalSectors;	//��������
	unsigned __int64 MFT;	//MFT��ʼ�غ�
	unsigned __int64 MFTMirror;	//MFTMirror�ļ���ʼ�غ�
	char fileRecord;	//�ļ���¼
	BYTE unusedBytes4[3];	//3��δ���ֽ�
	char indexSize;	//������������С
	BYTE unusedBytes5[3];	//δ���ֽ�
	BYTE volumeSerialID64[8];	//�����к�
	unsigned int checkSum;	//У���
	BYTE bootCode[426];	//��������
	BYTE endSignature[2];	//������־
}NTFSDBR, *pNTFSDBR;



//MFT����Ľṹ
// �ļ���¼ͷ
typedef struct _FILE_RECORD_HEADER
{
	/*+0x00*/	BYTE Type[4];            // �̶�ֵ'FILE'
	/*+0x04*/	UINT16 USNOffset;        // �������к�ƫ��, �����ϵͳ�й�
	/*+0x06*/	UINT16 USNCount;         // �̶��б��СSize in words of Update Sequence Number & Array (S)
	/*+0x08*/  UINT64 Lsn;               // ��־�ļ����к�(LSN)
	/*+0x10*/  UINT16  SequenceNumber;   // ���к�(���ڼ�¼�ļ�������ʹ�õĴ���)
	/*+0x12*/  UINT16  LinkCount;        // Ӳ������
	/*+0x14*/  UINT16  AttributeOffset;  // ��һ������ƫ��
	/*+0x16*/  UINT16  Flags;            // flags, 00��ʾɾ���ļ�,01��ʾ�����ļ�,02��ʾɾ��Ŀ¼,03��ʾ����Ŀ¼
	/*+0x18*/  UINT32  BytesInUse;       // �ļ���¼ʵʱ��С(�ֽ�) ��ǰMFT�����,��FFFFFF�ĳ���+4
	/*+0x1C*/  UINT32  BytesAllocated;   // �ļ���¼�����С(�ֽ�)
	/*+0x20*/  UINT64  BaseFileRecord;   // = 0 �����ļ���¼ File reference to the base FILE record
	/*+0x28*/  UINT16  NextAttributeNumber; // ��һ������ID��
	/*+0x2A*/  UINT16  Pading;           // �߽�
	/*+0x2C*/  UINT32  MFTRecordNumber;  // windows xp��ʹ��,��MFT��¼��
	/*+0x30*/  UINT16  USN;      // �������к�
	/*+0x32*/  BYTE  UpdateArray[0];      // ��������
} FILE_RECORD_HEADER, *pFILE_RECORD_HEADER;

//��פ���Ժͷǳ�פ���ԵĹ��ò���
typedef struct _CommonAttributeHeader {
	UINT32 ATTR_Type; //��������
	UINT32 ATTR_Size; //����ͷ����������ܳ���
	BYTE ATTR_ResFlag; //�Ƿ��ǳ�פ���ԣ�0��פ 1�ǳ�פ��
	BYTE ATTR_NamSz; //�������ĳ���
	UINT16 ATTR_NamOff; //��������ƫ�� ���������ͷ
	UINT16 ATTR_Flags; //��־��0x0001ѹ�� 0x4000���� 0x8000ϡ�裩
	UINT16 ATTR_Id; //����ΨһID
}CommonAttributeHeader,*pCommonAttributeHeader;

//��פ���� ����ͷ
typedef struct _ResidentAttributeHeader {
	CommonAttributeHeader ATTR_Common;
	UINT32 ATTR_DatSz; //�������ݵĳ���
	UINT16 ATTR_DatOff; //�����������������ͷ��ƫ��
	BYTE ATTR_Indx; //����
	BYTE ATTR_Resvd; //����
	BYTE ATTR_AttrNam[0];//��������Unicode����β��0
}ResidentAttributeHeader, *pResidentAttributeHeader;

//�ǳ�פ���� ����ͷ
typedef struct _NonResidentAttributeHeader {
	CommonAttributeHeader ATTR_Common;
	UINT64 ATTR_StartVCN; //����������������ʼ����غ� 
	UINT64 ATTR_EndVCN; //����������������ֹ����غ�
	UINT16 ATTR_DatOff; //�����б����������ͷ��ƫ��
	UINT16 ATTR_CmpSz; //ѹ����λ 2��N�η�
	UINT32 ATTR_Resvd;
	UINT64 ATTR_AllocSz; //���Է���Ĵ�С
	UINT64 ATTR_ValidSz; //���Ե�ʵ�ʴ�С
	UINT64 ATTR_InitedSz; //���Եĳ�ʼ��С
	BYTE ATTR_AttrNam[0];
}NonResidentAttributeHeader, *pNonResidentAttributeHeader;

/*�����������ṹ�Ķ���*/

//��׼����ͷ�Ľṹ
typedef struct _STD_INDEX_HEADER {
	BYTE SIH_Flag[4];  //�̶�ֵ "INDX"
	UINT16 SIH_USNOffset;//�������к�ƫ��
	UINT16 SIH_USNSize;//�������кź͸��������С
	UINT64 SIH_Lsn;               // ��־�ļ����к�(LSN)
	UINT64 SIH_IndexCacheVCN;//�����������������������е�VCN
	UINT32 SIH_IndexEntryOffset;//�������ƫ�� ����ڵ�ǰλ��
	UINT32 SIH_IndexEntrySize;//������Ĵ�С
	UINT32 SIH_IndexEntryAllocSize;//���������Ĵ�С
	UINT8 SIH_HasLeafNode;//��һ ��ʾ���ӽڵ�
	BYTE SIH_Fill[3];//���
	UINT16 SIH_USN;//�������к�
	BYTE SIH_USNArray[0];//������������
}STD_INDEX_HEADER,*pSTD_INDEX_HEADER;

//��׼������Ľṹ
typedef struct _STD_INDEX_ENTRY {
	UINT64 SIE_MFTReferNumber;//�ļ���MFT�ο���
	UINT16 SIE_IndexEntrySize;//������Ĵ�С
	UINT16 SIE_FileNameAttriBodySize;//�ļ���������Ĵ�С
	UINT16 SIE_IndexFlag;//������־
	BYTE SIE_Fill[2];//���
	UINT64 SIE_FatherDirMFTReferNumber;//��Ŀ¼MFT�ļ��ο���
	FILETIME SIE_CreatTime;//�ļ�����ʱ��
	FILETIME SIE_AlterTime;//�ļ�����޸�ʱ��
	FILETIME SIE_MFTChgTime;//�ļ���¼����޸�ʱ��
	FILETIME SIE_ReadTime;//�ļ�������ʱ��
	UINT64 SIE_FileAllocSize;//�ļ������С
	UINT64 SIE_FileRealSize;//�ļ�ʵ�ʴ�С
	UINT64 SIE_FileFlag;//�ļ���־
	UINT8 SIE_FileNameSize;//�ļ�������
	UINT8 SIE_FileNamespace;//�ļ������ռ�
	BYTE SIE_FileNameAndFill[0];//�ļ��������
}STD_INDEX_ENTRY,*pSTD_INDEX_ENTRY;



/****���涨��ľ���������Ľṹ ����������ͷ****/


//STANDARD_INFORMATION 0X10������
/*
SI_DOSAttrȡֵ��
	0x0001    ֻ��
	0x0002    ����
	0x0004    ϵͳ
	0x0020    �鵵
	0x0040    �豸
	0x0080    ����
	0x0100    ��ʱ�ļ�
	0x0200    ϡ���ļ�
	0x0400    �ؽ�����
	0x0800    ѹ��
	0x1000    ����
	0x2000    ����������
	0x4000    ����
*/
typedef struct _STANDARD_INFORMATION {
	FILETIME SI_CreatTime;//����ʱ��
	FILETIME SI_AlterTime;//����޸�ʱ��
	FILETIME SI_MFTChgTime;//�ļ���MFT�޸ĵ�ʱ��
	FILETIME SI_ReadTime;//������ʱ��
	UINT32 SI_DOSAttr;//DOS�ļ�����
	UINT32 SI_MaxVer;//�ļ����õ����汾�� 0��ʾ����
	UINT32 SI_Ver;//�ļ��汾�� �����汾��Ϊ0 ��ֵΪ0
	UINT32 SI_ClassId;//??
	//UINT64 SI_OwnerId;//�ļ�ӵ����ID
	//UINT64 SI_SecurityId;//��ȫID
	//UINT64 SI_QuotaCharged;//�ļ�����ʹ�õĿռ���� 0��ʾ������
	//UINT64 SI_USN;//�ļ����һ�θ��µļ�¼��
#if 0  
	uint32 QuotaId;
	uint32 SecurityId;
	uint64 QuotaCharge;
	USN Usn;
#endif  
}STANDARD_INFORMATION,*pSTANDARD_INFORMATION;


//ATTRIBUTE_LIST 0X20������
typedef struct _ATTRIBUTE_LIST {
	UINT32 AL_RD_Type;
	UINT16 AL_RD_Len;
	BYTE AL_RD_NamLen;
	BYTE AL_RD_NamOff;
	UINT64 AL_RD_StartVCN;//����������������ʼ�Ĵغ�
	UINT64 AL_RD_BaseFRS;/*�����Լ�¼������MFT��¼�ļ�¼��
						  ע�⣺��ֵ�ĵ�6�ֽ���MFT��¼�ţ���2�ֽ��Ǹ�MFT��¼�����к�*/
	UINT16 AL_RD_AttrId;
	//BYTE AL_RD_Name[0];
	UINT16 AlignmentOrReserved[3];
}ATTRIBUTE_LIST,*pATTRIBUTE_LIST;


//FILE_NAME 0X30������
typedef struct _FILE_NAME {
	UINT64 FN_ParentFR; /*��Ŀ¼��MFT��¼�ļ�¼������
							ע�⣺��ֵ�ĵ�6�ֽ���MFT��¼�ţ���2�ֽ��Ǹ�MFT��¼�����к�*/
	FILETIME FN_CreatTime;
	FILETIME FN_AlterTime;
	FILETIME FN_MFTChg;
	FILETIME FN_ReadTime;
	UINT64 FN_AllocSz;
	UINT64 FN_ValidSz;//�ļ�����ʵ�ߴ�
	UINT32 FN_DOSAttr;//DOS�ļ�����
	UINT32 FN_EA_Reparse;//��չ����������
	BYTE FN_NameSz;//�ļ������ַ���
	BYTE FN_NamSpace;/*�����ռ䣬��ֵ��Ϊ����ֵ�е�����һ��
						0��POSIX������ʹ�ó�NULL�ͷָ�����/��֮�������UNICODE�ַ���������ʹ��255���ַ���ע�⣺�������ǺϷ��ַ�����Windows������ʹ�á�
						1��Win32��Win32��POSIX��һ���Ӽ��������ִ�Сд������ʹ�ó�������������������?��������������/������<������>������/������|��֮�������UNICODE�ַ��������ֲ����ԡ�.����ո��β��
						2��DOS��DOS�����ռ���Win32���Ӽ���ֻ֧��ASCII����ڿո��8BIT��д�ַ����Ҳ�֧�������ַ�������������������?��������������/������<������>������/������|������+������,������;������=����ͬʱ���ֱ��밴���¸�ʽ������1~8���ַ���Ȼ���ǡ�.����Ȼ������1~3���ַ���
						3��Win32&DOS����������ռ���ζ��Win32��DOS�ļ����������ͬһ���ļ��������С�*/
	BYTE FN_FileName[0];
}FILE_NAME,*pFILE_NAME;


//VOLUME_VERSION 
typedef struct _VOLUME_VERSION {
	//??
}VOLUME_VERSION,*pVOLUME_VERSION;


//OBJECT_ID 0X40������
typedef struct _OBJECT_ID {
	BYTE OID_ObjID[16];//�ļ���GUID
	BYTE OID_BirthVolID[16];//�ļ�����ʱ���ھ��ID
	BYTE OID_BirthID[16];//�ļ���ԭʼID
	BYTE OID_DomainID[16];//����������ʱ�������ID
}OBJECT_ID, *pOBJECT_ID;


//SECRUITY_DESCRIPTOR 0X50������
typedef struct _SECRUITY_DESCRIPTOR {
	//??
}SECRUITY_DESCRIPTOR,*pSECRUITY_DESCRIPTOR;


//VOLUME_NAME 0X60������
typedef struct _VOLUME_NAME {
	BYTE VN_Name[0];
}VOLUME_NAME,*pVOLUME_NAME;


//VOLUME_INFORMATION 0X70������
typedef struct _VOLUME_INFORMATION{
	UINT64 VI_Resvd;
	BYTE VI_MajVer;//�����汾��
	BYTE VI_MinVer;//���Ӱ汾��
	UINT16 VI_Flags;/*��־λ�����������¸�ֵ���
					0x0001    ��λ������ֵ������ʱWindows�������´�����ʱ����chkdsk/F���
					0x0002    ��־�ļ��ı�ߴ�
					0x0004    ��ҽ�ʱ����
					0x0008    ��Windows NT 4�ҽ�
					0x0010    ����ʱɾ��USN
					0x0020    �޸�����ID
					0x8000    ��chkdsk�޸Ĺ�*/
}VOLUME_INFORMATION,*pVOLUME_INFORMATION;


//DATA 0X80������
typedef struct _DATA {
	//??
	///*+0x10*/   UINT64 StartVcn;     // LowVcn ��ʼVCN  ��ʼ�غ�  
	///*+0x18*/   UINT64 LastVcn;      // HighVcn  ����VCN  �����غ�  
	///*+0x20*/   UINT16 RunArrayOffset;    // �������е�ƫ��  
	///*+0x22*/   UINT16 CompressionUnit;   // ѹ������  
	///*+0x24*/   UINT32  Padding0;       // ���  
	///*+0x28*/   UINT32  IndexedFlag;    // Ϊ����ֵ�����С(������Ĵص��ֽ�������)  
	///*+0x30*/   UINT64 AllocatedSize;   // ����ֵʵ�ʴ�С  
	///*+0x38*/   UINT64 DataSize;     // ����ֵѹ����С  
	///*+0x40*/   UINT64 InitializedSize;   // ʵ�����ݴ�С  
	///*+0x48*/   UINT64 CompressedSize;    // ѹ�����С 
	BYTE D_data[0];
}DATA,*pDATA;



typedef struct _INDEX_ENTRY {
	UINT64 IE_MftReferNumber;/*���ļ���MFT�ο��š�ע�⣺��ֵ�ĵ�6�ֽ���MFT��¼�ţ���2�ֽ��Ǹ�MFT��¼�����к�*/
	UINT16 IE_Size;//������Ĵ�С ����������ʼ��ƫ����
	UINT16 IE_FileNAmeAttriBodySize;//�ļ���������Ĵ�С
	UINT16 IE_Flags;/*��־����ֵ����������ֵ֮һ��
					0x00       ��ͨ�ļ���
					0x01       ������
					0x02       ��ǰ�������һ��Ŀ¼��
					�ڶ�ȡ����������ʱӦ�����ȼ��ó�Ա��ֵ��ȷ����ǰ�������*/
	UINT16 IE_Fill;//��� ������
	UINT64 IE_FatherDirMftReferNumber;//��Ŀ¼��MFT�ļ��ο���
	FILETIME IE_CreatTime;//�ļ�����ʱ��
	FILETIME IE_AlterTime;//�ļ�����޸�ʱ��
	FILETIME IE_MFTChgTime;//�ļ���¼����޸�ʱ��
	FILETIME IE_ReadTime;//�ļ�������ʱ��

	UINT64 IE_FileAllocSize;//�ļ������С
	UINT64 IE_FileRealSize;//�ļ�ʵ�ʴ�С
	UINT64 IE_FileFlag;//�ļ���־
	UINT8 IE_FileNameSize;//�ļ�������
	UINT8 IE_FileNamespace;//�ļ������ռ�
	BYTE IE_FileNameAndFill[0];//�ļ��������
							   //BYTE IE_Stream[0];//Ŀ¼�����ݣ��ṹ���ļ������Ե�������ͬ
							   //UINT64 IE_SubNodeFR;//����ļ�¼��������ֵ�ĵ�6�ֽ���MFT��¼�ţ���2�ֽ��Ǹ�MFT��¼�����к�
}INDEX_ENTRY,*pINDEX_ENTRY;

typedef struct _INDEX_HEADER {
	UINT32 IH_EntryOff;//��һ��Ŀ¼���ƫ��
	UINT32 IH_TalSzOfEntries;//Ŀ¼����ܳߴ�(��������ͷ�������������)
	UINT32 IH_AllocSize;//Ŀ¼�����ĳߴ�
	BYTE IH_Flags;/*��־λ����ֵ���������º�ֵ֮һ��
				  0x00       СĿ¼(���ݴ���ڸ��ڵ����������)
				  0x01       ��Ŀ¼(��ҪĿ¼��洢����������λͼ)*/
	BYTE IH_Resvd[3];
}INDEX_HEADER,*pINDEX_HEADER;

//INDEX_ROOT 0X90������
typedef struct _INDEX_ROOT {
	//������
	UINT32 IR_AttrType;//���Ե�����
	UINT32 IR_ColRule;//�������
	UINT32 IR_EntrySz;//Ŀ¼�����ߴ�
	BYTE IR_ClusPerRec;//ÿ��Ŀ¼��ռ�õĴ���
	BYTE IR_Resvd[3];
	//����ͷ
	INDEX_HEADER IH;
	//������  ���ܲ�����
	BYTE IR_IndexEntry[0];
}INDEX_ROOT,*pINDEX_ROOT;

//INDEX_ALLOCATION 0XA0������
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
	UINT32 RP_Type;/*�ؽ����������ͣ���ֵ����������ֵ֮һ
					0x20000000    ����
					0x40000000    ��ߵȴ�ʱ��
					0x80000000    ΢��ʹ��
					0x68000005    NSS
					0x68000006    NSS�ָ�
					0x68000007    SIS
					0x68000008    DFS
					0x88000003    ��ҽӵ�
					0xA8000004   HSM
					0xE8000000   Ӳ����*/
	UINT16 RP_DatSz;//�ؽ������ݳߴ�
	UINT16 RP_Resvd;//
	BYTE RP_Data[0];//	�ؽ�������
}REPARSE_POINT,*pREPARSE_POINT;

//EA_INFORMATION
typedef struct _EA_INFORMATION {
	UINT16 EI_PackedSz;//	ѹ����չ���Գߴ�
	UINT16 EI_NumOfEA;//ӵ��NEED_EA��¼����չ���Ը���
	UINT32 EI_UnpackedSz;//δѹ����չ���Գߴ�
}EA_INFORMATION,*pEA_INFORMATION;

//EA
typedef struct _EA {
	UINT32 EA_Next;//��һ����չ���Ե�ƫ��(����¼�ĳߴ�)
	BYTE EA_Flags;//��־λ��ֵȡ0x80��ʾ��ҪEA
	BYTE EA_NamLen;//�������ݵĳ���(M)
	UINT16 EA_ValLen;//ֵ���ݵĳ���
	BYTE EA_NameVal[0];//�������ݺ�ֵ����
}EA,*pEA;

//PROPERTY_SET
typedef struct _PROPERTY_SET {
	//??
}PROPERTY_SET,*pPROPERTY_SET;

//LOGGED_UNTILITY_STREAM
typedef struct _LOGGED_UNTILITY_STREAM {
	//??
}LOGGED_UNTILITY_STREAM,*pLOGGED_UNTILITY_STREAM;