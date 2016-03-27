#pragma once
/* ��Ϣ�������� */
#include "MsgPackage.h"
class MsgTransport
{
public:
	MsgTransport(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg);
	~MsgTransport(void);
	void SetListenStatus(bool ListenOpenStatus); //��������setter����,true��ʾ��������,false��ʾ�رռ���
	void SetServerPort(int Port); //�������˿�setter����
	void SetServerIP(CString IP); //������IP setter����
	bool StartListen(int Port); //������ģʽ������һ������
	void StopListen(); //������ģʽ��ֹͣ����
	bool StartConnectServer(CString IP,int Port); //�ͻ���ģʽ�����ӷ�����
	void StopConnectServer(); //�ͻ���ģʽ��ֹͣ���ӷ�����
	bool SendPackage(CString PackageData); //����һ�����ݰ�

private:
	CString *MsgSendDetail; //��Ϣ�����ı����ı�����
	CString *MsgReceivedDetail; //��Ϣ�����ı����ı�����
	CDialogEx* MainWindow; //������
	MsgPackage* MsgPackageObj; //���ݰ���װ��������
	bool ListenStatus; //����������ʶ,��������ģʽ
	bool ConnectStatus; //���ӿ�����ʶ���ͻ���ģʽ
	int ServerPort; //�������˿�
	CString ServerIP; //������IP
	CAsyncSocket *ConnectSocket;
	bool ClientConnectFlag; //������ģʽ���ͻ�������״̬��ʶ
	bool ConnectServerFlag; //�ͻ���ģʽ�����ӷ�����״̬��ʶ
	CWinThread *SocketThread; //Socket�߳�
	static UINT ListenThreadFunc(LPVOID MsgPackageObjPointer); //Socket�����̷߳�������̬�����������߳�ʹ��,��������ģʽ
	static UINT ClientThreadFunc(LPVOID MsgPackageObjPointer); //Socket�ͻ����̷߳�������̬�����������߳�ʹ��,�ͻ���ģʽ
	bool ConnectServer(CString IP,int Port); //Socket���ӷ���������
	void AddLogDetails(CString DetailsText); //�����־��Ϣ����Ϣ�����������ʾ
};

