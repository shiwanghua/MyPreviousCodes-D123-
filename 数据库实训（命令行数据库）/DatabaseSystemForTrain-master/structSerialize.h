#pragma once
#include<iostream>
#include<string>
#include<vector> 
#include<boost/serialization/vector.hpp>
#include<boost/serialization/string.hpp>
#include"DBs.h"
#include"Database.h"
#include"Table.h"
#include"Column.h"
#include"Row.h"

using namespace std;

struct sRow
{
	vector<string> data;
	//���л�����
	friend class boost::serialization::access;
	template <typename Archive> void serialize(Archive &ar, const unsigned int version)
	{
		ar & data;
	}

	//template <typename Archive> friend void serialize(Archive &ar, sRow & p, const unsigned int version);
};

//enum type { varchar = 0 };

struct sColumn
{
	bool primaryKey;  //�����ʶ
	string columnName;  //����
	type columnType; //�е�����
	bool allowNull;  //�Ƿ�����Ϊ��
	int length;  //��ֵ�ĳ���
	//���л�����
	friend class boost::serialization::access;
	template <typename Archive> void serialize(Archive &ar, const unsigned int version)
	{
		ar & primaryKey;
		ar & columnName;
		ar & columnType;
		ar & allowNull;
		ar & length;
	}
};

struct sTable
{
	string tableName;  //����
	int columnCount = 0;  //�еĸ���
	int rowCount = 0;  //�ñ�ǰ�еĸ���
	vector<sColumn> column;  //������
	vector<sRow> row; //������

	//���л�����
	friend class boost::serialization::access;
	template <typename Archive> void serialize(Archive &ar, const unsigned int version)
	{
		ar & tableName;
		ar & columnCount;
		ar & rowCount;
		ar & column;
		ar & row;
	}
};

struct sDatabase
{
	string name;
	int tableCount = 0;
	vector<sTable> tables;

	//���л�����
	friend class boost::serialization::access;
	template <typename Archive> void serialize(Archive &ar, const unsigned int version)
	{
		ar & name;
		ar & tableCount;
		ar & tables;
	}
};

struct sDBs
{
	vector<sDatabase> databasees;  //�������ݿ���������

	//���л�����
	friend class boost::serialization::access;
	template<typename Archive> void serialize(Archive &ar, const unsigned int version)
	{
		ar & databasees;
	}
};

class structSerialize
{
public:
	DBs* dbs;
	//���ݿ�����Ӧ�Ľṹ
	sDBs sdbs;
	vector<sDatabase> sdatabasees;
	vector<sTable> stables;
	vector<sColumn> scolumn;
	vector<sRow> srow;
	structSerialize();

	//�� DBs �����е� Table ����� Row ��������� Column ��������ת��Ϊ sRow �� sColumn 
	sDBs transformObjectToStruct(DBs dbs);
	DBs* transformStructToObject(sDBs sdbs);
};

template<typename Archive>
inline void serialize(Archive & ar, sRow & p, const unsigned int version)
{
	ar & p.data;
}
