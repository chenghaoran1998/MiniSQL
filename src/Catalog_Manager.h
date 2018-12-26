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
	map<string, int> colummap;   //int: vecotr的下表
	vector<attrbute> colum;     //table->colum[1]  //table->colum.size();
	int length;
	int recordindex;
};


class CatalogManager {
public:
	CatalogManager(string database = "db");
	bool tableExists(string table);
	/*检查表是否存在*/
	bool Create_Table(string tablename);
	//建表
	bool Create_Attr(string tablename, string Attr_Name, int Attr_Type, bool unique);
	//添加表
	int tableCount();
	/*返回表的总数*/
	int attrCount(string table);
	/*返回字段的总数*/
	int recordCount(string table);
	/*返回记录总数*/

	bool Drop_Table(string table);
	/*删除表*/
	Table get_table_info(string table);
	/*获取表信息，具体返回类型尚待商榷*/
	attrbute get_attr_info(string table, string attr);
	/*获取列信息，具体返回类型尚待商榷*/
	bool isUnique(string table, string attr);
	/*检查某表某字段是否唯一（字段的属性是否是unique）*/
	bool hasIndex(string table, string attr);
	/*检查某表某字段是否有索引*/
	bool attrExists(string table, string attr);
	//属性是否存在
	bool indexExists(string index);
	//索引存在
	Index* indexName(string table, string attr);
	/*返回某字段上所有的索引，返回类型尚待商榷*/
	bool isPK(string table, string attr);
	/*检查某表是否是主键*/
	string pkOnTable(string table);
	/*寻找表中主键*/
	bool set_zero(string tablename);
	/*某表格置零*/

	Index get_index_info(string index);
	/*获取某索引信息，返回类型尚待商榷*/
	map<string, Index> get_all_index();
	/*获取所有索引信息*/
	int indexCount(string table);
	/*返回索引的总数*/
	string tableIndexOn(string index);
	/*获取索引建立在哪个表*/
	string attrIndexOn(string index);
	/*获取索引建立在哪个字段*/
	bool addIndex(string index, string table, string attr);
	/*添加索引*/
	bool deleteIndex(string index);
	/*删除索引*/
	bool save();
	bool setpk(string table, string attr);
	/*建立主键索引*/
	void Insert_Data(string tablename);
	/*插入数据*/

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

