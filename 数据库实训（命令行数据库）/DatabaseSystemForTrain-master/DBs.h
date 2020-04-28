#pragma once 
#include<boost/serialization/vector.hpp> 
#include<string>
#include<iostream>
#include<vector>
#include<map>
#include"Table.h"
#include"Database.h"
#include"Where.h"
#include"Row.h"

using namespace std;

class DBs
{
private:
	vector<Database> databasees;  //�������ݿ���������
public:
	DBs();
	DBs(vector<Database> databasees);
	bool insert(string databaseName, string tableName, map<string, string> insertData);
	bool insert(string databaseName, string tableName, string* insertData);  //insert ��������ÿһ���ж�Ӧ��ֵ
	bool Delete(string databaseName, string tableName, Where deleteCondition);  //Where �ṹ���������ȷ��Row�������±�
	bool Delete(string databaseName, string tableName);  //��where���
	void select(string databaseName, string tableName, vector<string> columnName, Where selectCondition);
	void select(string databaseName, string tableName, vector<string> columnName);  //�� where ���
	bool update(string databaseName, string tableName, map<string, string> updateData, Where updateCondition);
	bool update(string databaseName, string tableName, map<string, string> updateData);  //�� where ���
	bool createTable(string databaseName, Table* newTable); //���� Database.addTable(Table*)
	bool dropTable(string databaseName, string tableName);  //���� Database.deleteTable(String tableName)
	bool createDatabase(Database* database);
	bool dropDatabase(string databaseName);
	int getTableColumnCount(string databaseName, string tableName);
	vector<Database> getDatabasees();
	void showDBs();
};
