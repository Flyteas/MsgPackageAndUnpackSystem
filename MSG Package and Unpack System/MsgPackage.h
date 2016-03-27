#pragma once
/* ��Ϣ����װ������ */
class MsgPackage
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg);
	~MsgPackage(void);
	bool PackageMessage(CString SourceMsg); //��װ����
	bool UnpackMessage(CString SourceMsg); //���װ����
	void SetListenStatus(bool ListenOpenStatus); //��������setter����,true��ʾ��������,false��ʾ�رռ���
	void SetServerPort(int Port); //�������˿�setter����
	void SetServerIP(CString IP); //������IP setter����
	bool StartListen(int Port); //������ģʽ������һ������
	void StopListen(); //������ģʽ��ֹͣ����
	bool StartConnectServer(CString IP,int Port); //�ͻ���ģʽ�����ӷ�����
	void StopConnectServer(); //�ͻ���ģʽ��ֹͣ���ӷ�����
	bool SendPackage(CString PackageData); //����һ�����ݰ�
	void AddMsgSendDetails(CString DetailsText); //�����Ϣ����Ϣ�����������ʾ
	void AddMsgReceiveDetails(CString DetailsText); //�����Ϣ����Ϣ�����������ʾ

private:
	CString *MsgSendDetail; //��Ϣ�����ı����ı�����
	CString *MsgReceivedDetail; //��Ϣ�����ı����ı�����
	CDialogEx* MainWindow; //������
	bool ListenStatus; //����������ʶ,��������ģʽ
	bool ConnectStatus; //���ӿ�����ʶ���ͻ���ģʽ
	int ServerPort; //�������˿�
	CString ServerIP; //������IP
	CAsyncSocket *ConnectSocket;
	bool ClientConnectFlag; //������ģʽ���ͻ�������״̬��ʶ
	bool ConnectServerFlag; //�ͻ���ģʽ�����ӷ�����״̬��ʶ
	CWinThread *SocketThread; //Socket�߳�

	CString MsgPackageApplication(CString SourceMsg); //Ӧ�ò��װ
	CString MsgPackageTransport(CString SourceMsg); //������װ
	CString MsgPackageNetwork(CString SourceMsg); //������װ
	CString MsgPackageDatalink(CString SourveMsg); //������·���װ
	CString MsgPackagePhysical(CString SourceMsg); //������װ

	CString MsgUnpackApplication(CString SourceMsg); //Ӧ�ò���װ
	CString MsgUnpackTransport(CString SourceMsg); //�������װ
	CString MsgUnpackNetwork(CString SourceMsg); //�������װ
	CString MsgUnpackDatalink(CString SourveMsg); //������·����װ
	CString MsgUnpackPhysical(CString SourceMsg); //�������װ

	static UINT ListenThreadFunc(LPVOID MsgPackageObjPointer); //Socket�����̷߳�������̬�����������߳�ʹ��,��������ģʽ
	static UINT ClientThreadFunc(LPVOID MsgPackageObjPointer); //Socket�ͻ����̷߳�������̬�����������߳�ʹ��,�ͻ���ģʽ
	bool ConnectServer(CString IP,int Port); //Socket���ӷ���������
};

