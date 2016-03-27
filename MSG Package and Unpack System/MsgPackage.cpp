#include "stdafx.h"
#include "MsgPackage.h"

MsgPackage::MsgPackage(CString* SendDetailEditValue,CString* ReceivedDetailEditValue,CDialogEx* MainWindowDlg) //构造函数，初始化数据
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

void MsgPackage::SetServerPort(int Port)
{
	this->ServerPort = Port;
}

void MsgPackage::SetServerIP(CString IP)
{
	this->ServerIP = IP;
}

bool MsgPackage::StartListen(int Port) //服务器模式，开启监听
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

void MsgPackage::StopListen() //服务器模式，关闭监听
{
	this->ListenStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //等待监听线程结束
	this->SocketThread = NULL;
}

UINT MsgPackage::ListenThreadFunc(LPVOID MsgPackageObjPointer) //服务器端模式线程方法
{
	MsgPackage *MsgPackageObj = (MsgPackage*)MsgPackageObjPointer;
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
	if(!ServerSocket.Bind(MsgPackageObj->ServerPort)) //绑定端口
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
			MsgPackageObj->ConnectSocket = new CAsyncSocket();
			if(ServerSocket.Accept(*MsgPackageObj->ConnectSocket)) //接收到一个客户端连接
			{
				MsgPackageObj->AddMsgSendDetails("客户端已连接");
				MsgPackageObj->ClientConnectFlag = true;
			}
			continue;
		}
		char RecBuff[2048]; //接收数据缓冲池
		memset(RecBuff,0,2048); //初始化缓冲池
		int ReceiveResult;
		ReceiveResult = MsgPackageObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //连接错误
		{
			MsgPackageObj->ClientConnectFlag = false;
			MsgPackageObj->AddMsgSendDetails("连接已断开,等待客户端连接中");
			if(MsgPackageObj->ConnectSocket != NULL)
			{
				MsgPackageObj->ConnectSocket->Close();
				delete MsgPackageObj->ConnectSocket;
				MsgPackageObj->ConnectSocket = NULL;
			}
			continue;
		}
		else if(ReceiveResult == -1) //非阻塞模式对方未发送消息则返回-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		RecMsg = "接收到数据包: " + RecMsg;
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

bool MsgPackage::StartConnectServer(CString IP,int Port) //客户端模式，连接服务器
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

void MsgPackage::StopConnectServer() //客户端模式，停止连接服务器
{
	this->ConnectStatus = false;
	WaitForSingleObject(this->SocketThread,INFINITE); //等待监听线程结束
	this->SocketThread = NULL;
}

UINT MsgPackage::ClientThreadFunc(LPVOID MsgPackageObjPointer) //客户端模式线程方法
{
	MsgPackage *MsgPackageObj = (MsgPackage*)MsgPackageObjPointer;
	if(!AfxSocketInit()) //初始化Socket
	{
		MessageBoxA(0,"初始化Socket失败!","错误",0);
		return 1;
	}
	while(MsgPackageObj->ConnectStatus)
	{
		if(!MsgPackageObj->ConnectServerFlag) //如果没有连接服务器，则尝试连接服务器
		{
			MsgPackageObj->ConnectSocket = new CAsyncSocket();
			if(!MsgPackageObj->ConnectSocket->Create()) //创建Socket
			{
				MessageBox(0,"创建Socket失败!","错误",0);
				delete MsgPackageObj->ConnectSocket;
				MsgPackageObj->ConnectSocket = NULL;
				return 2;
			}
			MsgPackageObj->ConnectSocket->Connect(MsgPackageObj->ServerIP,MsgPackageObj->ServerPort);
			MsgPackageObj->ConnectServerFlag = true;
			Sleep(3000);
			MsgPackageObj->AddMsgSendDetails("连接服务器成功!");
		}
		char RecBuff[2048]; //接收数据缓冲池
		memset(RecBuff,0,2048); //初始化缓冲池
		int ReceiveResult;
		ReceiveResult = MsgPackageObj->ConnectSocket->Receive(RecBuff,2048);
		if(ReceiveResult < -1 || ReceiveResult == 0) //连接错误
		{
			MsgPackageObj->AddMsgSendDetails("连接已断开,请重新连接!");
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
		else if(ReceiveResult == -1) //非阻塞模式对方为发送消息则返回-1
		{
			continue;
		}
		CString RecMsg(RecBuff);
		RecMsg = "接收到数据包: " + RecMsg;
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


bool MsgPackage::SendPackage(CString PackageData) //发送数据
{
	if(this->ConnectSocket == NULL)
	{
		return false;
	}
	if(this->ConnectSocket->Send(PackageData,PackageData.GetLength()) <0 ) //发送数据
	{
		return false;
	}
	return true;
}

void MsgPackage::AddMsgSendDetails(CString DetailsText) //添加消息到消息发送详情框显示
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgSendDetail = *this->MsgSendDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}
void MsgPackage::AddMsgReceiveDetails(CString DetailsText) //添加消息到消息接收详情框显示
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgReceivedDetail = *this->MsgReceivedDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}