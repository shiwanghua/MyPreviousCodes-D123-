#include<string>
#include<fstream>
#include<cstdlib>
#include<iostream>
#include<memory.h>

using namespace std;
typedef struct UserFileDirectory    //�û��ļ�Ŀ¼
{
	string FileName;
	int StartAddress;                   //�ļ��ڴ��̴洢�ռ����ʼ��ַ
	int ProtectCode;                    //�ļ��ı�����
	int FileLength;                      //�ļ���ǰ�ĳ���
	int MaxFilelength;                  //�ļ�����󳤶�
	UserFileDirectory *nextFile;       //ָ����û�Ŀ¼�µ���һ���ļ�
}UFD, *UFD_ptr;

typedef struct MainFileDirectory         //���ļ�Ŀ¼���洢�û���Ϣ
{
	string UserName;
	string UserPassword;
	int userNo;                                //�û����
	int FileNum;

	UserFileDirectory *userFile;            //�û��ļ�
	MainFileDirectory *nextUser;         //��һ���û� 

}MFD, *MFD_ptr;

typedef struct RuntimeFileDirectory     //����ʱ���ļ����򿪵��ļ���
{
	string FileName;
	int StartAddress;
	int ProtectCode;
	int FileLength;                            //�ļ���ǰ�ĳ���
	int MaxFileLength;
	int OpenWays;                            // �ļ��򿪷�ʽ��1����ֻ����2����ɶ���д
	char content[1024 * 1024];         // �ļ�����

	RuntimeFileDirectory *nextFile;
}RFD, *RFD_ptr;

typedef struct Disk
{
	int FreeSpace;               //ʣ��ռ�
	int NowAddress;            //��ʼλ��
}DiskNode;

void InitialDisk();
bool InitialMFD();
bool InitialUFD(char*);
bool InitialAFD();
void PrintUFD();
void Help();
bool UserLogin();
void RenameFile();
bool CreateMFD();
bool CreateUFD();
bool DeleteUFD();
bool OpenFile();
void getOpenFile();	
bool CloseFile();
bool ReadFile();
bool WriteFile();
bool SaveMFD();
bool SaveUFD(char*);
void SystemInitial();
void FileSystem();
bool QuitSystem();
void DiskFormat();
