#pragma once
/* ���ݰ���װ������ */
class MsgPackage
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg);
	~MsgPackage(void);
	CString PackageMessage(CString SourceMsg); //��װ����
	bool UnpackMessage(CString SourceMsg); //���װ����

private:
	CString *MsgSendDetail; //��Ϣ�����ı����ı�����
	CString *MsgReceivedDetail; //��Ϣ�����ı����ı�����
	CDialogEx* MainWindow; //������

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

	void AddMsgSendDetails(CString DetailsText); //�����Ϣ����Ϣ�����������ʾ
	void AddMsgReceiveDetails(CString DetailsText); //�����Ϣ����Ϣ�����������ʾ
};

