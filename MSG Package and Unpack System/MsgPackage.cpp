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
	return SourceMsg;
}

CString MsgPackage::MsgPackagePhysical(CString SourceMsg) //������װ����
{
	return SourceMsg;
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
	return SourceMsg;
}

int MsgPackage::CRC32(CString& ComputeStr) //CRC32У��
{
	int StrLen;
	unsigned long CRC32Table[256];
    int i,j;
    unsigned long crc;
    for (i = 0; i < 256; i++) //����CRC���
	{
        crc = i;
        for (j = 0; j < 8; j++) 
		{
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320; //��׼ȡֵ����Winrarһ��
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