#include "stdafx.h"
#include "MsgPackage.h"


MsgPackage::MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue) //构造函数，初始化数据
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->ListenFlag = true;
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

bool MsgPackage::Listen(int Port) //服务器模式，开启监听
{
	CSocket Sockets;
	CSocket SocketServer;
	if(!AfxSocketInit()) //初始化Socket
	{
		MessageBoxA(0,"初始化Socket失败!","错误",0);
		return false;
	}
	if(!Sockets.Socket()) //创建Socket
	{
		MessageBoxA(0,"创建Socket失败!","错误",0);
		return false;
	}
	bool SocketOpt = true;
	int SocketOptLength = sizeof(SocketOpt);
	Sockets.SetSockOpt(SO_REUSEADDR,&SocketOpt,SocketOptLength); //设置Socket选项
	if(!Sockets.Bind(Port)) //绑定端口
	{
		MessageBoxA(0,"绑定端口失败!","错误",0);
		return false;
	}
	if(!Sockets.Listen()) //开启一个监听
	{
		MessageBoxA(0,"开启监听失败","错误",0);
		return false;
	}

	while(this->ListenFlag)
	{
		if(!Sockets.Accept(SocketServer))
		{
			MessageBoxA(0,"Accept connect error!","Error",0);
			continue;
		}
		char RecBuff[2048]; //接收数据缓冲池
		memset(RecBuff,0,2048); //初始化缓冲池
		SocketServer.Receive(RecBuff,2048);
		CString RecMsg(RecBuff);
		MessageBox(0,RecMsg,NULL,0);
		SocketServer.Close();
	}

	Sockets.Close();
}

bool MsgPackage::SendPackage(CString PackageData) //发送数据
{
	return true;
}