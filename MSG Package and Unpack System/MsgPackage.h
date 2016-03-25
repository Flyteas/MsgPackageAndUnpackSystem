#pragma once
/* ��Ϣ����װ������ */
class MsgPackage : public CSocket
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue);
	~MsgPackage(void);
	bool PackageMessage(CString SourceMsg); //��װ����
	bool UnpackMessage(CString SourceMsg); //���װ����
	void SetListenStatus(bool ListenOpenStatus); //��������setter����,true��ʾ��������,false��ʾ�رռ���
	void SetListenPort(int Port); //�����˿�setter����
	bool StartListen(int Port); //������ģʽ������һ������
	void StopListen(); //������ģʽ��ֹͣ����
	bool StartConnectServer(CString IP,int Port); //�ͻ���ģʽ�����ӷ�����
	bool StopConnectServer(); //�ͻ���ģʽ��ֹͣ���ӷ�����

private:
	CString *MsgSendDetail; //��Ϣ��װ����ϸ��
	CString *MsgReceivedDetail; //��Ϣ���ս��ϸ��
	bool ListenStatus; //����������ʶ
	int ListenPort;
	CSocket *ClientSocket;
	bool ClientConnectFlag;

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

	static UINT ListenThreadFunc(LPVOID Port); //Socket�����̷߳�������̬�����������߳�ʹ��
	bool SendPackage(CString PackageData); //����һ�����ݰ�
};

