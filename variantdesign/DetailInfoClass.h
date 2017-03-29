#pragma once
#include "ProSolid.h"


// DetailInfoClass ����Ŀ��

class DetailInfoClass : public CObject
{
public:
	DetailInfoClass();
	virtual ~DetailInfoClass();
public:
	ProSolid  solid;				   //������
	CString   InsCode;				   //����ͼ��
	int       SolidID;				   //����Ż��������
	int       OldSolidID;              //�ɵ�������������
	int       ProductID;               //��Ʒ��ID,�������Ϊ-1
	CString   MdlNum;				   //ģ�ͱ��
	CString   MdlName;                 //ģ������
	CString   InsName;				   //ʵ����
	CString   Type;					   //����������
	CString   Designer;				   //�����
	CString   FinishTime;              //������ʱ��
	CString   Auditor;				   //�����
	CString   AuditTime;			   //���ʱ��
	CString   Material;				   //���ϱ��
	CString   GrpFrm;                  //�����������
	int       GroupNum;                //���������

	CString   Unit;                    //�ߴ絥λ

	CString   AuditState;              //���״̬
	CString   DesignState;             //���״̬
	CString   PDMState;                //�ύ״̬

	CString   SolidFilePath;		   //�ļ�����λ��
	CString   SolidFileName;           //��ά�ļ���

	CString   OldSolidFileName;		   //ģ�͵��ļ���
	CString   OldDrwName;

	CString   DrwName;                 //��ά�ļ���
	CString   DrwFilePath;

	CString   OldSolidFilePath;		   //ģ�͵ı���λ��
	CString   OldDrwFilePath;

	int       SaveStyle;               //���淽ʽ��0=���룬1=���£�2�������ύ
	BOOL      IsSaved;                 //��Ϣ�Ƿ��Ѿ�����
	CString   IsInsert;                //�Ѿ������

	CString   TempPath;				   //�ϴ���������ʱ�������ļ�����ʱĿ¼��ַ
	CString   BackupPathSolid;			   //�������ݷ��������صĵ�ַ���Է�����ʧ�ܵ�ʱ��������ϴ�
	CString   BackupPathDrw;

	int       MdlCompNum;              //��ʵ���к�ģ�Ͷ�Ӧ�ı�ţ���ģ�ʹ��ǻ��

	int       SolidVersion;
	int       DrwVersion;
	BOOL      CurrentSaveFlag;         //��ǰ�Ƿ񱣴���ı�־���ڱ���ʧ�ܻ�ɹ���
	CString   TaskType;                //�ڴ����������

    CStringArray PerformanceValue;     //���ܲ���ֵ
	CStringArray InsValueStr;          //���ܲ���



	CString   IsLocalOrServer;         //�����ڱ��ػ��Ƿ�����
	CString   PDMCode;                 //PDMͼ��
	CString   OldInsCode;              //Ҫ���͵�����򲿼��Ļ�ͼ��
	CString   Note;







	////////////////////////////////////////////////////////////////��Ҫ��
	//CString   PartMap;
	//CString   OldPartMap;
	//CString   PartNum;
	//BOOL      IsHave;       //�Ƿ��������ܲ���
	//BOOL      PerHave;       //ʵ���Ƿ���д�����ܲ���
	//BOOL      IsInsHave;	//�Ƿ��е�ǰ��ʵ��
	//BOOL      IsVarHave;		//�Ƿ��пɱ����
	//BOOL      IsThreeModelHave;         //�Ƿ�����ά��ģ�ʹ���
	//
	//BOOL      IsMenberHave;
	//BOOL      IsInsCodeNewOrNot;      //ȷ��ͼ������ʵ�������Ǿ�ʵ��
	//int       InsNum;       //Ҫ�������ʵ����


};


