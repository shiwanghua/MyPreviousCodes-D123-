#pragma once
#include<string>
#include<boost/serialization/vector.hpp>
#include<boost/serialization/string.hpp> 
using namespace std;
class Row
{

private:
	string* data;  //�е�����

public:
	Row();
	Row(string* rowData);
	string getCell(int index);
	bool setCell(int index, string cellContent);
	string* getData();
};
