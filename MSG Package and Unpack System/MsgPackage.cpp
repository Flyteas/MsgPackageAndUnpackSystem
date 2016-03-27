#include "stdafx.h"
#include "MsgPackage.h"

MsgPackage::MsgPackage(CString* SendDetailEditValue,CString* ReceivedDetailEditValue,CDialogEx* MainWindowDlg) //���캯������ʼ������
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->MainWindow = MainWindowDlg;
	this->ListenStatus = false;
	this->ConnectStatus = false;
	this->ClientConnectFlag = false;
	this->ConnectServerFlag = false;
}


MsgPackage::~MsgPackage(void)
{
}

bool MsgPackage::PackageMessage(CString SourceMsg) //��װ����
{
	CString PackagedMsg;
	PackagedMsg = this->MsgPackageApplication(SourceMsg); //����Ӧ�ò�����ݽ��з�װ
	PackagedMsg = this->MsgPackageTransport(PackagedMsg); //���ô��������ݽ��з�װ
	PackagedMsg = this->MsgPackageNetwork(PackagedMsg); //�������������ݽ��з�װ
	PackagedMsg = this->MsgPackageDatalink(PackagedMsg); //����������·������ݽ��з�װ
	PackagedMsg = this->MsgPackagePhysical(PackagedMsg); //�������������ݽ��з�װ
	return true;
}

bool MsgPackage::UnpackMessage(CString SourceMsg) //���װ����
{
	CString UnpackedMsg;
	UnpackedMsg = this->MsgUnpackPhysical(SourceMsg); //�������������ݽ��н��װ
	UnpackedMsg = this->MsgUnpackDatalink(UnpackedMsg); //����������·������ݽ��н��װ
	UnpackedMsg = this->MsgUnpackNetwork(UnpackedMsg); //�������������ݽ��н��װ
	UnpackedMsg = this->MsgUnpackTransport(UnpackedMsg); //���ô��������ݽ��н��װ
	UnpackedMsg = this->MsgUnpackApplication(UnpackedMsg); //����Ӧ�ò�����ݽ��н��װ
	return true;
}

CString MsgPackage::MsgPackageApplication(CString SourceMsg) //Ӧ�ò��װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgPackageTransport(CString SourceMsg) //������װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgPackageNetwork(CString SourceMsg) //������װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgPackageDatalink(CString SourceMsg) //������·���װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgPackagePhysical(CString SourceMsg) //������װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackApplication(CString SourceMsg) //Ӧ�ò���װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackTransport(CString SourceMsg) //�������װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackNetwork(CString SourceMsg) //�������װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackDatalink(CString SourceMsg) //������·����װ����
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackPhysical(CString SourceMsg) //�������װ����
{
	return SourceMsg;
}

void MsgPackage::SetListenStatus(bool ListenOpenStatus) //��������setter����,true��ʾ��������,false��ʾ�رռ���
{
	this->ListenStatus =  ListenOpenStatus;
}

void MsgPackage::SetServerPort(int Port)
{
	this->ServerPort = Port;
}

void MsgPackage::SetServerIP(CString IP)
{
	this->ServerIP = IP;
}

bool MsgPackage::StartListen(int Port) //������ģʽ����������
{
	this->ListenStatus = true;
	this->SetServerPort(Port);
	this->SocketThread = AfxBeginThread(this->ListenThreadFunc,(LPVOID)this); //���������߳�
	if(this->SocketThread == NULL) //���������߳�ʧ��
	{
		return false;
	}
	return true;
}

void MsgPackage::StopListen() //������ģʽ���رռ���
{
	this->ListenStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //�ȴ������߳̽���
	this->SocketThread = NULL;
}

UINT MsgPackage::ListenThreadFunc(LPVOID MsgPackageObjPointer) //��������ģʽ�̷߳���
{
	MsgPackage *MsgPackageObj = (MsgPackage*)MsgPackageObjPointer;
	CAsyncSocket ServerSocket;
	if(!AfxSocketInit()) //��ʼ��Socket
	{
		MessageBoxA(0,"��ʼ��Socketʧ��!","����",0);
		return 1;
	}
	if(!ServerSocket.Socket()) //����Socket
	{
		MessageBoxA(0,"����Socketʧ��!","����",0);
		return 2;
	}
	bool SocketOpt = true;
	int SocketOptLength = sizeof(SocketOpt);
	ServerSocket.SetSockOpt(SO_REUSEADDR,&SocketOpt,SocketOptLength); //����Socketѡ��
	if(!ServerSocket.Bind(MsgPackageObj->ServerPort)) //�󶨶˿�
	{
		MessageBoxA(0,"�󶨶˿�ʧ��!","����",0);
		return 3;
	}
	if(!ServerSocket.Listen()) //����һ������
	{
		MessageBoxA(0,"��������ʧ��","����",0);
		return 4;
	}
	while(MsgPackageObj->ListenStatus)
	{
		if(!MsgPackageObj->ClientConnectFlag) //���û�пͻ��������ӣ���Accept�ͻ��˵�����
		{
			MsgPackageObj->ConnectSocket = new CAsyncSocket();
			if(ServerSocket.Accept(*MsgPackageObj->ConnectSocket)) //���յ�һ���ͻ�������
			{
				MsgPackageObj->AddMsgSendDetails("�ͻ���������");
				MsgPackageObj->ClientConnectFlag = true;
			}
			continue;
		}
		char RecBuff[2048]; //�������ݻ����
		memset(RecBuff,0,2048); //��ʼ�������
		int ReceiveResult;
		ReceiveResult = MsgPackageObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //���Ӵ���
		{
			MsgPackageObj->ClientConnectFlag = false;
			MsgPackageObj->AddMsgSendDetails("�����ѶϿ�,�ȴ��ͻ���������");
			if(MsgPackageObj->ConnectSocket != NULL)
			{
				MsgPackageObj->ConnectSocket->Close();
				delete MsgPackageObj->ConnectSocket;
				MsgPackageObj->ConnectSocket = NULL;
			}
			continue;
		}
		else if(ReceiveResult == -1) //������ģʽ�Է�δ������Ϣ�򷵻�-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		RecMsg = "���յ����ݰ�: " + RecMsg;
		MsgPackageObj->AddMsgReceiveDetails(RecMsg);
	}
	if(MsgPackageObj->ConnectSocket != NULL)
	{
		MsgPackageObj->ConnectSocket->Close();
		delete MsgPackageObj->ConnectSocket;
		MsgPackageObj->ConnectSocket = NULL;
	}
	MsgPackageObj->ClientConnectFlag = false;
	ServerSocket.Close();
	return 0;
}

bool MsgPackage::StartConnectServer(CString IP,int Port) //�ͻ���ģʽ�����ӷ�����
{
	this->ConnectStatus = true;
	this->SetServerIP(IP);
	this->SetServerPort(Port);
	SocketThread = AfxBeginThread(this->ClientThreadFunc,(LPVOID)this); //���������߳�
	if(SocketThread == NULL) //���������߳�ʧ��
	{
		return false;
	}
	return true;
}

void MsgPackage::StopConnectServer() //�ͻ���ģʽ��ֹͣ���ӷ�����
{
	this->ConnectStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //�ȴ������߳̽���
	this->SocketThread = NULL;
}

UINT MsgPackage::ClientThreadFunc(LPVOID MsgPackageObjPointer) //�ͻ���ģʽ�̷߳���
{
	MsgPackage *MsgPackageObj = (MsgPackage*)MsgPackageObjPointer;
	if(!AfxSocketInit()) //��ʼ��Socket
	{
		MessageBoxA(0,"��ʼ��Socketʧ��!","����",0);
		return 1;
	}
	while(MsgPackageObj->ConnectStatus)
	{
		if(!MsgPackageObj->ConnectServerFlag) //���û�����ӷ��������������ӷ�����
		{
			MsgPackageObj->ConnectSocket = new CAsyncSocket();
			if(!MsgPackageObj->ConnectSocket->Create()) //����Socket
			{
				MessageBox(0,"����Socketʧ��!","����",0);
				delete MsgPackageObj->ConnectSocket;
				MsgPackageObj->ConnectSocket = NULL;
				return 2;
			}
			MsgPackageObj->ConnectSocket->Connect(MsgPackageObj->ServerIP,MsgPackageObj->ServerPort);
			MsgPackageObj->ConnectServerFlag = true;
			Sleep(3000);
			MsgPackageObj->AddMsgSendDetails("���ӷ������ɹ�!");
		}
		char RecBuff[2048]; //�������ݻ����
		memset(RecBuff,0,2048); //��ʼ�������
		int ReceiveResult;
		ReceiveResult = MsgPackageObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //���Ӵ���
		{
			MsgPackageObj->AddMsgSendDetails("�����ѶϿ�,����������!");
			MsgPackageObj->ConnectServerFlag = false;
			if(MsgPackageObj->ConnectSocket != NULL)
			{
				MsgPackageObj->ConnectSocket->Close();
				delete MsgPackageObj->ConnectSocket;
				MsgPackageObj->ConnectSocket = NULL;
			}
			MsgPackageObj->ConnectServerFlag = false;
			return 3;
		}
		else if(ReceiveResult == -1) //������ģʽ�Է�Ϊ������Ϣ�򷵻�-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		RecMsg = "���յ����ݰ�: " + RecMsg;
		MsgPackageObj->AddMsgReceiveDetails(RecMsg);
	}
	if(MsgPackageObj->ConnectSocket != NULL)
	{
		MsgPackageObj->ConnectSocket->Close();
		delete MsgPackageObj->ConnectSocket;
		MsgPackageObj->ConnectSocket = NULL;
	}
	MsgPackageObj->ConnectServerFlag = false;
	return 0;
}


bool MsgPackage::SendPackage(CString PackageData) //��������
{
	if(this->ConnectSocket == NULL)
	{
		return false;
	}
	if(this->ConnectSocket->Send(PackageData,PackageData.GetLength()) <0 ) //��������
	{
		return false;
	}
	return true;
}

void MsgPackage::AddMsgSendDetails(CString DetailsText) //�����Ϣ����Ϣ�����������ʾ
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgSendDetail = *this->MsgSendDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}
void MsgPackage::AddMsgReceiveDetails(CString DetailsText) //�����Ϣ����Ϣ�����������ʾ
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgReceivedDetail = *this->MsgReceivedDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}