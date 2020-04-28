#pragma once
#include<boost/serialization/vector.hpp>
#include<boost/serialization/string.hpp> 
#include<string>
#include<vector>  //ʵ�� Table �� Row ����Ķ�̬����
#include"Column.h"
#include"Row.h"
#include"Where.h"
using namespace std;
class Table
{
private:
	string tableName;  //����
	int columnCount = 0;  //�еĸ���
	int rowCount = 0;  //�ñ�ǰ�еĸ���
	vector<Column> column;  //������
	vector<Row> row; //������

	
public:
	Table();  //Ĭ�Ϲ��캯��
	Table(string name);
	Table(string name, int colCount);
	Table(string name,int colCount, vector<Column>* col);
	Table(string name, int columnCount, int rowCount, vector<Column> column, vector<Row> row);

	vector<Column> getColumnArray();
	vector<Row> getRowArray();

	bool setTableName(string);
	string getTableName();

	void setColumn(string columnName);  //ͨ�������������

	bool setColumnCount(int c);  //����֮ǰ���rowCount�Ƿ�Ϊ��
	int getColumnCount();

	int getRowCount();

	bool addRow(Row* newRow);
	bool deleteRow(int rowIndex);  //ͨ�����±�
	bool deleteRow(Where whe);  //ͨ�� where ��� 
	bool deleteAllRow();  //��ձ�

	bool addColumn(Column* newColumn);
	bool deleteColumn(int columnIndex);  //ͨ�����±�
	bool deleteColumn(string columnName);  //ͨ������

	void showTable();

	void setRowArray(vector<Row> tempRows);

};


