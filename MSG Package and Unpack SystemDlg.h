
// MSG Package and Unpack SystemDlg.h : 头文件
//

#pragma once


// CMSGPackageandUnpackSystemDlg 对话框
class CMSGPackageandUnpackSystemDlg : public CDialogEx
{
// 构造
public:
	CMSGPackageandUnpackSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MSGPACKAGEANDUNPACKSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnectBtn();
	afx_msg void OnBnClickedListenBtn();
	afx_msg void OnBnClickedMsgsendBtn();
	CString MsgSendContent;
	CString SendDetails;
	CString ReceivedDetails;
};
