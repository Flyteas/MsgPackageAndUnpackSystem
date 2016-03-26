#include "stdafx.h"
#include "MsgPackage.h"


MsgPackage::MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue) //���캯������ʼ������
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->ListenStatus = false;
	this->ClientConnectFlag = false;
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

void MsgPackage::SetListenPort(int Port)
{
	this->ListenPort = Port;
}

bool MsgPackage::StartListen(int Port) //������ģʽ����������
{
	this->ListenStatus = true;
	this->SetListenPort(Port);
	ListenThread = AfxBeginThread(this->ListenThreadFunc,(LPVOID)this); //���������߳�
	if(ListenThread == NULL) //���������߳�ʧ��
	{
		return false;
	}
	return true;
}

void MsgPackage::StopListen() //������ģʽ���رռ���
{
	WaitForSingleObject(ListenThread,INFINITE); //�ȴ������߳̽���
	this->ListenStatus = false;
}

UINT MsgPackage::ListenThreadFunc(LPVOID MsgPackageObjPointer)
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
	if(!ServerSocket.Bind(MsgPackageObj->ListenPort)) //�󶨶˿�
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
			if(MsgPackageObj->ConnectSocket != NULL)
			{
				MsgPackageObj->ConnectSocket->Close();
				delete MsgPackageObj->ConnectSocket;
				MsgPackageObj->ConnectSocket = NULL;
			}
			continue;
		}
		else if(ReceiveResult == -1) //������ģʽ�Է�Ϊ������Ϣ�򷵻�-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		MessageBox(0,RecMsg,NULL,0);
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
	return true;
}

bool MsgPackage::StopConnectServer() //�ͻ���ģʽ��ֹͣ���ӷ�����
{
	return true;
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
