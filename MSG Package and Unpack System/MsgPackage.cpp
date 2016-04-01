#include "stdafx.h"
#include "MsgPackage.h"


MsgPackage::MsgPackage(CString *SendDetailEditValue,CString *ReceivedDetailEditValue,CDialogEx* MainWindowDlg) //���캯������ʼ������
{
	this->MsgSendDetail = SendDetailEditValue;
	this->MsgReceivedDetail = ReceivedDetailEditValue;
	this->MainWindow = MainWindowDlg;
}


MsgPackage::~MsgPackage(void)
{
}

CString MsgPackage::PackageMessage(CString SourceMsg) //��װ����
{
	CString PackagedMsg;
	this->AddMsgSendDetails("ԭʼ����: " + SourceMsg);
	PackagedMsg = this->MsgPackageApplication(SourceMsg); //����Ӧ�ò�����ݽ��з�װ
	this->AddMsgSendDetails("Ӧ�ò��װ: " + PackagedMsg);
	PackagedMsg = this->MsgPackageTransport(PackagedMsg); //���ô��������ݽ��з�װ
	this->AddMsgSendDetails("������װ: " + PackagedMsg);
	PackagedMsg = this->MsgPackageNetwork(PackagedMsg); //�������������ݽ��з�װ
	this->AddMsgSendDetails("������װ: " + PackagedMsg);
	PackagedMsg = this->MsgPackageDatalink(PackagedMsg); //����������·������ݽ��з�װ
	this->AddMsgSendDetails("������·���װ: " + PackagedMsg);
	PackagedMsg = this->MsgPackagePhysical(PackagedMsg); //�������������ݽ��з�װ
	this->AddMsgSendDetails("������װ: " + PackagedMsg);
	this->AddMsgSendDetails("��������: " + PackagedMsg);
	return PackagedMsg;
}

bool MsgPackage::UnpackMessage(CString SourceMsg) //���װ����
{
	CString UnpackedMsg;
	this->AddMsgReceiveDetails("�յ�����: " + SourceMsg);
	UnpackedMsg = this->MsgUnpackPhysical(SourceMsg); //�������������ݽ��н��װ
	this->AddMsgReceiveDetails("�������װ: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackDatalink(UnpackedMsg); //����������·������ݽ��н��װ
	this->AddMsgReceiveDetails("������·����װ: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackNetwork(UnpackedMsg); //�������������ݽ��н��װ
	this->AddMsgReceiveDetails("�������װ: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackTransport(UnpackedMsg); //���ô��������ݽ��н��װ
	this->AddMsgReceiveDetails("�������װ: " + UnpackedMsg);
	UnpackedMsg = this->MsgUnpackApplication(UnpackedMsg); //����Ӧ�ò�����ݽ��н��װ
	this->AddMsgReceiveDetails("Ӧ�ò���װ: " + UnpackedMsg);
	this->AddMsgReceiveDetails("ԭʼ����: " + UnpackedMsg);
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
	CString PackagedMsg;
	CString CRC32Value;
	CRC32Value.Format("%X",this->CRC32(SourceMsg)); //����CRC32ֵ����д16��ֹ
	PackagedMsg = SourceMsg + "|" + CRC32Value;
	return PackagedMsg;
}

CString MsgPackage::MsgPackagePhysical(CString SourceMsg) //������װ����
{
	CString PackagedMsg;
	PackagedMsg = this->CStringToBinary(SourceMsg); //���������л�
	return PackagedMsg;
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
	CString UnpackedMsg;
	UnpackedMsg = this->BinaryToCString(SourceMsg); //�����Ʒ����л�;
	return UnpackedMsg;
}

int MsgPackage::CRC32(CString& ComputeStr) //CRC32У��
{
	int StrLen;
	unsigned long CRC32Table[256];
    int i,j;
    unsigned long CRCNum;
	ULONG CRCResult(0xffffffff);
    unsigned char* StrBuff;
    for (i=0;i<256;i++) //����CRC���
	{
        CRCNum = i;
        for (j=0;j<8;j++) 
		{
            if (CRCNum & 1)
                CRCNum = (CRCNum >> 1) ^ 0xEDB88320; //��׼ȡֵ����Winrarһ��
            else
                CRCNum >>= 1;
        }
        CRC32Table[i] = CRCNum;
    }

    StrLen = ComputeStr.GetLength();
    StrBuff = (unsigned char*)(LPCTSTR)ComputeStr;
    while(StrLen--) //����CRCֵ
	{
        CRCResult = (CRCResult >> 8) ^ CRC32Table[(CRCResult & 0xFF) ^ *StrBuff++];
	}
    return CRCResult^0xffffffff;
}

CString MsgPackage::CStringToBinary(CString SourceMsg) //��CString���͵�����ת���ɶ����Ʊ�ʾ
{
	CString ResultStr;
	int Result;
	int ResultSize;
	char* SourceMsgCharArray;
	SourceMsgCharArray = (LPSTR)SourceMsg.GetBuffer();
    int i=0;
    for(int i=0;SourceMsgCharArray[i]!=0;i++)
	{

        ResultStr.Format(ResultStr+"%02X",(unsigned char)SourceMsgCharArray[i]);
    }
	SourceMsg.ReleaseBuffer();
	Result = atoi(ResultStr);
	ResultSize = ResultStr.GetLength()*4;
	ResultStr = "";
	_itoa_s(Result,ResultStr.GetBuffer(),ResultSize,2);
	return ResultStr;
}

CString MsgPackage::BinaryToCString(CString SourceMsg) //�������Ʊ�ʾ������ת����CString
{
	int SourceBinary;
	CString Result;
	int ResultSize;
	char* ResultCharArray;
	unsigned int ResultCharInt;
	int i;
	ResultSize = ResultSize = SourceMsg.GetLength()/4;
	SourceBinary = strtol(SourceMsg,NULL,2);
	SourceMsg = "";
	_itoa_s(SourceBinary,SourceMsg.GetBuffer(),ResultSize,16); //������תʮ������
	ResultCharArray = (char*)malloc(SourceMsg.GetLength()/2+1); //�����ڴ�
	i = 0;
	for(int j=0;j<SourceMsg.GetLength();j=j+2)
	{
		ResultCharInt = strtol(SourceMsg.Mid(j,2),NULL,16); //������Ϊ2����ʮ�����Ʒָ�
        ResultCharArray[i] = (char)ResultCharInt; //ת�����ַ�
		i++;
	}
	ResultCharArray[i] = 0;
	Result.Format("%s",ResultCharArray);
	SourceMsg.ReleaseBuffer();
	return Result;
}

void MsgPackage::AddMsgSendDetails(CString DetailsText) //�����Ϣ����Ϣ�����������ʾ
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgSendDetail = *this->MsgSendDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}
void MsgPackage::AddMsgReceiveDetails(CString DetailsText) //�����Ϣ����Ϣ�����������ʾ
{
	DetailsText = "[" + CTime::GetCurrentTime().Format("%H:%M:%S") + "]  " + DetailsText + "\r\n";
	*this->MsgReceivedDetail = *this->MsgReceivedDetail + DetailsText;
	this->MainWindow->UpdateData(false);
}