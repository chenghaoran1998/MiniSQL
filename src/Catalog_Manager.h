#ifndef Catalog_Manager_h
#define Catalog_Manager_h

#include <sstream>
#include <vector>
#include <map>
#include "Buffer_Manager.h"

#define CATA_PATH "Catalog/"
#define CATA_SIZE 512
using namespace std;
struct Index {
	string indexname;
	string tableName;
	string columnName;
};
struct attrbute {
	string name;
	int type;
	bool unique;
	Index* index;
};
struct Table {
	string database;
	int num_of_attr;
	int num_of_index;
	int primarykey;
	map<string, int> colummap;   //int: vecotr���±�
	vector<attrbute> colum;     //table->colum[1]  //table->colum.size();
	int length;
	int recordindex;
};


class CatalogManager {
public:
	CatalogManager(string database = "db");
	bool tableExists(string table);
	/*�����Ƿ����*/
	bool Create_Table(string tablename);
	//����
	bool Create_Attr(string tablename, string Attr_Name, int Attr_Type, bool unique);
	//��ӱ�
	int tableCount();
	/*���ر������*/
	int attrCount(string table);
	/*�����ֶε�����*/
	int recordCount(string table);
	/*���ؼ�¼����*/

	bool Drop_Table(string table);
	/*ɾ����*/
	Table get_table_info(string table);
	/*��ȡ����Ϣ�����巵�������д���ȶ*/
	attrbute get_attr_info(string table, string attr);
	/*��ȡ����Ϣ�����巵�������д���ȶ*/
	bool isUnique(string table, string attr);
	/*���ĳ��ĳ�ֶ��Ƿ�Ψһ���ֶε������Ƿ���unique��*/
	bool hasIndex(string table, string attr);
	/*���ĳ��ĳ�ֶ��Ƿ�������*/
	bool attrExists(string table, string attr);
	//�����Ƿ����
	bool indexExists(string index);
	//��������
	Index* indexName(string table, string attr);
	/*����ĳ�ֶ������е����������������д���ȶ*/
	bool isPK(string table, string attr);
	/*���ĳ���Ƿ�������*/
	string pkOnTable(string table);
	/*Ѱ�ұ�������*/
	bool set_zero(string tablename);
	/*ĳ�������*/

	Index get_index_info(string index);
	/*��ȡĳ������Ϣ�����������д���ȶ*/
	map<string, Index> get_all_index();
	/*��ȡ����������Ϣ*/
	int indexCount(string table);
	/*��������������*/
	string tableIndexOn(string index);
	/*��ȡ�����������ĸ���*/
	string attrIndexOn(string index);
	/*��ȡ�����������ĸ��ֶ�*/
	bool addIndex(string index, string table, string attr);
	/*�������*/
	bool deleteIndex(string index);
	/*ɾ������*/
	bool save();
	bool setpk(string table, string attr);
	/*������������*/
	void Insert_Data(string tablename);
	/*��������*/

private:
	string currentdatabase;
	int num_of_table;
	map<string, Table> tableMap;
	map<string, Index> indexMap;
	bool initialtable(Table &t);
	bool savetable(string tablename);
};
extern CatalogManager CataM;
#endif /* CatalogManager_hpp */

