#include "stdafx.h"
#include "MsgPackage.h"


MsgPackage::MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue) //构造函数，初始化数据
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->ListenStatus = false;
	this->ClientConnectFlag = false;
}


MsgPackage::~MsgPackage(void)
{
}

bool MsgPackage::PackageMessage(CString SourceMsg) //封装数据
{
	CString PackagedMsg;
	PackagedMsg = this->MsgPackageApplication(SourceMsg); //调用应用层对数据进行封装
	PackagedMsg = this->MsgPackageTransport(PackagedMsg); //调用传输层对数据进行封装
	PackagedMsg = this->MsgPackageNetwork(PackagedMsg); //调用网络层对数据进行封装
	PackagedMsg = this->MsgPackageDatalink(PackagedMsg); //调用数据链路层对数据进行封装
	PackagedMsg = this->MsgPackagePhysical(PackagedMsg); //调用物理层对数据进行封装
	return true;
}

bool MsgPackage::UnpackMessage(CString SourceMsg) //解封装数据
{
	CString UnpackedMsg;
	UnpackedMsg = this->MsgUnpackPhysical(SourceMsg); //调用物理层对数据进行解封装
	UnpackedMsg = this->MsgUnpackDatalink(UnpackedMsg); //调用数据链路层对数据进行解封装
	UnpackedMsg = this->MsgUnpackNetwork(UnpackedMsg); //调用网络层对数据进行解封装
	UnpackedMsg = this->MsgUnpackTransport(UnpackedMsg); //调用传输层对数据进行解封装
	UnpackedMsg = this->MsgUnpackApplication(UnpackedMsg); //调用应用层对数据进行解封装
	return true;
}

CString MsgPackage::MsgPackageApplication(CString SourceMsg) //应用层封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgPackageTransport(CString SourceMsg) //传输层封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgPackageNetwork(CString SourceMsg) //网络层封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgPackageDatalink(CString SourceMsg) //数据链路层封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgPackagePhysical(CString SourceMsg) //物理层封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackApplication(CString SourceMsg) //应用层解封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackTransport(CString SourceMsg) //传输层解封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackNetwork(CString SourceMsg) //网络层解封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackDatalink(CString SourceMsg) //数据链路层解封装方法
{
	return SourceMsg;
}

CString MsgPackage::MsgUnpackPhysical(CString SourceMsg) //物理层解封装方法
{
	return SourceMsg;
}

void MsgPackage::SetListenStatus(bool ListenOpenStatus) //监听开关setter方法,true表示开启监听,false表示关闭监听
{
	this->ListenStatus =  ListenOpenStatus;
}

void MsgPackage::SetListenPort(int Port)
{
	this->ListenPort = Port;
}

bool MsgPackage::StartListen(int Port) //服务器模式，开启监听
{
	CWinThread *ListenThread;
	this->ListenStatus = true;
	this->SetListenPort(Port);
	ListenThread = AfxBeginThread(this->ListenThreadFunc,(LPVOID)this); //创建监听线程
	if(ListenThread == NULL) //创建监听线程失败
	{
		return false;
	}
	return true;
}

void MsgPackage::StopListen() //服务器模式，关闭监听
{
	this->ListenStatus = false;
	if(this->ClientSocket != NULL)
	{
		this->ClientSocket->Close();
		delete this->ClientSocket;
		this->ClientSocket = NULL;
	}
}

UINT MsgPackage::ListenThreadFunc(LPVOID MsgPackageObjPointer)
{
	MsgPackage *MsgPackageObj = (MsgPackage*)MsgPackageObjPointer;
	CSocket ServerSocket;
	if(!AfxSocketInit()) //初始化Socket
	{
		MessageBoxA(0,"初始化Socket失败!","错误",0);
		return 1;
	}
	if(!ServerSocket.Socket()) //创建Socket
	{
		MessageBoxA(0,"创建Socket失败!","错误",0);
		return 2;
	}
	bool SocketOpt = true;
	int SocketOptLength = sizeof(SocketOpt);
	ServerSocket.SetSockOpt(SO_REUSEADDR,&SocketOpt,SocketOptLength); //设置Socket选项
	if(!ServerSocket.Bind(MsgPackageObj->ListenPort)) //绑定端口
	{
		MessageBoxA(0,"绑定端口失败!","错误",0);
		return 3;
	}
	if(!ServerSocket.Listen()) //开启一个监听
	{
		MessageBoxA(0,"开启监听失败","错误",0);
		return 4;
	}
	while(MsgPackageObj->ListenStatus)
	{
		if(!MsgPackageObj->ClientConnectFlag) //如果没有客户端已连接，则Accept客户端的连接
		{
			MsgPackageObj->ClientSocket = new CSocket();
			if(!ServerSocket.Accept(*MsgPackageObj->ClientSocket)) //Accept客户端失败
			{
				MessageBoxA(0,"Accept connect error!","Error",0);
				break;
			}
			MsgPackageObj->ClientConnectFlag = true;
		}
		char RecBuff[2048]; //接收数据缓冲池
		memset(RecBuff,0,2048); //初始化缓冲池
		if(MsgPackageObj->ClientSocket->Receive(RecBuff,2048) <= 0) //连接错误
		{
			MsgPackageObj->ClientConnectFlag = false;
			if(MsgPackageObj->ClientSocket != NULL)
			{
				MsgPackageObj->ClientSocket->Close();
				delete MsgPackageObj->ClientSocket;
				MsgPackageObj->ClientSocket = NULL;
			}
			continue;
		}
		CString RecMsg(RecBuff);
		MessageBox(0,RecMsg,NULL,0);
	}
	if(MsgPackageObj->ClientSocket != NULL)
	{
		MsgPackageObj->ClientSocket->Close();
		delete MsgPackageObj->ClientSocket;
		MsgPackageObj->ClientSocket = NULL;
	}
	ServerSocket.Close();
	MsgPackageObj->ClientConnectFlag = false;
	return 0;
}

bool MsgPackage::StartConnectServer(CString IP,int Port) //客户端模式，连接服务器
{
	return true;
}

bool MsgPackage::StopConnectServer() //客户端模式，停止连接服务器
{
	return true;
}

bool MsgPackage::SendPackage(CString PackageData) //发送数据
{
	return true;
}
