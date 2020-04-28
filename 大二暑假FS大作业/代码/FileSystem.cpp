#include"FileSystem.h"
#define MaxDiskLength 1024*1024

const int MaxUserNum = 5;
const int MaxOpenFile = 8;
int UserNum = 0;
int FileOpened = 0;

char login = false; // �ж��Ƿ��ѵ�¼
char UserName[20];
char Password[20];
char UserPassword[20];
char DiskContent[MaxDiskLength];

// ָ������Ŀ¼���ĸ�ָ��
MFD* pMFD = NULL; 
UFD* pUFD = NULL;
RFD* pRFD = NULL;
MFD* pCurrentUser = NULL;
Disk DiskHead;  //����ͷ

//��ʼ�����̺͸���Ŀ¼
void InitialDisk()
{
	DiskHead.FreeSpace = MaxDiskLength;
	DiskHead.NowAddress = 0;
	memset(DiskContent, '\0', sizeof(char));
}

bool InitialMFD()  // ���ı��ļ������ݣ����뵽���ݽṹ�У��ڴ��У�
{
	pMFD = new MFD; // ΪpMFDָ�����ռ�
	//��һ��MFD�ṹ�ﲻ�����ݣ��ӵڶ�����ʼ��
	pMFD->userFile = NULL;
	pMFD->UserName = "";
	pMFD->UserPassword = "";
	pMFD->userNo = 0;
	pMFD->FileNum = 0;

	ifstream mfd("MFD.txt"); //���ļ�Ŀ¼����������
	if (!mfd)
	{
		cout << "��Ŀ¼MFD�ļ���ʧ�ܣ�\n";
		pMFD->nextUser = NULL;
		return false;
	}

	// ����MFD�ı��ļ�
	MFD* temp = pMFD;  // ����һ��MFDָ�뽫��ȡ�����ݰ��˵����ݽṹ��
	while (!mfd.eof())       // MFD�ı��ļ��л�������δ�����ڴ�
	{
		temp->nextUser = new MFD;   // ��һ��MFD�ṹ�ﲻ�����ݣ��ӵڶ�����ʼ�棬����ʹ��ΪnextUser�����ڴ������
		temp = temp->nextUser;
		mfd >> temp->UserName >> temp->UserPassword >> temp->userNo>>temp->FileNum;  // �����û���Ϣ
		temp->userFile = NULL;         // �û��ļ��ĳ�ʼ������InitialUFD��������
		temp->nextUser = NULL;       // ����û����һ���û��ˣ�������wileѭ����Ϊ���һ���û���MFD���ݽṹ��β
	}

	mfd.close();
	return true;
}

bool InitialUFD(char* FileName)
{
	pUFD = new UFD;
	// ��һ��UFD�ṹҲ��������
	pUFD->FileLength = 0;
	pUFD->FileName = "";
	pUFD->MaxFilelength = 0;
	pUFD->ProtectCode = -1;
	pUFD->StartAddress = 0;

	ifstream ufd(FileName);
	if (!ufd)
	{
		cout << "���û���δ�����ļ���\n\n";
		pUFD->nextFile = NULL;
		return false;
	}

	int readFileNum = pCurrentUser->FileNum;
	UFD* tempUFD = pUFD;
	int fileNum = 0;
	char content[MaxDiskLength];   // �洢�ļ�����
	while (readFileNum--)
	{
		memset(content, '\0', sizeof(content));
		tempUFD->nextFile = new UFD;
		tempUFD = tempUFD->nextFile;
		fileNum++;  

		ufd >> tempUFD->FileName >> tempUFD->StartAddress >> tempUFD->ProtectCode >> tempUFD->FileLength >> tempUFD->MaxFilelength;
		tempUFD->nextFile = NULL;

		ufd.read(content, tempUFD->FileLength+1);
		for (int i = tempUFD->StartAddress, j = 0; j < tempUFD->FileLength+1; i++, j++)  // ���ļ����ݶ�����������
			DiskContent[i] = content[j];
	
		// ���´���ͷ����
		DiskHead.FreeSpace -= tempUFD->MaxFilelength;
		DiskHead.NowAddress += tempUFD->MaxFilelength;
	}

	MFD* tempMFD = pMFD->nextUser;
	string userName;
	while (tempMFD)   // �����ļ�MFD���û��ļ�ָ��ָ����ȷλ��
	{
		userName = tempMFD->UserName + ".txt";
		if (userName == FileName)
		{
			tempMFD->userFile = pUFD->nextFile;
			tempMFD->FileNum = fileNum;
			break;
		}
		tempMFD = tempMFD->nextUser;
	}
	
	ufd.close();
	return true;
}

bool InitialAFD()
{
	pRFD = new RFD;
	pRFD->FileName = "";
	pRFD->StartAddress = -1;
	pRFD->ProtectCode = -1;
	pRFD->FileLength = 0;
	pRFD->MaxFileLength = 0;
	pRFD->OpenWays = -1;
	memset(pRFD->content, '\0', sizeof(pRFD->content));
	pRFD->nextFile = NULL;
	return true;
}

//------------------------------------------------------------------------

void PrintUFD()  //��ӡ�û����ļ�Ŀ¼��Ϣ
{
	UFD*temp = pCurrentUser->userFile;
	if (!temp)
	{
		cout << "\n���û�û�д����ļ�! \n" << endl;
		return;
	}

	cout << "\n����ʣ��ռ䣺" << DiskHead.FreeSpace << endl;
	cout << "\n�û�����" << pCurrentUser->UserName << endl;
	cout << "�û���ţ�" << pCurrentUser->userNo << endl;
	cout << "�û��ļ�������" << pCurrentUser->FileNum << endl << endl;
	cout << "�ļ���\t\t��ʼ�����ַ\t������\t�ļ�����\t����ļ�����" << endl;
	while (temp)
	{
		cout <<"\n"<< temp->FileName<<"\t\t" << static_cast<void*> (&DiskContent[temp->StartAddress] )<< "\t";
		int ptc = temp->ProtectCode;
		if (ptc == 0)
			cout << "---\t";
		else if (ptc == 1)
			cout << "r-x\t";
		else if (ptc == 2)
			cout << "-wx\t";
		else if (ptc == 3)
			cout << "rwx\t";
		cout << temp->FileLength << "\t\t" << temp->MaxFilelength << "\n\n";

		temp = temp->nextFile;
	}

}

void Help()
{
	system("color B1");
	cout << "\n*********************�����ļ�ϵͳ--�����**********************" << endl;
	cout << "*\t\t����			  ˵��			*" << endl;
	cout << "*                                                               * " << endl;
	cout << "*\t\tlogin             \t��¼ϵͳ		*" << endl;
	cout << "*\t\tlogout 			�˳���¼		*" << endl;
	cout << "*\t\tdir	        \t��ʾĿ¼		*" << endl;
	cout << "*\t\tcreate			�����ļ�		*" << endl;
	cout << "*\t\tdelete			ɾ���ļ�		*" << endl;
	cout << "*\t\topen			���ļ�		*" << endl;
	cout << "*\t\tgetOpenFile             �鿴�Ѵ��ļ�          *" << endl;
	cout << "*\t\tclose			�ر��ļ�		*" << endl;
	cout << "*\t\tread			��ȡ�ļ�		*" << endl;
	cout << "*\t\twrite			д���ļ�		*" << endl;
	cout << "*\t\trename		\t�������ļ�		*" << endl;
	cout << "*\t\thelp			�����˵�		*" << endl;
	cout << "*\t\tcls 			�����Ļ		*" << endl;
	cout << "*\t\tquit			�˳�ϵͳ		*" << endl;
	cout << "*****************************************************************\n" << endl;
	return;
}

bool UserLogin()
{
	while (true)                                      // ֱ��������ȷΪֹ
	{
		cout << "�������û�����";
		cin >> UserName;

		int countInput = 0;                        // ��ೢ��������������
		MFD *tempMFD = pMFD->nextUser;
		while (tempMFD)
		{
			if (UserName == tempMFD->UserName)
			{
				while (true)                                  //  �ҵ��û�����Ӧ��MFD�ṹ
				{
					cout << "���������룺";
					cin >> Password;

					if (Password == tempMFD->UserPassword)
					{
						cout << "\n��¼�ɹ�!\n" << endl;
						pCurrentUser = tempMFD;       // ָ��ǰ�û�
						char userFileName[20];
						memset(userFileName, '\0', sizeof(userFileName));
						strcpy(userFileName, UserName);
						strcat(userFileName, ".txt");      // �ļ�ȫ��	
						if (InitialUFD(userFileName))     // ��ȡ�û��ļ���Ϣ
							PrintUFD();                      // ��ӡ�û���Ϣ
						return true;
					}
					else {
						if (countInput == 3)
						{
							cout << "������������������Ժ��¼\n";
							return false;
						}
						else
						{
							cout << "������󣬵�¼ʧ�� !\n";
							countInput++;
						}
					}  // end else
				}      // end inner while
			}          // end if

			tempMFD = tempMFD->nextUser;  // ����ÿһ���û�
		}              // End outer while

		cout << "�����ڸ��û��������������û�����"<<endl;
	}
}

void RenameFile()
{
	string oldFileName;
	string newFileName;
	UFD* tempUFD;
	bool answer = true;
	while (answer)
	{
		cout << "��������Ҫ�޸ĵ��ļ�����";
		cin >> oldFileName;

		tempUFD = pUFD->nextFile;
		while (tempUFD)
		{
			if (tempUFD->FileName == oldFileName)
			{
				RFD* tempRFD = pRFD->nextFile;
				while (tempRFD)
				{
					if (tempRFD->FileName == oldFileName)
					{
						cout << "��ǰ�ļ��Ѵ򿪣���رպ�����������\n";
						return;
					}
					tempRFD = tempRFD->nextFile;
				}

				cout << "�������µ��ļ�����";
				cin >> newFileName;
				tempUFD->FileName = newFileName;
				cout << "�޸ĳɹ�";
				PrintUFD();
				return;
			}
			tempUFD = tempUFD->nextFile;
		}

		cout << "��������ļ��������ڣ��Ƿ��������?<y/n>";
		char input;
		cin >> input;
		if (input == 'n')
			answer = false;
	}
}

bool CreateMFD()
{
	if (MaxUserNum == UserNum)
	{
		cout << "�Ѿ�����" << MaxUserNum << "���û����ﵽ���ޣ�\n";
		return false;
	}
	string userName;
	string password;
	MFD* temp = pMFD->nextUser;
	
	cout << "�������û�����";
label1:
	cin >> userName;
	if (temp)  // ����ǿվͲ���
	{
		while (temp)                                        // �û�������
		{
			if (userName == temp->UserName)
			{
				cout << "���и��û��������������룺";
				temp = pMFD->nextUser;
				goto label1;
			}

			if (!temp->nextUser)  // ���������һ���û�, ˵�����ظ�����ʱ����while���Ա�����ָ���ֵ
				break;
			temp = temp->nextUser;
		}
	}
	else
		temp = pMFD;

	cout << "�������û����룺";
	cin >> password;
	temp->nextUser = new MFD;
	temp = temp->nextUser;
	temp->nextUser = NULL;
	temp->userFile = NULL;
	temp->UserName = userName;
	temp->UserPassword = password;
	temp->userNo = ++UserNum;
	temp->FileNum = 0;
	return true;
}

bool CreateUFD()
{
	string fileName;
	UFD* temp = pUFD->nextFile;
	int startAddress;
	int protectCode;
	int fileLength = 0;
	int maxFileLength;

	cout << "�������´������ļ�����";
label2:
	cin >> fileName;
	if (temp)
	{
		while (temp)
		{
			if (temp->FileName == fileName)
			{
				cout << "�ļ����ظ������������룺";
				temp = pUFD->nextFile;
				goto label2;
			}

			if (!temp->nextFile)    // ��ʱtempָ����û������һ���ļ�UFD
				break;
			temp = temp->nextFile;
		}
		DiskHead.NowAddress = temp->StartAddress + temp->MaxFilelength;  // �ƶ���ͷ�����һ���ļ���ĩβ
	}
	else
		temp = pUFD;

	startAddress = DiskHead.NowAddress;

	cout << "�������ļ���д������(0,1,2,3)��";
	cin >> protectCode;
	while (protectCode < 0 || protectCode>3)
	{
		cout << "������0,1,2��3��";
		cin >> protectCode;
	}

	cout << "�������ļ����������";
label3:
	cin >> maxFileLength;
	if (maxFileLength + DiskHead.NowAddress > MaxDiskLength)
	{
		cout << "���̿ռ䲻�㣡��ʣ" << MaxDiskLength - DiskHead.NowAddress << "���ַ��Ŀռ䣡" << endl;
		cout << "�����������ļ����������";
		goto label3;
	}
	DiskHead.FreeSpace -= maxFileLength;

	// д���ʼ�ļ�����
	char input;
	cout << "�������ļ����ݣ��������Ϊ" << maxFileLength << "������$�ٰ��س����ɽ�������\n";
	input = cin.get();
	while (input!='$')  // �����ļ����ݣ����� $ �������
	{
		DiskContent[startAddress + fileLength++] = input;
		if (fileLength == maxFileLength)
		{
			cout << "�������ݴﵽ�ļ��������������ֹͣ��\n";
			break;
		}
		input = cin.get();
	}

	temp->nextFile = new UFD;
	temp = temp->nextFile;
	// �����ļ����ԣ�ǰ�涼�ǰ�˳��һ��һ����ȡ�ļ�����
	temp->FileName = fileName;
	temp->StartAddress = startAddress;
	temp->ProtectCode = protectCode;
	temp->FileLength = fileLength;
	temp->MaxFilelength = maxFileLength;
	temp->nextFile = NULL;

	pCurrentUser->FileNum++;
	if (pCurrentUser->FileNum == 1)  // �û��ĵ�һ���ļ�
		pCurrentUser->userFile = temp;
	cout << "�½��ļ��ɹ���" << endl;
	PrintUFD();                      // ��ӡ�û���Ϣ
	return true;
}

bool DeleteUFD()
{
	string fileName;
	cout << "������Ҫɾ�����ļ�����";
	cin >> fileName;
	RFD* tempRFD = pRFD->nextFile;
	while (tempRFD)
	{
		if (tempRFD->FileName == fileName)
		{
			cout << "���ļ����ڴ�״̬�����ȹرգ���ɾ����\n" << endl;
			return false;
		}
		tempRFD = tempRFD->nextFile;
	}

	UFD* tempUFD = pUFD;
	while (tempUFD->nextFile)
	{
		if (tempUFD->nextFile->FileName == fileName)
		{
			/*// ɾ���ı��ļ�-- ������ɾ���û�
			string tempFileName = tempDelete->FileName + ".txt";
		    char* tempFN = new char(1+tempFileName.length());
			strcpy(tempFN, tempFileName.c_str());
			remove(tempFN);*/

			UFD* tempDelete = tempUFD->nextFile;
			tempUFD->nextFile = tempDelete->nextFile;
			pCurrentUser->userFile = pUFD->nextFile;  // ���ɾ�����ǵ�һ��λ�õ��ļ�������Ҫ�ı�pCurrentUserָ���userFileָ��
			pCurrentUser->FileNum--;

			// �������
			DiskHead.FreeSpace += tempDelete->MaxFilelength;
			for (int i = tempDelete->StartAddress; i < tempDelete->StartAddress + tempDelete->MaxFilelength; i++)
				DiskContent[i] = '\0';

			delete tempDelete;
			cout << "�ļ��ɹ�ɾ����" << endl;
			PrintUFD();
			return true;
		}
		tempUFD = tempUFD->nextFile;
	}

	cout << "�����ڸ��ļ���" << endl;
	return false;
}

bool OpenFile()
{
	if (FileOpened == MaxOpenFile)
	{
		cout << "�Ѿ���8���ļ����ﵽ�˴��ļ������ޡ������ȹر������ļ��ٳ��Դ��ļ���\n";
		return false;
	}
	string fileName;
	cout << "\n������Ҫ�򿪵��ļ�����";
	cin >> fileName;

	UFD* tempUFD = pUFD->nextFile;
	RFD* tempRFD = pRFD->nextFile;
	while (tempRFD)
	{
		if (tempRFD->FileName == fileName)
		{
			cout << "���ļ��Ѿ��򿪣�\n";
			return true;
		}
		if (!tempRFD->nextFile)  // ����tempRFDָ�����һ���򿪵��ļ�
			break;
		else
			tempRFD = tempRFD->nextFile;
	}
	if (!tempRFD)   // ���������û�д򿪵��ļ���
		tempRFD = pRFD;

	while (tempUFD)
	{
		if (tempUFD->FileName == fileName)
		{
			if (tempUFD->ProtectCode == 0)
			{
				cout << "������Ϊ0�����ܴ򿪡�\n\n";
				return false;
			}
			else if (tempUFD->ProtectCode == 1)
			{
				tempRFD->nextFile = new RFD;
				tempRFD = tempRFD->nextFile;
				tempRFD->FileName = fileName;
				tempRFD->StartAddress = tempUFD->StartAddress;
				tempRFD->ProtectCode = tempUFD->ProtectCode;
				tempRFD->FileLength = tempUFD->FileLength;
				tempRFD->MaxFileLength = tempUFD->MaxFilelength;
				tempRFD->OpenWays = 1;
				tempRFD->nextFile = NULL;
				for (int i = 0, j = tempRFD->StartAddress; i < tempRFD->FileLength; i++, j++)
					tempRFD->content[i] = DiskContent[j];
				FileOpened++;
				cout << "���ļ��򿪳ɹ���������Ϊ1��ֻ�ܶ�������д��\n";
				return true;
			}
			else if (tempUFD->ProtectCode == 3)
			{
				cout << "��ֻ����ʽ���ļ�������1���Կɶ���д��ʽ���ļ�����2��";
				int input;
				cin >> input;
				while (input != 1 && input != 2)
				{
					cout << "������������������1��2��";
					cin >> input;
				}
				tempRFD->nextFile = new RFD;
				tempRFD = tempRFD->nextFile;
				tempRFD->FileName = fileName;
				tempRFD->StartAddress = tempUFD->StartAddress;
				tempRFD->ProtectCode = tempUFD->ProtectCode;
				tempRFD->FileLength = tempUFD->FileLength;
				tempRFD->MaxFileLength = tempUFD->MaxFilelength;
				tempRFD->OpenWays = input;
				tempRFD->nextFile = NULL;
				for (int i = 0, j = tempRFD->StartAddress; i < tempRFD->FileLength; i++, j++)
				{
					tempRFD->content[i] = DiskContent[j];
				}
				FileOpened++;
				cout << "���ļ��򿪳ɹ���������Ϊ"<< tempUFD->ProtectCode<<"���򿪷�ʽΪ";
				if (input == 1)
					cout << "ֻ����\n\n";
				else
					cout << "�ɶ���д��\n\n";
				return true;
			}
		}
		tempUFD = tempUFD->nextFile;
	}

	cout << "�����ڸ��ļ�����ʧ��! \n";
	return false;
}

void getOpenFile() // �����Ѵ򿪵��ļ����ļ���
{
	RFD* tempRFD = pRFD->nextFile;

	if (tempRFD)
	{
		cout << "\n�Ѵ򿪵��ļ��У�\n       \t\t";
		while (tempRFD)
		{
			cout << tempRFD->FileName << "\n       \t\t";
			tempRFD = tempRFD->nextFile;
		}
	}
	else cout << "\nû�д򿪵��ļ���\n\n";
}

bool CloseFile()
{
	string fileName;
	UFD* tempUFD = pUFD->nextFile;
	RFD* tempRFD = pRFD;

	cout << "������Ҫ�رյ��ļ�����";
	cin >> fileName;
	
	while (tempRFD->nextFile)
	{
		if (tempRFD->nextFile->FileName == fileName)
		{
			RFD*tempRFDDelete = tempRFD->nextFile;
			tempRFD->nextFile = tempRFDDelete->nextFile;

			if (tempRFDDelete->OpenWays == 2)  // �Կ�д��ʽ�򿪵ģ��ر��ļ�ʱӦ�ñ���������ԡ�
			{
				while (tempUFD)
				{
					if (tempUFD->FileName == fileName)
					{
						tempUFD->FileLength = tempRFDDelete->FileLength;
						for (int i = 0, j = tempRFDDelete->StartAddress; i < tempRFDDelete->FileLength; i++, j++)
							DiskContent[j] = tempRFDDelete->content[i];
						DiskContent[tempRFDDelete->StartAddress + tempRFDDelete->FileLength] = '\0';
						delete tempRFDDelete;
						cout << "�ļ��رճɹ���" << endl;
						FileOpened--;
						PrintUFD();
						getOpenFile();
						return true;
					}
					tempUFD = tempUFD->nextFile;
				}
				cout << "�ر��ļ�ʱ����ʧ�ܣ�����";
				return false;
			}
			FileOpened--;
			cout << "�ɹ��ر��ļ���\n";
			getOpenFile();
			return true;
		}
		tempRFD = tempRFD->nextFile;
	}
	cout << "���ļ���û�д򿪣�����رգ�" << endl;
	return false;
}

bool ReadFile()
{
	getOpenFile();
	string fileName;
	RFD* tempRFD = pRFD->nextFile;

	if (tempRFD)
	{
		cout << "\n������Ҫ��ȡ���ļ�����";
		cin >> fileName;

		while (tempRFD)
		{
			if (tempRFD->FileName == fileName && tempRFD->ProtectCode != 2)
			{                                                                                 // ������Ϊ2ʱֻ��д�벻�ܶ�ȡ��
				cout << "�ļ��������£�" << endl;
				for (int i = 0; i < tempRFD->FileLength; i++)
					cout << tempRFD->content[i];
				cout << "\n �ļ���ȡ��ϣ�\n\n";
				return true;
			}
			tempRFD = tempRFD->nextFile;
		}

		cout << "��ȡʧ�ܣ��ļ������ڻ���δ�򿪻��߱����벻��1��3��\n";
		return false;
	}
	else return false;
}

bool WriteFile()
{
	string fileName;
	cout << "������Ҫд����ļ�����";
	cin >> fileName;

	RFD* tempRFD = pRFD->nextFile;
	while (tempRFD)
	{
		if (tempRFD->FileName == fileName)
		{
			if (tempRFD->ProtectCode < 2)
			{
				cout << "���ļ�����д��������С��2��\n" << endl;
				return false;
			}
			else if (tempRFD->OpenWays == 1)
			{
				cout << "�ļ�����ֻ����ʽ�򿪵ģ������޸ģ�\n\n";
				return false;
			}
			else
			{
				string newContent;
				cout << "���ļ����ݣ�\n    ";
				for (int i = tempRFD->StartAddress; i < tempRFD->StartAddress + tempRFD->FileLength; i++)
					cout << DiskContent[i];
				cout << "\n�ļ���󳤶�Ϊ" << tempRFD->MaxFileLength << "���ַ���";

				cout << "\n�������µ��ļ������Ը���ԭ�ļ�����(����$�ٰ��س����ɽ�������\n    ";
				char input;
				int length = 0;
				input = cin.get();
				while (input != '$')  // �����ļ����ݣ����� $ �������
				{
					tempRFD->content[length++] = input;
					if (length == tempRFD->MaxFileLength)
					{
						cout << "�������ݴﵽ�ļ��������������ֹͣ��\n\n";
						break;
					}
					input = cin.get();
				}		

				tempRFD->FileLength = length;						
				cout << "д���ļ��ɹ�(δ���棩��\n\n";
				return true;
			}
		}
		tempRFD = tempRFD->nextFile;
	}  // end while
	cout << "�����ڴ��ļ����ߴ��ļ�δ�򿪣����ȳ���open���ļ���\n\n" << endl;
	return false;
}

bool SaveMFD()
{
	ofstream ofsMFD("MFD.txt");
	if (!ofsMFD.is_open())
	{
		cout << "MFD.txt�ļ���ʧ�ܣ�\n";
		ofsMFD.close();
		return false;
	}

	MFD* tempMFD = pMFD->nextUser;
	while (tempMFD)
	{
		ofsMFD << tempMFD->UserName <<" "<< tempMFD->UserPassword << " " << tempMFD->userNo << " " <<tempMFD->FileNum;  // �����û���Ϣ
		if (tempMFD->nextUser)
			ofsMFD << "\n";
		tempMFD = tempMFD->nextUser;
	}
	ofsMFD.close();
	cout << "�ɹ�������Ŀ¼�ļ�...\n" << endl;
	return true;
}

bool SaveUFD(char* name)
{
	ofstream ofsUFD(name);        //�ļ������������
	if (!ofsUFD.is_open())
	{
		cout << name<<"�ļ��򲻿�������ʧ�ܣ�\n";
		ofsUFD.close();
		return false;
	}
	if (pUFD->nextFile)
	{
		UFD* tempUFD = pUFD->nextFile;
		while (tempUFD)
		{
			char* tempContent = new char(tempUFD->FileLength + 1); // ���һ���ַ��ǿ��ַ��������������
			for (int i = 0, j = tempUFD->StartAddress; i < tempUFD->FileLength; i++, j++)
				tempContent[i] = DiskContent[j];

			ofsUFD << tempUFD->FileName << " " << tempUFD->StartAddress << " " << tempUFD->ProtectCode << " " << tempUFD->FileLength << " " << tempUFD->MaxFilelength << " ";
			ofsUFD.write(tempContent, tempUFD->FileLength);
			ofsUFD << " ";
			cout << "�ļ�" << tempUFD->FileName << "����ɹ���\n";
			tempUFD = tempUFD->nextFile;
		}
	}
	else
	{
		ofsUFD.close();
		remove(name);
		cout << "���û��ļ���Ҫ���棡\n";
		return true;
	}
	

	ofsUFD << '\0';
	ofsUFD.close();
	cout << "�û��ļ�������ϣ�" << endl;
	return true;
}

void SystemInitial()
{
	Help();
	InitialDisk();
	InitialAFD();
	if (!InitialMFD())  // ��һ������ϵͳʱMFD�ļ�δ���������󴴽���ʼ�û���
	{
		cout << "��һ������ϵͳ�������ʼ������......\n��ʼ�����û�:\n" << endl;
		bool goOn = true;
		char input;
		while (goOn)
		{
			CreateMFD();
			cout << "\n�Ƿ��贴���µ��û�������y��ʾ�ǣ�n��ʾ�񣩣�";
			cin >> input;
			if (input == 'n')
				goOn = false;
			cout << endl;
		}
		cout << "ϵͳ��ʼ����ϣ�";
		SaveMFD();
	}
}

void FileSystem()
{
	string command;
	while (true)
	{
		cout << "�ȴ��������";
		cin >> command;
		if (login == false && command != "help"&&command != "cls"&&command != "login"&&command != "quit")
			cout << "���ȵ�¼�û��ٽ��в�����\n\n";
		else if (login == false && command == "login")
		{
			if (UserLogin())  // ��¼�ɹ�
			{
				login = true;
				InitialAFD();
			}
		}
		else if (login == true && command == "login")
			cout << "�û�" << UserName << "�ѵ�¼!һ��ֻ����һ���û���¼�� \n\n";
		else if (command == "create")
			CreateUFD();
		else if (command == "delete")
			DeleteUFD();
		else if (command == "open")
			OpenFile();
		else if (command == "getOpenFile")
			getOpenFile();
		else if (command == "close")
			CloseFile();
		else if (command == "read")
			ReadFile();
		else if (command == "write")
			WriteFile();
		else if (command == "dir")
			PrintUFD();
		else if (command == "rename")
			RenameFile();
		else if (command == "help")
			Help();
		else if (command == "cls")
			system("cls");
		else if (command == "logout")  // �˳���¼�����Լ��������û���¼
		{
			if (QuitSystem())
			{
				login = false;
				SystemInitial();
			}
			else cout << "�˳���¼ʧ�ܣ�\n";
		}
		else if (command == "quit")
			if (QuitSystem())
			{
				login = false;
				break;
			}
			else
				cout << "�˳�ϵͳʧ�ܣ�" << endl;
		else
		{
			cout << "���������������������롣\n";
			Help();
		}
	}
}

bool QuitSystem()
{
	RFD* tempRFD = pRFD->nextFile;
	if (tempRFD)
	{
		cout << "���ļ�δ�رգ��Ƿ�ȫ�����棿����1��ʾȫ�����棬����2��ʾȫ�������棬����3��ʾ������������˳�ϵͳ��:";
		char input;
label4:
		cin >> input;
		if (input == '1')  // ����RFD���ݵ�UFD
		{
			while (tempRFD)
			{
				if (tempRFD->OpenWays == 2)  // �Կ�д��ʽ���ļ�����Ҫ����
				{
					UFD* tempUFD = pUFD->nextFile;
					while (tempUFD)
					{
						if (tempUFD->FileName == tempRFD->FileName)
						{
							tempUFD->FileLength = tempRFD->FileLength;
							for (int i = 0, j = tempRFD->StartAddress; i < tempRFD->FileLength; i++, j++)
								DiskContent[j] = tempRFD->content[i];						
							cout << tempUFD->FileName << "�ļ�����ɹ���" << endl;
							break;  // �����ڲ�whileѭ��
						}
						tempUFD = tempUFD->nextFile;
					}
				}
				tempRFD = tempRFD->nextFile;
			}
		}
		else if (input == '2')		{		}
		else if (input == '3')  return false;
		else
		{
			cout << "���������������������룬����1��ʾȫ�����棬����2��ʾȫ�������棬����3��ʾ������������˳�ϵͳ����";
			goto label4;
		}
	}
	// �������ݵ������ı��ļ�
	SaveMFD();
	if (login)
	{
		char TempUserName[20];
		strcpy(TempUserName, UserName);
		strcat(TempUserName, ".txt");
		SaveUFD(TempUserName);
	}
	Help();
	cout << "�ѳɹ��˳�ϵͳ��\n";
	return true;
}

void DiskFormat()  // ���̸�ʽ������������û���Ϣ���û����ļ���Ϣ
{
	MFD* tempMFD = pMFD->nextUser;
	UFD* tempUFD = pUFD->nextFile;
	RFD* tempRFD = pRFD->nextFile;

	while (tempMFD)
	{
		pMFD->nextUser = tempMFD->nextUser;
		delete tempMFD;
		tempMFD = pMFD->nextUser;
	}

	while (tempUFD)
	{
		pUFD->nextFile = tempUFD->nextFile;
		delete tempUFD;
		tempUFD = pUFD->nextFile;
	}

	while (tempRFD)
	{
		pRFD->nextFile = tempRFD->nextFile;
		delete tempRFD;
		tempRFD->nextFile = pRFD->nextFile;
	}
	memset(DiskContent, '\0', sizeof(DiskContent));
	cout << "�����Ѹ�ʽ����";
}

int main()
{
	SystemInitial();
	FileSystem();
	return 0;
}
