#pragma once

class CAsmInfoFn
{
public:
	CAsmInfoFn(void);
	~CAsmInfoFn(void);
private:
	//�����ģ�͵����ܲ�����Ϣ��ӵ������
	BOOL AddAsmMdlPerfParaInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//�����ģ�͵�ͼƬ��Ϣ��ӵ������
	BOOL AddAsmMdlPicInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//�����ģ�͵��������Ա���Ϣ��ӵ����ģ����
	BOOL AddAsmMdlSmlItemForMdl(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//�����ģ�͵��������Ա���Ϣ��ӵ����ʵ����ȥ
	BOOL AddAsmMdlSmlItemForIns(CDatabase * p_db,ProMdl asm_solid,long asm_num,long asm_mdl_num);


	//��������
	//����Ԫ�������ƣ��õ�Ԫ����
	BOOL GetComAndComNumList(CDatabase * p_db,ProMdl asm_solid,long asm_num
		,CArray<ProSolid,ProSolid> & solidlist
		,CArray<long,long>	& comnumlist);
public:
	//��������ģ����Ϣ
	BOOL AddAsmMdlInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
	//��������ģ����Ϣ
	BOOL AddAsmInsInfo(CDatabase * p_db,ProMdl asm_solid,long asm_num);
};
