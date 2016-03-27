#include "stdafx.h"
#include "MsgTransport.h"

MsgTransport::MsgTransport(CString* SendDetailEditValue,CString* ReceivedDetailEditValue,CDialogEx* MainWindowDlg) //构造函数，初始化数据
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


MsgTransport::~MsgTransport(void) //析构
{
	delete this->MsgPackageObj;
	this->MsgPackageObj = NULL;
}

void MsgTransport::SetListenStatus(bool ListenOpenStatus) //监听开关setter方法,true表示开启监听,false表示关闭监听
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

bool MsgTransport::StartListen(int Port) //服务器模式，开启监听
{
	this->ListenStatus = true;
	this->SetServerPort(Port);
	this->SocketThread = AfxBeginThread(this->ListenThreadFunc,(LPVOID)this); //创建监听线程
	if(this->SocketThread == NULL) //创建监听线程失败
	{
		return false;
	}
	return true;
}

void MsgTransport::StopListen() //服务器模式，关闭监听
{
	this->ListenStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //等待监听线程结束
	this->SocketThread = NULL;
}

UINT MsgTransport::ListenThreadFunc(LPVOID MsgTransportObjPointer) //服务器端模式线程方法
{
	MsgTransport *MsgTransportObj = (MsgTransport*)MsgTransportObjPointer;
	CAsyncSocket ServerSocket;
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
	if(!ServerSocket.Bind(MsgTransportObj->ServerPort)) //绑定端口
	{
		MessageBoxA(0,"绑定端口失败!","错误",0);
		return 3;
	}
	if(!ServerSocket.Listen()) //开启一个监听
	{
		MessageBoxA(0,"开启监听失败","错误",0);
		return 4;
	}
	while(MsgTransportObj->ListenStatus)
	{
		if(!MsgTransportObj->ClientConnectFlag) //如果没有客户端已连接，则Accept客户端的连接
		{
			MsgTransportObj->ConnectSocket = new CAsyncSocket();
			if(ServerSocket.Accept(*MsgTransportObj->ConnectSocket)) //接收到一个客户端连接
			{
				MsgTransportObj->AddLogDetails("客户端已连接");
				MsgTransportObj->ClientConnectFlag = true;
			}
			continue;
		}
		char RecBuff[2048]; //接收数据缓冲池
		memset(RecBuff,0,2048); //初始化缓冲池
		int ReceiveResult;
		ReceiveResult = MsgTransportObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //连接错误
		{
			MsgTransportObj->ClientConnectFlag = false;
			MsgTransportObj->AddLogDetails("连接已断开,等待客户端连接中");
			if(MsgTransportObj->ConnectSocket != NULL)
			{
				MsgTransportObj->ConnectSocket->Close();
				delete MsgTransportObj->ConnectSocket;
				MsgTransportObj->ConnectSocket = NULL;
			}
			continue;
		}
		else if(ReceiveResult == -1) //非阻塞模式对方未发送消息则返回-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		MsgTransportObj->MsgPackageObj->UnpackMessage(RecMsg); //解封装数据包
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

bool MsgTransport::StartConnectServer(CString IP,int Port) //客户端模式，连接服务器
{
	this->ConnectStatus = true;
	this->SetServerIP(IP);
	this->SetServerPort(Port);
	SocketThread = AfxBeginThread(this->ClientThreadFunc,(LPVOID)this); //创建监听线程
	if(SocketThread == NULL) //创建监听线程失败
	{
		return false;
	}
	return true;
}

void MsgTransport::StopConnectServer() //客户端模式，停止连接服务器
{
	this->ConnectStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //等待监听线程结束
	this->SocketThread = NULL;
}

UINT MsgTransport::ClientThreadFunc(LPVOID MsgTransportObjPointer) //客户端模式线程方法
{
	MsgTransport *MsgTransportObj = (MsgTransport*)MsgTransportObjPointer;
	if(!AfxSocketInit()) //初始化Socket
	{
		MessageBoxA(0,"初始化Socket失败!","错误",0);
		return 1;
	}
	while(MsgTransportObj->ConnectStatus)
	{
		if(!MsgTransportObj->ConnectServerFlag) //如果没有连接服务器，则尝试连接服务器
		{
			MsgTransportObj->ConnectSocket = new CAsyncSocket();
			if(!MsgTransportObj->ConnectSocket->Create()) //创建Socket
			{
				MessageBox(0,"创建Socket失败!","错误",0);
				delete MsgTransportObj->ConnectSocket;
				MsgTransportObj->ConnectSocket = NULL;
				return 2;
			}
			MsgTransportObj->ConnectSocket->Connect(MsgTransportObj->ServerIP,MsgTransportObj->ServerPort);
			MsgTransportObj->ConnectServerFlag = true;
			Sleep(3000);
			MsgTransportObj->AddLogDetails("连接服务器成功!");
		}
		char RecBuff[2048]; //接收数据缓冲池
		memset(RecBuff,0,2048); //初始化缓冲池
		int ReceiveResult;
		ReceiveResult = MsgTransportObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //连接错误
		{
			MsgTransportObj->AddLogDetails("连接已断开,请重新连接!");
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
		else if(ReceiveResult == -1) //非阻塞模式对方为发送消息则返回-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		MsgTransportObj->MsgPackageObj->UnpackMessage(RecMsg); //解封装数据包
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


bool MsgTransport::SendPackage(CString PackageData) //发送数据
{
	if(this->ConnectSocket == NULL)
	{
		return false;
	}
	PackageData = this->MsgPackageObj->PackageMessage(PackageData); //封装数据包
	if(this->ConnectSocket->Send(PackageData,PackageData.GetLength()) <0 ) //发送数据
	{
		return false;
	}
	this->AddLogDetails("发送成功!");
	return true;
}

void MsgTransport::AddLogDetails(CString DetailsText) //添加消息到消息发送详情框显示
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgSendDetail = *this->MsgSendDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}
