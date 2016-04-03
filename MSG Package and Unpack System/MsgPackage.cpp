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
	CString PackagedMsg;
	CString CRC32Value;
	CRC32Value.Format("%X",this->CRC32(SourceMsg)); //计算CRC32值，大写16进制
	PackagedMsg = SourceMsg + "|" + CRC32Value;
	return PackagedMsg;
}

CString MsgPackage::MsgPackagePhysical(CString SourceMsg) //物理层封装方法
{
	CString PackagedMsg;
	PackagedMsg = this->CStringToBinary(SourceMsg); //二进制序列化
	return PackagedMsg;
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
	CString UnpackedMsg;
	unsigned int CRC32Vaule;
	UnpackedMsg = SourceMsg.Mid(0,SourceMsg.Find('|')); //包数据段
	CRC32Vaule = strtol(SourceMsg.Mid(SourceMsg.Find('|')+1),NULL,16); //CRC32数据段,十六进制
	if(this->CRC32(UnpackedMsg) != CRC32Vaule) //CRC32校验失败
	{
		this->AddMsgReceiveDetails("CRC32 校验失败");
	}
	return UnpackedMsg;
}

CString MsgPackage::MsgUnpackPhysical(CString SourceMsg) //物理层解封装方法
{
	CString UnpackedMsg;
	UnpackedMsg = this->BinaryToCString(SourceMsg); //二进制反序列化;
	return UnpackedMsg;
}

int MsgPackage::CRC32(CString& ComputeStr) //CRC32校验
{
	int StrLen;
	unsigned long CRC32Table[256];
    int i,j;
    unsigned long CRCNum;
	ULONG CRCResult(0xffffffff);
    unsigned char* StrBuff;
    for (i=0;i<256;i++) //生成CRC码表
	{
        CRCNum = i;
        for (j=0;j<8;j++) 
		{
            if (CRCNum & 1)
                CRCNum = (CRCNum >> 1) ^ 0xEDB88320; //标准取值，和Winrar一致
            else
                CRCNum >>= 1;
        }
        CRC32Table[i] = CRCNum;
    }

    StrLen = ComputeStr.GetLength();
    StrBuff = (unsigned char*)(LPCTSTR)ComputeStr;
    while(StrLen--) //计算CRC值
	{
        CRCResult = (CRCResult >> 8) ^ CRC32Table[(CRCResult & 0xFF) ^ *StrBuff++];
	}
    return CRCResult^0xffffffff;
}

CString MsgPackage::CStringToBinary(CString SourceMsg) //将CString类型的数据转换成二进制表示
{
	CString ResultStr;
	char* SourceMsgCharArray;
	SourceMsgCharArray = (LPSTR)SourceMsg.GetBuffer();
    int i=0;
    for(int i=0;SourceMsgCharArray[i]!=0;i++)
	{

        ResultStr.Format(ResultStr+"%02X",(unsigned char)SourceMsgCharArray[i]); //将字符串转换成十六进制表示
    }
	SourceMsg.ReleaseBuffer();
	ResultStr = this->HexStrToBinStr(ResultStr); //十六进制字符串转成二进制字符串
	return ResultStr;
}

CString MsgPackage::BinaryToCString(CString SourceMsg) //将二进制表示的数据转换成CString
{
	CString Result;
	char* ResultCharArray;
	unsigned int ResultCharInt;
	int i;
	SourceMsg = this->BinStrToHexStr(SourceMsg); //二进制字符串转十六进制字符串
	ResultCharArray = (char*)malloc(SourceMsg.GetLength()/2+1); //分配内存
	i = 0;
	for(int j=0;j<SourceMsg.GetLength();j=j+2)
	{
		ResultCharInt = strtol(SourceMsg.Mid(j,2),NULL,16); //按长度为2进行十六进制分割
        ResultCharArray[i] = (char)ResultCharInt; //转换成字符
		i++;
	}
	ResultCharArray[i] = 0;
	Result.Format("%s",ResultCharArray);
	return Result;
}

CString MsgPackage::HexStrToBinStr(CString HexStr) //十六进制字符串转成二进制字符串
{
	CString BinStr;
	HexStr.MakeUpper(); //将十六进制字符串转换成大写
	for(int i=0;i<HexStr.GetLength();i++)
	{
		switch (HexStr.GetAt(i))
		{
		case '0':
			BinStr += "0000";
			break;
		case '1':
			BinStr += "0001";
			break;
		case '2':
			BinStr += "0010";
			break;
		case '3':
			BinStr += "0011";
			break;
		case '4':
			BinStr += "0100";
			break;
		case '5':
			BinStr += "0101";
			break;
		case '6':
			BinStr += "0110";
			break;
		case '7':
			BinStr += "0111";
			break;
		case '8':
			BinStr += "1000";
			break;
		case '9':
			BinStr += "1001";
			break;
		case 'A':
			BinStr += "1010";
			break;
		case 'B':
			BinStr += "1011";
			break;
		case 'C':
			BinStr += "1100";
			break;
		case 'D':
			BinStr += "1101";
			break;
		case 'E':
			BinStr += "1110";
			break;
		case 'F':
			BinStr += "1111";
			break;
		default:
			break;
		}
	}
	return BinStr;
}

CString MsgPackage::BinStrToHexStr(CString BinStr) //二进制字符串转十六进制字符串
{
	CString HexStr;
	for(int i=0;i<BinStr.GetLength();i=i+4)
	{
		HexStr.Format(HexStr+"%X",strtol(BinStr.Mid(i,4),NULL,2));
	}
	return HexStr;
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