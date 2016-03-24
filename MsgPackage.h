#pragma once
/* ��Ϣ����װ������ */
class MsgPackage
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue);
	~MsgPackage(void);
	bool PackageMessage(CString SourceMsg); //��װ����
	bool UnpackMessage(CString SourceMsg); //���װ����
	bool Listen(int Port); //������ģʽ������һ������
	bool ConnectServer(CString IP,int Port); //�ͻ���ģʽ�����ӷ�����

private:
	CString *MsgSendDetail; //��Ϣ��װ����ϸ��
	CString *MsgReceivedDetail; //��Ϣ���ս��ϸ��
	bool ListenFlag; //����������ʶ

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

	bool SendPackage(CString PackageData); //����һ�����ݰ�
};

