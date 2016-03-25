#pragma once
/* 消息包封装与解封类 */
class MsgPackage : public CSocket
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue);
	~MsgPackage(void);
	bool PackageMessage(CString SourceMsg); //封装数据
	bool UnpackMessage(CString SourceMsg); //解封装数据
	void SetListenStatus(bool ListenOpenStatus); //监听开关setter方法,true表示开启监听,false表示关闭监听
	void SetListenPort(int Port); //监听端口setter方法
	bool StartListen(int Port); //服务器模式，开启一个监听
	void StopListen(); //服务器模式，停止监听
	bool StartConnectServer(CString IP,int Port); //客户端模式，连接服务器
	bool StopConnectServer(); //客户端模式，停止连接服务器

private:
	CString *MsgSendDetail; //消息封装发送细节
	CString *MsgReceivedDetail; //消息接收解封细节
	bool ListenStatus; //监听开启标识
	int ListenPort;
	CSocket *ClientSocket;
	bool ClientConnectFlag;

	CString MsgPackageApplication(CString SourceMsg); //应用层封装
	CString MsgPackageTransport(CString SourceMsg); //传输层封装
	CString MsgPackageNetwork(CString SourceMsg); //网络层封装
	CString MsgPackageDatalink(CString SourveMsg); //数据链路层封装
	CString MsgPackagePhysical(CString SourceMsg); //物理层封装

	CString MsgUnpackApplication(CString SourceMsg); //应用层解封装
	CString MsgUnpackTransport(CString SourceMsg); //传输层解封装
	CString MsgUnpackNetwork(CString SourceMsg); //网络层解封装
	CString MsgUnpackDatalink(CString SourveMsg); //数据链路层解封装
	CString MsgUnpackPhysical(CString SourceMsg); //物理层解封装

	static UINT ListenThreadFunc(LPVOID Port); //Socket监听线程方法，静态方法，供多线程使用
	bool SendPackage(CString PackageData); //发送一个数据包
};

