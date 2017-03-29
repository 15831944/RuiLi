#pragma once
class CPartMdlOpen
{
public:
	CPartMdlOpen(void);
	~CPartMdlOpen(void);
	CDatabase         *  m_db;
	ProSolid            part_solid;
	ProDrawing          part_draw;
	CString				part_dir;

	//���������
	BOOL OpenPart(CDatabase * p_db,long part_num);
	//���������ģ��
	BOOL Open(CDatabase * p_db,long part_num);
	//�������
	BOOL LoadPart(long num);
	//����Ƿ�����ͬ�Ĵ���
	BOOL CheckCurrentPart(long num);
	//�������ģ��
	BOOL LoadSolid(long num);
	//�������ģ��
	BOOL CheckCurrentPartMdl(long num);
	BOOL AddPartMdlInfo(long  PrtNum);
	//����������Ϣ
	BOOL AddPartInfo(long  PrtNum);
	BOOL AddPartMdlInfoForOtherUse(CDatabase * p_db,ProSolid solid,long part_num);
};
