#include "stdafx.h"
#include "MsgPackage.h"
#include <aes.h>
#include <modes.h>
#include <filters.h>
#include <hex.h>

#pragma comment (lib,"cryptlib.lib")


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
	CString PackagedMsg;
	PackagedMsg = "HTTP/1.1|" + SourceMsg; //APPͷ��Ϣ
	return PackagedMsg;
}

CString MsgPackage::MsgPackageTransport(CString SourceMsg) //������װ����
{
	CString PackagedMsg;
	PackagedMsg = "Port|";
	PackagedMsg = "TCP|" + PackagedMsg;
	PackagedMsg += SourceMsg; 
	return PackagedMsg;
}

CString MsgPackage::MsgPackageNetwork(CString SourceMsg) //������װ����
{
	CString PackagedMsg;
	PackagedMsg = "IP|" + SourceMsg;
	return PackagedMsg;
}

CString MsgPackage::MsgPackageDatalink(CString SourceMsg) //������·���װ����
{
	CString PackagedMsg;
	CString CRC32Value;
	CRC32Value.Format("%X",this->CRC32(SourceMsg)); //����CRC32ֵ����д16����
	PackagedMsg = CRC32Value + "|";
	PackagedMsg = "MAC|" + PackagedMsg;
	PackagedMsg += SourceMsg;
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
	CString UnpackedMsg;
	UnpackedMsg = SourceMsg;
	if(UnpackedMsg.Mid(0,UnpackedMsg.Find('|')) != "HTTP/1.1")
	{
		this->AddMsgReceiveDetails("Ӧ�ò���װ����");
	}
	UnpackedMsg = UnpackedMsg.Mid(UnpackedMsg.Find('|')+1);
	return UnpackedMsg;
}

CString MsgPackage::MsgUnpackTransport(CString SourceMsg) //�������װ����
{
	CString UnpackedMsg;
	UnpackedMsg = SourceMsg;
	if(UnpackedMsg.Mid(0,UnpackedMsg.Find('|')) != "TCP")
	{
		this->AddMsgReceiveDetails("�������װ����");
	}
	UnpackedMsg = UnpackedMsg.Mid(UnpackedMsg.Find('|')+1);
	if(UnpackedMsg.Mid(0,UnpackedMsg.Find('|')) != "Port")
	{
		this->AddMsgReceiveDetails("�������װ����");
	}
	UnpackedMsg = UnpackedMsg.Mid(UnpackedMsg.Find('|')+1);
	return UnpackedMsg;
}

CString MsgPackage::MsgUnpackNetwork(CString SourceMsg) //�������װ����
{
	CString UnpackedMsg;
	UnpackedMsg = SourceMsg;
	if(UnpackedMsg.Mid(0,UnpackedMsg.Find('|')) != "IP")
	{
		this->AddMsgReceiveDetails("�������װ����");
	}
	UnpackedMsg = UnpackedMsg.Mid(UnpackedMsg.Find('|')+1);
	return UnpackedMsg;
}

CString MsgPackage::MsgUnpackDatalink(CString SourceMsg) //������·����װ����
{
	CString UnpackedMsg;
	unsigned int CRC32Vaule;
	UnpackedMsg = SourceMsg;
	if(UnpackedMsg.Mid(0,UnpackedMsg.Find('|')) != "MAC")
	{
		this->AddMsgReceiveDetails("������·����װ����");
	}
	UnpackedMsg = UnpackedMsg.Mid(UnpackedMsg.Find('|')+1);
	CRC32Vaule = strtoul(UnpackedMsg.Mid(0,UnpackedMsg.Find('|')),NULL,16); //CRC32���ݶ�,ʮ������,ת����int�޷�������
	UnpackedMsg = UnpackedMsg.Mid(UnpackedMsg.Find('|')+1); //�����ݶ�
	if(this->CRC32(UnpackedMsg) != CRC32Vaule) //CRC32У��ʧ��
	{
		this->AddMsgReceiveDetails("CRC32 У��ʧ��");
	}
	else
	{
		this->AddMsgReceiveDetails("CRC32 У��ͨ��");
	}
	return UnpackedMsg;
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

CString MsgPackage::AESEncrypt(CString AESKey,CString SourceStr) //AES����
{
	std::string EncryptedStr;
	std::string EncryptedStrHex;
	byte AESKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];
	byte *EncryptedStrByte;
	for (int i=0;i<CryptoPP::AES::DEFAULT_KEYLENGTH;i++) 
	{ 
		AESKeyByte[i] = AESKey.GetAt(i); 
	}
	CryptoPP::ECB_Mode<CryptoPP::AES>::Encryption ECBEncryption(AESKeyByte,CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::StringSource(SourceStr,true,new CryptoPP::StreamTransformationFilter(ECBEncryption,new CryptoPP::StringSink(EncryptedStr)));
	CryptoPP::HexEncoder HexEncoderObj; //Hex������
	EncryptedStrByte = (byte*)EncryptedStr.c_str();
	HexEncoderObj.Put(EncryptedStrByte,EncryptedStr.length());
	HexEncoderObj.MessageEnd();
	if(HexEncoderObj.MaxRetrievable()>0)
	{
		EncryptedStrHex.resize(HexEncoderObj.MaxRetrievable());		
		HexEncoderObj.Get((byte*)EncryptedStrHex.data(),EncryptedStrHex.size()); //ת����HEX����
	}
	CString EncryptedResult(EncryptedStrHex.c_str());
	return EncryptedResult;
}

CString MsgPackage::AESDecrypt(CString AESKey,CString SourceStrHex) //AES����
{
	std::string SourceStr;
	std::string DecryptedStr;
	CString DecryptedResult;
	byte AESKeyByte[CryptoPP::AES::DEFAULT_KEYLENGTH];

	CryptoPP::HexDecoder HexDecoderObj; //Hex������
	HexDecoderObj.Put((byte*)SourceStrHex.GetBuffer(),SourceStrHex.GetLength());
	HexDecoderObj.MessageEnd();
	if(HexDecoderObj.MaxRetrievable()&&HexDecoderObj.MaxRetrievable() <= SIZE_MAX)
	{
		SourceStr.resize(HexDecoderObj.MaxRetrievable());		
		HexDecoderObj.Get((byte*)SourceStr.data(), SourceStr.size());
	}
	SourceStrHex.ReleaseBuffer();
	for (int i=0;i<CryptoPP::AES::DEFAULT_KEYLENGTH;i++) 
	{ 
		AESKeyByte[i] = AESKey.GetAt(i); 
	}
	CryptoPP::ECB_Mode<CryptoPP::AES>::Decryption ECBDecryption(AESKeyByte,CryptoPP::AES::DEFAULT_KEYLENGTH);
	CryptoPP::StringSource(SourceStr,true,new CryptoPP::StreamTransformationFilter(ECBDecryption,new CryptoPP::StringSink(DecryptedStr)));
	DecryptedResult.Format("%s",DecryptedStr);
	return DecryptedResult;
}

CString MsgPackage::CStringToBinary(CString SourceMsg) //��CString���͵�����ת���ɶ����Ʊ�ʾ
{
	CString ResultStr;
	char* SourceMsgCharArray;
	SourceMsgCharArray = (LPSTR)SourceMsg.GetBuffer();
    int i=0;
    for(int i=0;SourceMsgCharArray[i]!=0;i++)
	{

        ResultStr.Format(ResultStr+"%02X",(unsigned char)SourceMsgCharArray[i]); //���ַ���ת����ʮ�����Ʊ�ʾ
    }
	SourceMsg.ReleaseBuffer();
	ResultStr = this->HexStrToBinStr(ResultStr); //ʮ�������ַ���ת�ɶ������ַ���
	return ResultStr;
}

CString MsgPackage::BinaryToCString(CString SourceMsg) //�������Ʊ�ʾ������ת����CString
{
	CString Result;
	char* ResultCharArray;
	unsigned int ResultCharInt;
	int i;
	SourceMsg = this->BinStrToHexStr(SourceMsg); //�������ַ���תʮ�������ַ���
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
	return Result;
}

CString MsgPackage::HexStrToBinStr(CString HexStr) //ʮ�������ַ���ת�ɶ������ַ���
{
	CString BinStr;
	HexStr.MakeUpper(); //��ʮ�������ַ���ת���ɴ�д
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

CString MsgPackage::BinStrToHexStr(CString BinStr) //�������ַ���תʮ�������ַ���
{
	CString HexStr;
	for(int i=0;i<BinStr.GetLength();i=i+4)
	{
		HexStr.Format(HexStr+"%X",strtol(BinStr.Mid(i,4),NULL,2));
	}
	return HexStr;
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