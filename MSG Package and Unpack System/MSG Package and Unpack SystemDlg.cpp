
// MSG Package and Unpack SystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MSG Package and Unpack System.h"
#include "MSG Package and Unpack SystemDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMSGPackageandUnpackSystemDlg 对话框



CMSGPackageandUnpackSystemDlg::CMSGPackageandUnpackSystemDlg(CWnd* pParent /*=NULL*/) //构造函数
	: CDialogEx(CMSGPackageandUnpackSystemDlg::IDD, pParent)
	, MsgSendContent(_T(""))
	, SendDetails(_T(""))
	, ReceivedDetails(_T(""))
	, PortEditText(_T(""))
	, IPEditStr(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->MsgPackageObj = new MsgPackage(&SendDetails,&ReceivedDetails,this);
}

CMSGPackageandUnpackSystemDlg::~CMSGPackageandUnpackSystemDlg() //析构函数
{
	this->MsgPackageObj->StopListen();
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
	DDX_Text(pDX, IDC_ListenPort_Edit, PortEditText);
	DDX_Control(pDX, IDC_Connect_Btn, ConnectBtn);
	DDX_Text(pDX, IDC_ConnectIP_Edit, IPEditStr);
}

BEGIN_MESSAGE_MAP(CMSGPackageandUnpackSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Connect_Btn, &CMSGPackageandUnpackSystemDlg::OnBnClickedConnectBtn)
	ON_BN_CLICKED(IDC_Listen_Btn, &CMSGPackageandUnpackSystemDlg::OnBnClickedListenBtn)
	ON_BN_CLICKED(IDC_MsgSend_Btn, &CMSGPackageandUnpackSystemDlg::OnBnClickedMsgsendBtn)
END_MESSAGE_MAP()


// CMSGPackageandUnpackSystemDlg 消息处理程序

BOOL CMSGPackageandUnpackSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMSGPackageandUnpackSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMSGPackageandUnpackSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CMSGPackageandUnpackSystemDlg::OnBnClickedConnectBtn() //连接按钮
{
	CString ConnectBtnCaption; //连接按钮文本
	this->ConnectBtn.GetWindowText(ConnectBtnCaption); //获取连接按钮文本
	if(ConnectBtnCaption == "连接")
	{ //连接服务器
		UpdateData();
		if(!this->CheckConnectIPVaild(this->IPEditStr)) //检查输入的IP地址是否合法
		{
			MessageBox(_T("请输入正确的IP地址!"),_T("错误"));
			return;
		}
		if(!this->CheckPortVaild(this->PortEditText)) //检查输入的端口是否合法
		{
			MessageBox(_T("请输入正确的端口号：1-65535"),_T("错误"));
			return;
		}
		if(!this->MsgPackageObj->StartConnectServer(this->IPEditStr,_ttoi(this->PortEditText))) //连接服务器
		{
			MessageBox(_T("连接服务器失败!"),_T("错误"));
			return;
		}
		this->ListenBtn.EnableWindow(false); //客户端模式，禁用监听按钮
		this->ConnectBtn.SetWindowText(_T("停止"));
	}
	else
	{ //停止连接服务器
		this->MsgPackageObj->StopConnectServer(); //关闭连接
		this->ConnectBtn.SetWindowText(_T("连接")); //更新连接按钮文字
		this->ListenBtn.EnableWindow(true); //启用监听按钮
	}
}


void CMSGPackageandUnpackSystemDlg::OnBnClickedListenBtn() //监听按钮
{
	CString ListenBtnCaption; //监听按钮文本
	this->ListenBtn.GetWindowText(ListenBtnCaption); //获取监听按钮文本
	if(ListenBtnCaption == "监听")
	{ //开启监听
		UpdateData();
		if(!this->CheckPortVaild(this->PortEditText)) //检测端口输入合法性
		{
			MessageBox(_T("请输入正确的端口号：1-65535"),_T("错误"));
			return;
		}
		if(!this->MsgPackageObj->StartListen(_ttoi(this->PortEditText))) //开启监听失败
		{
		MessageBox(_T("服务器模式开启失败!"),_T("错误"));
			return;
		}
		this->ConnectBtn.EnableWindow(false); //服务器端模式,禁用连接按钮
		this->ListenBtn.SetWindowText(_T("停止"));
		MessageBox(_T("服务器模式开启成功!"),_T("成功"));
	}
	else
	{ //停止监听
		this->MsgPackageObj->StopListen();
		this->ListenBtn.SetWindowText(_T("监听"));
		this->ConnectBtn.EnableWindow(true); //启用连接按钮
	}
}


void CMSGPackageandUnpackSystemDlg::OnBnClickedMsgsendBtn() //发送按钮
{
	UpdateData();
	if(!this->MsgPackageObj->SendPackage(this->MsgSendContent)) //发送数据
	{
		MessageBox(_T("发送失败!请检查是否已建立连接!"),_T("错误"));
	}
}

bool CMSGPackageandUnpackSystemDlg::CheckPortVaild(CString PortStr) //检查端口输入是否合法，合法端口应该是1-65535
{
	for(int i=0;i<PortStr.GetLength();i++)
	{
		if((int)PortStr.GetAt(i)<48 || (int)PortStr.GetAt(i)>57) //判断每一位是否都是数字
		{
			return false;
		}
	}
	if(_ttoi(PortStr)<1 || _ttoi(PortStr)>65535) //判断端口号是否在1-65535之间
	{
		return false;
	}
	return true;
}

bool CMSGPackageandUnpackSystemDlg::CheckConnectIPVaild(CString IPStr) //判断IP输入是否合法
{
	if(inet_addr((LPCSTR)(LPCTSTR)IPStr) == INADDR_NONE) //非合法IP地址
	{
		return false;
	}
	return true;
}