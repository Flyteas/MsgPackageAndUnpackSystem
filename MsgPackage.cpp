#include "stdafx.h"
#include "MsgPackage.h"


MsgPackage::MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue) //���캯������ʼ������
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->ListenFlag = true;
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

bool MsgPackage::Listen(int Port) //������ģʽ����������
{
	CSocket Sockets;
	CSocket SocketServer;
	if(!AfxSocketInit()) //��ʼ��Socket
	{
		MessageBoxA(0,"��ʼ��Socketʧ��!","����",0);
		return false;
	}
	if(!Sockets.Socket()) //����Socket
	{
		MessageBoxA(0,"����Socketʧ��!","����",0);
		return false;
	}
	bool SocketOpt = true;
	int SocketOptLength = sizeof(SocketOpt);
	Sockets.SetSockOpt(SO_REUSEADDR,&SocketOpt,SocketOptLength); //����Socketѡ��
	if(!Sockets.Bind(Port)) //�󶨶˿�
	{
		MessageBoxA(0,"�󶨶˿�ʧ��!","����",0);
		return false;
	}
	if(!Sockets.Listen()) //����һ������
	{
		MessageBoxA(0,"��������ʧ��","����",0);
		return false;
	}

	while(this->ListenFlag)
	{
		if(!Sockets.Accept(SocketServer))
		{
			MessageBoxA(0,"Accept connect error!","Error",0);
			continue;
		}
		char RecBuff[2048]; //�������ݻ����
		memset(RecBuff,0,2048); //��ʼ�������
		SocketServer.Receive(RecBuff,2048);
		CString RecMsg(RecBuff);
		MessageBox(0,RecMsg,NULL,0);
		SocketServer.Close();
	}

	Sockets.Close();
}

bool MsgPackage::SendPackage(CString PackageData) //��������
{
	return true;
}