
// MSG Package and Unpack SystemDlg.h : ͷ�ļ�
//

#pragma once


// CMSGPackageandUnpackSystemDlg �Ի���
class CMSGPackageandUnpackSystemDlg : public CDialogEx
{
// ����
public:
	CMSGPackageandUnpackSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MSGPACKAGEANDUNPACKSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnectBtn();
	afx_msg void OnBnClickedListenBtn();
	afx_msg void OnBnClickedMsgsendBtn();
};
