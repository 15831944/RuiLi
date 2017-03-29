#pragma once

// MathString.h: interface for the CMathString class.
//
// MathString.h: interface for the CMathString class.

//////////////////////////////////////////////////////////////////////                    
//
//                   ���������ʽֵ����
//
//              ���ϳ���ѧԺ  ��Ϣ������ѧϵ  
//                        �ƽ���
//                 jiangfenghuang@msn.com
//                 jiangfenghuang@163.net
//                       2003.4.5           
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// ���ܣ�
//      �����ַ����е��������ʽ��ֵ��
//   
// �ص㣺
//      1.֧���ַ����к��и��ֳ��ú�������"7.5+sin(6*ln(8))/exp(5)"
//      2.���кܺõľ����������ܼ������ʽ�������Ƿ���ԡ��⺯����
//        ����ȷ
//      3.����������ܼ��������쳣�������Ϊ0����������sqrt(x)��x<0
//        �����Һ���acos(x)�е�x<-1��x>1��
//      4.Ϊ֧�ֻ��֡��󷽳̣��������ʽ�пɺ���x������ʱ�������еĳ�
//        Ա����xx���� 
//////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////// 
//
// ʹ�÷�����
//      1. ����һ��CMathString �Ķ�����Ҫ����������ʽ�����ʼ��
//         �磺 char *s1="7.5+sin(6)/exp(5)";
//              char *s2="4*pow(x,2)+2*x+4";
//              CMathString  mathstr1(s1);
//              CMathString  mathstr2(s2);
//      2.����checkString���������ʽ�Ƿ�Ϸ�
//      3.���������ֻ�j�ⷽ�̣�����setX(double x)���ñ��ʽ��x��ֵ
//      4.����stringToBolan()���������ʽת��Ϊ�������ʽ
//      5.����compvalue()�Բ������ʽ���м��㣬������ֵΪ0�����
//        ��getvalue()ȡ�ü�����������ɸ��ݷ���ֵ�жϳ���ط�
//
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//
//                ֧�ֵĺ����ı�ż�����
//
// ���     ����         ����         ���õ�math.h�⺯��     ��ע
//
//   1      abs(x)      ȡ����ֵ|x|    fabs(x)      
//   2      acos(x)     �����Һ���     acos(x)            -1<=x<=1
//   3      asin(x)     �����Һ���     asin(x)            -1<=x<=1
//   4      atan(x)     �����к���     atan(x)
//   5      acot(x)     �����к���     PI/2-atan(x)
//   6      cos(x)      ���Һ���       cos(x)
//   7      cosh(x)     ˫�����Һ���   cosh(x)
//   8      cot(x)      ���к���       1/tan(x)            tan(x)!=0
//   9      exp(x)      e��x�η�       exp(x)
//  10      floor(x)    ��x��          floor(x)   �󲻴���x��������� 
//  11      mod(x,y)     x%y           mod(x,y)             y!=0
//  12      ln(x)       ȡ��Ȼ����     log(x)               x>0
//  13      log(x)      ȡ10�Ķ���     log10(x)             x>0
//  14      pow(x,y)    x��y�η�       pow(x,y)
//  15      sin(x)      ���Һ���       sin(x)
//  16      sinh(x)     ˫�����Һ���   sinh(x)
//  17      sqrt(x)     ��x����        sqrt(x)            x>=0
//  18      tan(x)      ���к���       tan(x)
//  19      tanh(x)     ˫�����к���   tanh(x)
//
/////////////////////////////////////////////////////////////////////
#if !defined(AFX_MATHSTRING_H__669187A1_784A_421F_B9EA_EAED6FA6D9AF__INCLUDED_)
#define AFX_MATHSTRING_H__669187A1_784A_421F_B9EA_EAED6FA6D9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX    100    //���ʽ�Ϊ100���ַ�

class CMathString  
{
public:
	void Initial(char *str,double x);
	void Initial(char *s);
	//��Ա����	
	char string[MAX];     //����������ʽ������ΪMAX=100
	double xx,             //��Ż��ֻ����е�x
		result;            //�������ʽ��ֵ

	struct Bolan          //��Ų������ʽ�Ľṹ
	{
		int     flag;     //��ʶ�ýڵ�����ֵ����������Ǻ���
		//0Ϊ��ֵ��1Ϊ�������2Ϊ����
		double  value;    //��ֵֵ
		char    oper;     //����� +,-,*,/,~(ȡ��),(,)
		int     func;     //�����ı�ţ���������ż����ܼ���ʼ����

	};
	Bolan bolan[MAX];     //���ת����Ĳ������ʽ������ΪMAX=100

	CMathString();
	CMathString(char *s);  //���캯�������������ʽ*s����string
	CMathString(char *s,double x);   //���ʽ�д�δ֪���Ĺ��캯��
	//�������ʽ*s����string��δ֪��x����xx
	//���ڻ��ֱ��ʽ
	//��Ա����
	void setX(double x);     //���ó�Ա����xx��ֵ
	int checkString(void);  //�������ı��ʽ�Ƿ���ȷ�����������Ƿ�
	//��ԡ��⺯���Ƿ���ȷ�������С���㡢����
	//�Ƿ���ȷ
	//����ȷ�������ʽ�еĴ������Ÿó�С����
	//���ڱ��ʽβ�ӡ�#����ʶ
	//����ȷ������ֵΪ1�����򷵻�0��
	//����ֱ��ʽ����x�����Ա��ʽ�е�x��Ϊ
	//�ַ�����

	int stringToBolan(void); //���������ʽstringת���ɲ������ʽ��
	//�������ʽ����bolan[]��
	int compvalue(void);  //���㲨�����ʽbolan[]��ֵ
	//��������;û�з��������򷵻�0���������
	//�Ĳ������ʽֵ����result
	//��������;�������������Ϊ0������
	double getvalue(void);   //���ؼ���õ��ı��ʽֵ��

	//�ڲ����ܺ���
	int seekStr(char *str,int &i,char *s);
	//����ַ���str�ĵ�i��λ�ÿ�ʼ�ĺ����ַ��Ƿ����ַ���s��ƥ��
	//�ǵĻ���i=i+len(s),������1�����򷵻�0
	double stringToDigital(char *s);  //�������ַ���ת������ֵ
	//����ֵΪ�ַ�������ֵ

	virtual ~CMathString();


	BOOL GetValue(char * s,double * value);
	bool IsDouble(char * s);
};

#endif // !defined(AFX_MATHSTRING_H__669187A1_784A_421F_B9EA_EAED6FA6D9AF__INCLUDED_)

class CRel
{
public:
	CRel(void);
	CRel(ProSolid solid,CString strRel);
	CRel(ProSolid solid,CString strRel,bool &);
	//���������ģʽ���޸�����ڲ��Ĺ�ϵʽ����Ϊ�����ģʽ���к�׺������ǿ����Ҫ��׺
	CRel(ProSolid solid,CString strRel,bool /*�����Ƿ�Ҫ��׺*/,bool &/*�ж��Ƿ�ɹ�*/);
	~CRel(void);
private:
	char         m_strRel[MAX];
	char         m_strRight[MAX];  //��ϵ�ұ�
	char         m_strLeft[MAX];   //��ϵʽ���
	char         m_strMid[MAX];    //��ϵʽ�м伴:=,>=,<=,>,<,<>
	CMathString  mathStr; 
	BOOL         bolNeedPostID;        //�Ƿ���Ҫ����ϵʽ��Ӻ�׺(������������)
	typedef struct rel_item{
		int           item_type;       //0���� 1�ߴ� 2����
		CString       symbol;          //����(����)���ߴ�Ͳ���֮������еģ�����ǳߴ�Ͳ���
		ProDimension  dim;             //�ߴ�
		ProParameter  para;            //����
	};
	CArray<rel_item,rel_item&> m_aryRel;
	CArray<rel_item,rel_item&> m_leftRel;
	CArray<rel_item,rel_item&> m_RightRel;
	typedef struct dimpara_item{
		CString      str;		
		int          type;                                                //TYPE_DIM TYPE_PARA
		ProDimension dim;
		ProParameter para;
	};
	CArray<dimpara_item,dimpara_item&>  m_aryDimParaItem;                 //ģ�������еĳߴ�Ͳ�������,ֻʹ��һ��
	CStringArray						m_LeftParaStringList;
	CStringArray					    m_RightParaStringList;
	void   DepartRel(void);
	
	CStringArray  m_calSymbol;
	void   InitCalSymbol(void);
	int    CheckRelPart(char rel_str[MAX],CArray<rel_item,rel_item&> * temp_rel_item);      //����ұߵ�

	
public:
	 typedef enum rel_type{
		REL_TYPE_EQUAL =0,
		REL_TYPE_MORE_EQUAL=1,
		REL_TYPE_LESS_EQUAL=2,
		REL_TYPE_MORE=3,
		REL_TYPE_LESS=4,
		REL_TYPE_UN_EQUAL=5,
		REL_TYPE_NO=-1
	}rel_type;
	ProSolid owner;
	CString  m_strNote;
	 //������Ϣ
	CString  m_strError;  
	//��ù�ϵʽ���������Ƿ���Ҫ��׺������ģ�����;��������������Զ��Ӻ�׺
	BOOL     GetRelString(CString &);
	//��ϵ����Ӻ�׺���������
	BOOL     GetRelStringAsComp(CString &);                                   
	CString  GetRelNote(void);	
	//��ϵ������ 0������ 1��>=�� 2(<=) 3(>) 4(<) 5(<>)
	rel_type      m_iRelType;        
	//����ұ߲������� < > �ұߵĲ�����
	void     GetRightParamList(CArray<ProParameter,ProParameter&> *);   
	//����ұ߳ߴ���б�
	void     GetRightDimList(CArray<ProDimension,ProDimension&> *);   
	//�����ߵĲ����б�
    void     GetLeftParamList(CArray<ProParameter,ProParameter&> *);   
	 //�����߳ߴ磨= < > ��ߵĲ�����
    void     GetLeftDimList(CArray<ProDimension,ProDimension&> *);          
	void     GetRelAllParamList(CArray<ProParameter,ProParameter&> *);
	void     GetRelAllDimList(CArray<ProDimension,ProDimension&> *);
	BOOL     SetRightParaValue();   //�����ұ߲�����ֵ
	BOOL     Updata(void);          //�����ϵʽ������ģ�ͱ���
	//����ϵ
    BOOL     CheckRel(void); 
	//����ϵʽ
	BOOL     CheckRel(CString);     
	 //����
	BOOL     Calculate(CString,double *);   
private:
	void	 GetDimParaOfSolid(ProSolid);
	int      GetSymbolNum(CString m_str,char * m_sym);
	BOOL     CheckEquation(CString Expression ,CString & strError);
	BOOL     CheckExpression(CString Expression , CStringArray * importDim, CStringArray * exportDim,CString & strError);
	ProMdlType  mdlType;
public:
	CRel(const CRel & RightSides);
	bool    operator==(const CRel & RightSide) const;
	//����ϵʽ�ķ���,����Ϊ������
	bool    CheckRelSymbol(CString rel,CString & error);                  
    //�ж��Ƿ�Ϊʵ��
	BOOL    IsDouble(CString str);     
	CRel &  operator=(const CRel & RightSides);
	//��ù�ϵʽ���ұߵ�����ʽ,�����ִ������
	CString GetRelRightFormula(void);
	//��ù�ϵʽ����ߵ�����ʽ,�����ִ���
	CString GetRelLeftFormula(void);  
	CString GetRelFormula(void); 
};


