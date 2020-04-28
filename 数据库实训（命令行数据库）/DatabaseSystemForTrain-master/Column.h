#pragma once
#include<string>
#include<boost/serialization/vector.hpp>
#include<boost/serialization/string.hpp> 
using namespace std;

enum type { varchar = 0 };

class Column
{
private:
	bool primaryKey;  //�����ʶ
	string columnName;  //����
	type columnType; //�е�����
	bool allowNull;  //�Ƿ�����Ϊ��
	int length;  //��ֵ�ĳ���

	
public:
	Column();
	Column(string colName, type colType, int colLength, bool colPrimaryKey, bool colAllowNull);

	bool setColumnName(string colName);
	string getColumnName();

	bool getPrimaryKey();
	bool setPrimaryKey(bool colPrimaryKey);

	bool getAllowNull();
	bool setAllowNull(bool colAllowNull);

	type getColumnType();
	bool setColumnType(type colType);

	int getLength();
	bool setLength(int colLength);
};
