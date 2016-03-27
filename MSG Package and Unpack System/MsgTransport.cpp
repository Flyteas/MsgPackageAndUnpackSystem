#include "stdafx.h"
#include "MsgTransport.h"

MsgTransport::MsgTransport(CString* SendDetailEditValue,CString* ReceivedDetailEditValue,CDialogEx* MainWindowDlg) //���캯������ʼ������
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->MainWindow = MainWindowDlg;
	this->ListenStatus = false;
	this->ConnectStatus = false;
	this->ClientConnectFlag = false;
	this->ConnectServerFlag = false;
	this->MsgPackageObj = new MsgPackage(this->MsgSendDetail,this->MsgReceivedDetail,this->MainWindow);
}


MsgTransport::~MsgTransport(void) //����
{
	delete this->MsgPackageObj;
	this->MsgPackageObj = NULL;
}

void MsgTransport::SetListenStatus(bool ListenOpenStatus) //��������setter����,true��ʾ��������,false��ʾ�رռ���
{
	this->ListenStatus =  ListenOpenStatus;
}

void MsgTransport::SetServerPort(int Port)
{
	this->ServerPort = Port;
}

void MsgTransport::SetServerIP(CString IP)
{
	this->ServerIP = IP;
}

bool MsgTransport::StartListen(int Port) //������ģʽ����������
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

void MsgTransport::StopListen() //������ģʽ���رռ���
{
	this->ListenStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //�ȴ������߳̽���
	this->SocketThread = NULL;
}

UINT MsgTransport::ListenThreadFunc(LPVOID MsgTransportObjPointer) //��������ģʽ�̷߳���
{
	MsgTransport *MsgTransportObj = (MsgTransport*)MsgTransportObjPointer;
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
	if(!ServerSocket.Bind(MsgTransportObj->ServerPort)) //�󶨶˿�
	{
		MessageBoxA(0,"�󶨶˿�ʧ��!","����",0);
		return 3;
	}
	if(!ServerSocket.Listen()) //����һ������
	{
		MessageBoxA(0,"��������ʧ��","����",0);
		return 4;
	}
	while(MsgTransportObj->ListenStatus)
	{
		if(!MsgTransportObj->ClientConnectFlag) //���û�пͻ��������ӣ���Accept�ͻ��˵�����
		{
			MsgTransportObj->ConnectSocket = new CAsyncSocket();
			if(ServerSocket.Accept(*MsgTransportObj->ConnectSocket)) //���յ�һ���ͻ�������
			{
				MsgTransportObj->AddLogDetails("�ͻ���������");
				MsgTransportObj->ClientConnectFlag = true;
			}
			continue;
		}
		char RecBuff[2048]; //�������ݻ����
		memset(RecBuff,0,2048); //��ʼ�������
		int ReceiveResult;
		ReceiveResult = MsgTransportObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //���Ӵ���
		{
			MsgTransportObj->ClientConnectFlag = false;
			MsgTransportObj->AddLogDetails("�����ѶϿ�,�ȴ��ͻ���������");
			if(MsgTransportObj->ConnectSocket != NULL)
			{
				MsgTransportObj->ConnectSocket->Close();
				delete MsgTransportObj->ConnectSocket;
				MsgTransportObj->ConnectSocket = NULL;
			}
			continue;
		}
		else if(ReceiveResult == -1) //������ģʽ�Է�δ������Ϣ�򷵻�-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		MsgTransportObj->MsgPackageObj->UnpackMessage(RecMsg); //���װ���ݰ�
	}
	if(MsgTransportObj->ConnectSocket != NULL)
	{
		MsgTransportObj->ConnectSocket->Close();
		delete MsgTransportObj->ConnectSocket;
		MsgTransportObj->ConnectSocket = NULL;
	}
	MsgTransportObj->ClientConnectFlag = false;
	ServerSocket.Close();
	return 0;
}

bool MsgTransport::StartConnectServer(CString IP,int Port) //�ͻ���ģʽ�����ӷ�����
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

void MsgTransport::StopConnectServer() //�ͻ���ģʽ��ֹͣ���ӷ�����
{
	this->ConnectStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //�ȴ������߳̽���
	this->SocketThread = NULL;
}

UINT MsgTransport::ClientThreadFunc(LPVOID MsgTransportObjPointer) //�ͻ���ģʽ�̷߳���
{
	MsgTransport *MsgTransportObj = (MsgTransport*)MsgTransportObjPointer;
	if(!AfxSocketInit()) //��ʼ��Socket
	{
		MessageBoxA(0,"��ʼ��Socketʧ��!","����",0);
		return 1;
	}
	while(MsgTransportObj->ConnectStatus)
	{
		if(!MsgTransportObj->ConnectServerFlag) //���û�����ӷ��������������ӷ�����
		{
			MsgTransportObj->ConnectSocket = new CAsyncSocket();
			if(!MsgTransportObj->ConnectSocket->Create()) //����Socket
			{
				MessageBox(0,"����Socketʧ��!","����",0);
				delete MsgTransportObj->ConnectSocket;
				MsgTransportObj->ConnectSocket = NULL;
				return 2;
			}
			MsgTransportObj->ConnectSocket->Connect(MsgTransportObj->ServerIP,MsgTransportObj->ServerPort);
			MsgTransportObj->ConnectServerFlag = true;
			Sleep(3000);
			MsgTransportObj->AddLogDetails("���ӷ������ɹ�!");
		}
		char RecBuff[2048]; //�������ݻ����
		memset(RecBuff,0,2048); //��ʼ�������
		int ReceiveResult;
		ReceiveResult = MsgTransportObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //���Ӵ���
		{
			MsgTransportObj->AddLogDetails("�����ѶϿ�,����������!");
			MsgTransportObj->ConnectServerFlag = false;
			if(MsgTransportObj->ConnectSocket != NULL)
			{
				MsgTransportObj->ConnectSocket->Close();
				delete MsgTransportObj->ConnectSocket;
				MsgTransportObj->ConnectSocket = NULL;
			}
			MsgTransportObj->ConnectServerFlag = false;
			return 3;
		}
		else if(ReceiveResult == -1) //������ģʽ�Է�Ϊ������Ϣ�򷵻�-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		MsgTransportObj->MsgPackageObj->UnpackMessage(RecMsg); //���װ���ݰ�
	}
	if(MsgTransportObj->ConnectSocket != NULL)
	{
		MsgTransportObj->ConnectSocket->Close();
		delete MsgTransportObj->ConnectSocket;
		MsgTransportObj->ConnectSocket = NULL;
	}
	MsgTransportObj->ConnectServerFlag = false;
	return 0;
}


bool MsgTransport::SendPackage(CString PackageData) //��������
{
	if(this->ConnectSocket == NULL)
	{
		return false;
	}
	PackageData = this->MsgPackageObj->PackageMessage(PackageData); //��װ���ݰ�
	if(this->ConnectSocket->Send(PackageData,PackageData.GetLength()) <0 ) //��������
	{
		return false;
	}
	this->AddLogDetails("���ͳɹ�!");
	return true;
}

void MsgTransport::AddLogDetails(CString DetailsText) //�����Ϣ����Ϣ�����������ʾ
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgSendDetail = *this->MsgSendDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}
