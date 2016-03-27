#pragma once
/* 消息包封装与解封类 */
class MsgPackage
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg);
	~MsgPackage(void);
	bool PackageMessage(CString SourceMsg); //封装数据
	bool UnpackMessage(CString SourceMsg); //解封装数据
	void SetListenStatus(bool ListenOpenStatus); //监听开关setter方法,true表示开启监听,false表示关闭监听
	void SetServerPort(int Port); //服务器端口setter方法
	void SetServerIP(CString IP); //服务器IP setter方法
	bool StartListen(int Port); //服务器模式，开启一个监听
	void StopListen(); //服务器模式，停止监听
	bool StartConnectServer(CString IP,int Port); //客户端模式，连接服务器
	void StopConnectServer(); //客户端模式，停止连接服务器
	bool SendPackage(CString PackageData); //发送一个数据包
	void AddMsgSendDetails(CString DetailsText); //添加消息到消息发送详情框显示
	void AddMsgReceiveDetails(CString DetailsText); //添加消息到消息接收详情框显示

private:
	CString *MsgSendDetail; //消息发送文本框文本内容
	CString *MsgReceivedDetail; //消息接收文本框文本内容
	CDialogEx* MainWindow; //主窗口
	bool ListenStatus; //监听开启标识,服务器端模式
	bool ConnectStatus; //连接开启标识，客户端模式
	int ServerPort; //服务器端口
	CString ServerIP; //服务器IP
	CAsyncSocket *ConnectSocket;
	bool ClientConnectFlag; //服务器模式，客户端连接状态标识
	bool ConnectServerFlag; //客户端模式，连接服务器状态标识
	CWinThread *SocketThread; //Socket线程

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

	static UINT ListenThreadFunc(LPVOID MsgPackageObjPointer); //Socket监听线程方法，静态方法，供多线程使用,服务器端模式
	static UINT ClientThreadFunc(LPVOID MsgPackageObjPointer); //Socket客户端线程方法，静态方法，供多线程使用,客户端模式
	bool ConnectServer(CString IP,int Port); //Socket连接服务器方法
};

