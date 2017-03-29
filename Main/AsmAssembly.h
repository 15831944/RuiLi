//���齨װ�����
#pragma once

class CAsmAssembly
{
public:
	CAsmAssembly(void);
	~CAsmAssembly(void);
	//Ԫ���Ľṹ
	typedef struct comp_item{
		CString          type;        //���� �������� "asm" "prt"
		ProSolid         mdl;         //ģ�͵ľ��
		int              postid;      //��׺
		ProDrawing       drw;
		long             num;         //��ʶ�� ���ʵ�� �� ���ʵ��
		long             mdlcomnum;   //����㲿���ڲ�Ʒģ���еı�ţ����ڻ�ð����ģ�͵Ŀɱ���Ϣ��ӽ���
		long             mdlnum;      //ģ�ͺ� ���ģ�͵ı�� �����ģ�͵ı�� �������ģ�� -1
		CString          pdmnum;
		CString          filename;    //�ļ���
		CString          filefullpath;//�ļ��Ĵ��λ��
		CString          drawfilename;    //�ļ���
		CString          drawfilefullpath;//�ļ��Ĵ��λ��
		int              fileresource;//��Դ 0����ϵͳ  1 ���ϵͳ 2 PDMϵͳ
		bool             status;      //�Ƿ��Ѿ��ڵ�ǰ�д��� true ��ʾ���� false��ʾ������
	}comp_item;
	CArray<comp_item,comp_item> p_compitemlist;
	CDatabase * m_db;
	long        m_asm_num;
	CString     error_info;
	//���������Ʒ������������
	BOOL  OpenProduct(CDatabase * p_db,long asm_num,ProSolid * asm_com);
	//��������Ʒ�������㲿���Ƿ��Ѿ��ڵ�ǰ��
	//�û�����ȷ�ϲ���
	//����������Ѿ��е���������ڵ����������ͬһ�����������Ϣ
	//�������Ĵ��λ��
	BOOL  GetCompInfo(long asm_num);
	BOOL  GetCompMdl();										 //���ģ�;��    
	BOOL  CheckProduct(long asm_num);
	//���أ�����ģ�͵����ڴ�
	BOOL  LoadSolid(long asm_num);
	//װ��ɹ������Ϣ
	BOOL AddInfo();
	//װ�䲻�ɹ��������Ϣ
	BOOL CancelAssembly();
	BOOL AddComInfo(long asm_num);                           //��Ԫ�������Ϣ
	BOOL AddCompPartInfo(ProSolid mdl,ProDrawing part_draw,long num);             //����������Ϣ
	BOOL AddCompAsmInfo(ProSolid mdl,ProDrawing part_draw,long num);              //����������Ϣ
	CString  localDir;//���ص�Ŀ¼
	ProSolid asm_solid;
	ProDrawing asm_draw;
};
