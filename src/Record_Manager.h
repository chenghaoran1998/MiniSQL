#ifndef RecordManager_hpp
#define RecordManager_hpp 

#include <iostream>
#include <string>
#include "Buffer_Manager.h"

struct p {
	string value;
	int n;
};
typedef struct p Pair;

union F {
	char byte[4];
	float f;
};

union I {
	char byte[4];
	int a;
};

class RecordManager
{
public:
	void Select_All(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[]);
	void Select(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], int op_N, string Attr[], int op[], string Num[], string Index[]);
	void Insert(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], string value[]);
	void Delete_All(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], string Index[]);
	void Delete(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], int op_N, string Attr[], int op[], string Num[], string Index[]);
	Pair * Get_Attr(int N, string Table_Name, string Attr, int Attr_N, int Attr_Type[], string Attr_Name[]);
	int ExistValue(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], string value[], int WhichOne);
private:
	string To_String(char*p, int offset1, int offset2);
	int cal(int s[], int N);
	int* Print_Head(int Attr_Type[], string Attr_Name[], int Attr_N);
	void Print_Row(int leng[], int N);
	int verify(string temp, string Attr[], int op[], string Num[], int num, int Attr_Type[], string Attr_Name[], int n);
	void Show(string Table_Name, int length, int Attr_N, int Attr_Type[], string Attr_Name[], int op_N, string Attr[], int op[], string Num[], int s[], int n);
};

#endif

