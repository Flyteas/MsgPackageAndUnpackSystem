
// MSG Package and Unpack SystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MSG Package and Unpack System.h"
#include "MSG Package and Unpack SystemDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMSGPackageandUnpackSystemDlg �Ի���



CMSGPackageandUnpackSystemDlg::CMSGPackageandUnpackSystemDlg(CWnd* pParent /*=NULL*/) //���캯��
	: CDialogEx(CMSGPackageandUnpackSystemDlg::IDD, pParent)
	, MsgSendContent(_T(""))
	, SendDetails(_T(""))
	, ReceivedDetails(_T(""))
	, ListenPortEditText(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->MsgPackageObj = new MsgPackage(NULL,NULL);
}

CMSGPackageandUnpackSystemDlg::~CMSGPackageandUnpackSystemDlg() //��������
{
	delete this->MsgPackageObj;
	this->MsgPackageObj = NULL;
}

void CMSGPackageandUnpackSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MsgSend_Edit, MsgSendContent);
	DDX_Text(pDX, IDC_SendDetails_Edit, SendDetails);
	DDX_Text(pDX, IDC_ReceivedDetails_Edit, ReceivedDetails);
	DDX_Control(pDX, IDC_Listen_Btn, ListenBtn);
	DDX_Text(pDX, IDC_ListenPort_Edit, ListenPortEditText);
	DDX_Control(pDX, IDC_Connect_Btn, ConnectBtn);
}

BEGIN_MESSAGE_MAP(CMSGPackageandUnpackSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Connect_Btn, &CMSGPackageandUnpackSystemDlg::OnBnClickedConnectBtn)
	ON_BN_CLICKED(IDC_Listen_Btn, &CMSGPackageandUnpackSystemDlg::OnBnClickedListenBtn)
	ON_BN_CLICKED(IDC_MsgSend_Btn, &CMSGPackageandUnpackSystemDlg::OnBnClickedMsgsendBtn)
END_MESSAGE_MAP()


// CMSGPackageandUnpackSystemDlg ��Ϣ�������

BOOL CMSGPackageandUnpackSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMSGPackageandUnpackSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMSGPackageandUnpackSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMSGPackageandUnpackSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CMSGPackageandUnpackSystemDlg::OnBnClickedConnectBtn() //���Ӱ�ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CMSGPackageandUnpackSystemDlg::OnBnClickedListenBtn() //������ť
{
	CString ListenBtnCaption; //������ť�ı�
	this->ListenBtn.GetWindowTextW(ListenBtnCaption);
	if(ListenBtnCaption == "����")
	{
		UpdateData();
		if(!this->CheckListenPortEditVaild(ListenPortEditText)) //���˿�����Ϸ���
		{
			MessageBox(_T("��������ȷ�Ķ˿ںţ�1-65535"),_T("����"));
			return;
		}
		if(!this->MsgPackageObj->StartListen(_ttoi(ListenPortEditText))) //��������ʧ��
		{
			return;
		}
		this->ConnectBtn.EnableWindow(false); //��������ģʽ,�������Ӱ�ť
		this->ListenBtn.SetWindowTextW(_T("ֹͣ"));
		MessageBox(_T("������ģʽ�����ɹ�!",_T("�ɹ�")));
	}
	else
	{
		this->MsgPackageObj->StopListen();
		this->ListenBtn.SetWindowTextW(_T("����"));
		this->ConnectBtn.EnableWindow(true); //�������Ӱ�ť
	}
}


void CMSGPackageandUnpackSystemDlg::OnBnClickedMsgsendBtn() //���Ͱ�ť
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

bool CMSGPackageandUnpackSystemDlg::CheckListenPortEditVaild(CString ListenPortStr) //�������˿������Ƿ�Ϸ����Ϸ��˿�Ӧ����1-65535
{
	for(int i=0;i<ListenPortStr.GetLength();i++)
	{
		if((int)ListenPortStr.GetAt(i)<48 || (int)ListenPortStr.GetAt(i)>57) //�ж�ÿһλ�Ƿ�������
		{
			return false;
		}
	}
	if(_ttoi(ListenPortStr)<1 || _ttoi(ListenPortStr)>65535) //�ж϶˿ں��Ƿ���1-65535֮��
	{
		return false;
	}
	return true;
}