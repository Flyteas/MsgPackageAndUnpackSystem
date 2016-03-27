#pragma once
/* 数据包封装与解封类 */
class MsgPackage
{
public:
	MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg);
	~MsgPackage(void);
	CString PackageMessage(CString SourceMsg); //封装数据
	bool UnpackMessage(CString SourceMsg); //解封装数据

private:
	CString *MsgSendDetail; //消息发送文本框文本内容
	CString *MsgReceivedDetail; //消息接收文本框文本内容
	CDialogEx* MainWindow; //主窗口

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

	void AddMsgSendDetails(CString DetailsText); //添加消息到消息发送详情框显示
	void AddMsgReceiveDetails(CString DetailsText); //添加消息到消息接收详情框显示
};

