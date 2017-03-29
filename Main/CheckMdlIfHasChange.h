#pragma once
//���ģ���Ƿ��Ѿ��ı�
typedef enum REVTYPE{
	CHECK_FALSE=-1,
	CHECK_TRUE=0,
	CHECK_HAVE_NO_SWTX=1,
	CHECK_DIM_OR_PARA_WAS_DELTETED=2,
	CHECK_DIM_HAS_CHANGED=3,
	CHECK_PARA_HAS_CHANGED=4,
};
class CCheckMdlIfHasChange
{
public:
	CCheckMdlIfHasChange(void);
	~CCheckMdlIfHasChange(void);
private:
	typedef struct dim_para_item{
		int           type;      //�����ģ��TYPE_DIM TYPE_PARA
		long          Num;       //���
		CString       Name;
		CString       IdNum;     //�ߴ�Ͳ����ı�ʶ
		ProDimension  Dim;       //�ߴ�
		ProParameter  Para;      //����
		CString       ParaType;  //����������

		double        mValue;       //ģ�͵ĳ�ʼֵ
		CString       mParaStrValue;   //������������µĲ���ֵ
		int           mParaIntValue;   //��������������µ�ֵ 

		double        nValue;       //����ʵ����ֵ    
		CString       nParaStrValue;   //������������µĲ���ֵ
		int           nParaIntValue;   //��������������µ�ֵ     
	};

	typedef struct asm_mdl_item{
		ProSolid  mdl;
		int       num;
		int       postid;
	}asm_mdl_item;
public:
	//���ɱ������ֵ�Ƿ���ԭ�еĲ�ͬ

	//���һ������ģ�͵����
	REVTYPE CheckPartHasPrtMdl(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType/*��ʾ����Ƿ����޸ģ������ж���������ʵ��*/);
	//���һ��û��ģ�͵����
	REVTYPE CheckPartNoPrtMdl(CDatabase * p_db,ProSolid solid, long PrtNum,int CheckType);
	//���һ������ģ�͵Ĳ���
	REVTYPE CheckAsmHasAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType);
	//���һ��û��ģ�͵Ĳ���
	REVTYPE CheckAsmNoAsmMdl(CDatabase * p_db,ProSolid solid, long AsmNum,int CheckType);
	//������
	REVTYPE CheckPart(CDatabase * p_db,ProSolid solid,long PrtNum,int CheckType);
	//��鲿��
	REVTYPE CheckAsm(CDatabase * p_db,ProSolid solid,long AsmNum,int CheckType);
    //����ϵʽ
	REVTYPE CheckRel(CDatabase * p_db,ProSolid solid,long Num,int CheckType/*������0���Ǽ�����1*/);
	//���ɱ�����Ƿ���
	//���ݷ���ֵ���ж�
	CArray<ProDimension,ProDimension> p_ReDimList;         //�Ѿ��޸Ĺ��ĳߴ�
	CStringArray                      p_ReDimYuanlaiVal;   //ԭ���ĳߴ�ֵ
	CArray<ProParameter,ProParameter> p_ReParaList;        //�޸Ĺ��Ĳ�����ֵ
	CStringArray					  p_ReParaYuanlaiVal;  //ԭ���Ĳ���ֵ
    CString							  error_info;
	long							  changenum;           //������޸�֮����ģ���������������һ��
	CArray<asm_mdl_item,asm_mdl_item>       p_temp_AsmMdlItemList;
	void InitReValue();
};
