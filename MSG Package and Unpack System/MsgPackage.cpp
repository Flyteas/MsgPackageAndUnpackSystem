#include "stdafx.h"
#include "MsgPackage.h"


MsgPackage::MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg) //构造函数，初始化数据
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->MainWindow = MainWindowDlg;
}


MsgPackage::~MsgPackage(void)
{
}

CString MsgPackage::PackageMessage(CString SourceMsg) //封装数据
{
	CString PackagedMsg;
	this->AddMsgSendDetails("原始数据: " + SourceMsg);
	PackagedMsg = this->MsgPackageApplication(SourceMsg); //调用应用层对数据进行封装
	this->AddMsgSendDetails("应用层封装: " + PackagedMsg);
	PackagedMsg = this->MsgPackageTransport(PackagedMsg); //调用传输层对数据进行封装
	this->AddMsgSendDetails("传输层封装: " + PackagedMsg);
	PackagedMsg = this->MsgPackageNetwork(PackagedMsg); //调用网络层对数据进行封装
	this->AddMsgSendDetails("网络层封装: " + PackagedMsg);
	PackagedMsg = this->MsgPackageDatalink(PackagedMsg); //调用数据链路层对数据进行封装
	this->AddMsgSendDetails("数据链路层封装: " + PackagedMsg);
	PackagedMsg = this->MsgPackagePhysical(PackagedMsg); //调用物理层对数据进行封装
	this->AddMsgSendDetails("物理层封装: " + PackagedMsg);
	this->AddMsgSendDetails("发送数据: " + PackagedMsg);
	return PackagedMsg;
}

bool MsgPackage::UnpackMessage(CString SourceMsg) //解封装数据
{
	CString UnpackedMsg;
	this->AddMsgReceiveDetails("收到数据: " + SourceMsg);
	UnpackedMsg = this->MsgUnpackPhysical(SourceMsg); //调用物理层对数据进行解封装
	this->AddMsgReceiveDetails("物理层解封装: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackDatalink(UnpackedMsg); //调用数据链路层对数据进行解封装
	this->AddMsgReceiveDetails("数据链路层解封装: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackNetwork(UnpackedMsg); //调用网络层对数据进行解封装
	this->AddMsgReceiveDetails("网络层解封装: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackTransport(UnpackedMsg); //调用传输层对数据进行解封装
	this->AddMsgReceiveDetails("传输层解封装: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackApplication(UnpackedMsg); //调用应用层对数据进行解封装
	this->AddMsgReceiveDetails("应用层解封装: " + UnpackedMsg);
	this->AddMsgReceiveDetails("原始数据: " + UnpackedMsg);
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

int MsgPackage::CRC32(CString& ComputeStr) //CRC32校验
{
	int StrLen;
	unsigned long CRC32Table[256];
    int i,j;
    unsigned long crc;
    for (i = 0; i < 256; i++) //生成CRC码表
	{
        crc = i;
        for (j = 0; j < 8; j++) 
		{
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320; //标准取值，和Winrar一致
            else
                crc >>= 1;
        }
        CRC32Table[i] = crc;
    }
    ULONG CRCResult(0xffffffff);
    unsigned char* buffer;
    StrLen = ComputeStr.GetLength();
    buffer = (unsigned char*)(LPCTSTR)ComputeStr;
    while(StrLen--)
        CRCResult = (CRCResult >> 8) ^ CRC32Table[(CRCResult & 0xFF) ^ *buffer++];
    return CRCResult^0xffffffff;
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