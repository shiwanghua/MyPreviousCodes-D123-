#include<boost/archive/text_oarchive.hpp> 
#include<boost/archive/text_iarchive.hpp> 
#include<boost/serialization/vector.hpp>
#include<boost/serialization/string.hpp>
#include<iostream>
#include<sstream>
#include<string>
#include"DBs.h"
#include"Lexer.h"
#include"Parser.h"
#include"structSerialize.h"
using namespace std;
int main()
{
	DBs* dbs;
	structSerialize tempSS;  //������תΪ�ṹ�뽫�ṹתΪ����

	sDBs sdbs;  //������ļ��ж�ȡ�����ݿ���Ϣ

	//���ļ��ж�ȡ�Ѵ��������ݿ�
	string dbsPath = "E:\\database.txt";
	const char* tempPath = dbsPath.c_str();

	//��ȡ�ļ���С
	struct _stat info;
	_stat(tempPath, &info);
	int size = info.st_size;

	//��ȡ���л������ݿ⣬�Ƚ������ϵ����ݿ����ṹ��
	if (size > 0)
	{
		ifstream readDbs(dbsPath);
		boost::archive::text_iarchive ia(readDbs);
		if (readDbs.is_open())
		{
			ia >> sdbs;
		}
		readDbs.close();
		dbs = tempSS.transformStructToObject(sdbs);  //�ٽ��ṹת��Ϊ����
	}
	else
		dbs = new DBs();


	
	Lexer* lexer = new Lexer("");
	Parser parser(lexer, dbs);
	cout << "��ʼ�������ݿ⣺\n\n";

	bool isEnd = false;
	while (!isEnd)
	{
		char filepath[2048];
		char temp;
		int i = 0;  // �����ַ��ĸ���
		cout << "�������ļ�·����sql���( ����#��ʾ���� ����!�˳����� )��";   // ��ȡ�ļ�·����sql���
		temp = cin.get();
		if (temp == '!')
		{
			isEnd = true;
			break;
		}
		while (temp != '#')
		{
			filepath[i++] = temp;
			temp = cin.get();
			if (temp == '!')
			{
				isEnd = true;
				break;
			}
		}
		filepath[i] = '\0';
		lexer = new Lexer(filepath);
		parser.setLexer(lexer);
		parser.program();
	}

	//�����˳�ʱ�������л������ݿ�浽����,�Ƚ�����ת��Ϊ�ṹ����д�����
	sdbs = tempSS.transformObjectToStruct(*dbs);
	ofstream writeDbs(dbsPath);
	boost::archive::text_oarchive oa(writeDbs);
	if (writeDbs.is_open())
		oa << sdbs;
	writeDbs.close();
	return 0;
}