#include "Catalog_Manager.h"

CatalogManager CataM("db");
CatalogManager::CatalogManager(string database) {
	currentdatabase = database;
	string path = string(CATA_PATH) + "initial.txt";
	fstream f0(path, ios::in);
	if (f0.good()) {
		string table, index, attr, base, primary;
		int recordindex, length, number;
		bool unique;
		int type;
		while (!f0.eof()) {
			f0 >> table;
			string path = CATA_PATH + table + ".txt";
			fstream f(path, ios::in);
			if (f.good()) {
				f >> table >> base >> length >> recordindex >> primary;

				//                cout<<base<<" "<<length<<" "<<recordindex<<" "<<primary;

				if (base == database) {
					Create_Table(table);
					tableMap[table].recordindex = recordindex;
					string s;
					getline(f, s);
					istringstream info(s);
					info >> number;

					//                    cout<<" "<<number;

					for (int i = 0; i<number; i++) {
						info >> attr >> type >> unique;
						//                        cout<<" "<<attr<<" "<<type<<" "<<unique;
						Create_Attr(table, attr, type, unique);
					}
					getline(f, s);
					istringstream info2(s);
					info2 >> number;

					//                    cout<<" "<<number;

					for (int i = 0; i<number; i++) {
						info2 >> index >> attr;
						//                        cout<<" "<<index<<" "<<attr;
						addIndex(index, table, attr);
					}

					if (primary != "false") {
						setpk(table, primary);
					}
				}
			}
			f.close();
			Table &t = tableMap[table];
			for (int it = 0; it<t.colum.size(); it++) {
				if (t.colum[it].index != nullptr) {
					//                    cout<<" "<<it->second.index->indexname<<" "<<it->second.index->columnName;
				}
			}
		}
	}
	f0.close();
}
bool CatalogManager::save() {
	for (map<string, Table>::iterator it = tableMap.begin(); it != tableMap.end(); it++) {
		savetable(it->first);
	}
	string path = string(CATA_PATH) + "initial.txt";
	fstream f0(path, ios::out);
	if (f0.good()) {
		for (map<string, Table>::iterator it = tableMap.begin(); it != tableMap.end(); it++) {
			f0 << " " << it->first;
		}
	}
	f0.close();
	return true;
}
bool CatalogManager::savetable(string tablename) {
	string path = CATA_PATH + tablename + ".txt";
	Table &t = tableMap[tablename];
	fstream f(path, ios::out);
	f << tablename << " " << t.database << " " << t.length << " " << t.recordindex;
	if (t.primarykey != -1)
		f << " " << t.colum[t.primarykey].name;
	else
		f << " false";
	f << " " << t.num_of_attr;
	for (int it = 0; it<t.colum.size(); it++) {
		f << " " << t.colum[it].name << " " << t.colum[it].type << " " << t.colum[it].unique;
	}
	f << endl;
	f << t.num_of_index;
	for (int it = 0; it<t.colum.size(); it++) {
		if (t.colum[it].index != nullptr) {
			f << " " << t.colum[it].index->indexname << " " << t.colum[it].index->columnName;
		}
	}
	f << endl;
	f.close();
	return true;
}
bool CatalogManager::tableExists(string table) {//表存在
	if (tableMap.find(table) != tableMap.end()) {
		return true;
	}
	return false;
}
bool CatalogManager::attrExists(string table, string attr) {//attr存在
	if (tableMap[table].colummap.find(attr) != tableMap[table].colummap.end()) {
		return true;
	}
	return false;
}
bool CatalogManager::indexExists(string index) {//index存在
	if (indexMap.find(index) != indexMap.end()) {
		return true;
	}
	return false;
}
bool CatalogManager::initialtable(Table &t) {
	t.database = currentdatabase;
	t.num_of_attr = 0;
	t.num_of_index = 0;
	t.primarykey = -1;
	t.recordindex = 0;
	t.length = 0;
	return true;
}
bool CatalogManager::Create_Table(string tablename) {//建表
	if (currentdatabase != "nullptr") {
		if (tableExists(tablename)) {
			cout << "the table file have existed" << endl;
			return false;
		}
		Table t;
		initialtable(t);
		tableMap.insert(pair<string, Table>(tablename, t));
		num_of_table++;
		save();
		return true;
	}
	cout << "you have't use any database" << endl;
	return false;
}
bool CatalogManager::Create_Attr(string tablename, string Attr_Name, int Attr_Type, bool unique) {//增加属性
	if (currentdatabase != "nullptr") {
		if (!tableExists(tablename)) {
			cout << "the table file do not existed" << endl;
			return false;
		}
		Table &t = tableMap[tablename];
		attrbute attr;
		attr.index = nullptr;
		attr.name = Attr_Name;
		attr.type = Attr_Type;
		attr.unique = unique;
		t.colum.push_back(attr);
		t.colummap.insert(pair<string, int>(Attr_Name, t.colum.size() - 1));
		t.num_of_attr++;
		if (attr.type <= -1 || attr.type >= -255)
			t.length -= attr.type;
		else
			t.length += 4;
		savetable(tablename);
		return true;
	}
	cout << "you have't use any database" << endl;
	return false;
}
int CatalogManager::tableCount() {
	return num_of_table;
}
/*返回表的总数*/
int CatalogManager::attrCount(string table) {
	return tableMap[table].num_of_attr;
}
int CatalogManager::recordCount(string table)
{
	return tableMap[table].recordindex;
}
/*返回字段的总数*/
bool CatalogManager::Drop_Table(string table) {
	map<string, Table>::iterator it = tableMap.find(table);
	if (it != tableMap.end()) {
		tableMap.erase(it);
		num_of_table--;
		save();
		string path = CATA_PATH + table + ".txt";
		remove(path.c_str());
		return true;
	}
	cout << "No table to delect !" << endl;
	return false;
}
/*删除表*/
Table CatalogManager::get_table_info(string table) {
	return tableMap[table];
}
/*获取表信息，具体返回类型尚待商榷*/
attrbute CatalogManager::get_attr_info(string table, string attr) {
	return tableMap[table].colum[tableMap[table].colummap[attr]];
}
/*获取列信息，具体返回类型尚待商榷*/
bool CatalogManager::isUnique(string table, string attr) {
	return tableMap[table].colum[tableMap[table].colummap[attr]].unique;
}
/*检查某表某字段是否唯一（字段的属性是否是unique）*/
bool CatalogManager::hasIndex(string table, string attr) {
	if (tableMap[table].colum[tableMap[table].colummap[attr]].index == nullptr) {
		return false;
	}
	return true;
}
/*检查某表某字段是否有索引*/
Index* CatalogManager::indexName(string table, string attr) {
	return tableMap[table].colum[tableMap[table].colummap[attr]].index;
}
/*返回某字段上所有的索引，返回类型尚待商榷*/
bool CatalogManager::isPK(string table, string attr) {
	if (tableMap[table].primarykey != -1)
		return tableMap[table].colum[tableMap[table].primarykey].name == attr;
	else
		return false;
}
/*检查某表是否有主键*/
string CatalogManager::pkOnTable(string table) {
	if (tableMap[table].primarykey != -1)
		return tableMap[table].colum[tableMap[table].primarykey].name;
	else
		return "false";
}
bool CatalogManager::set_zero(string tablename)
{
	tableMap[tablename].recordindex = 0;
	return true;
}
/*寻找表中主键*/
bool CatalogManager::setpk(string table, string attr) {
	tableMap[table].primarykey = tableMap[table].colummap[attr];
	tableMap[table].colum[tableMap[table].primarykey].unique = true;
	savetable(table);
	return true;
}
Index CatalogManager::get_index_info(string index) {
	return indexMap[index];
}
/*获取某索引信息，返回类型尚待商榷*/
map<string, Index> CatalogManager::get_all_index() {
	return indexMap;
}
/*获取所有索引信息*/
int CatalogManager::indexCount(string table) {
	return tableMap[table].num_of_index;
}
/*返回索引的总数*/
string CatalogManager::tableIndexOn(string index) {
	return indexMap[index].tableName;
}
/*获取索引建立在哪个表*/
string CatalogManager::attrIndexOn(string index) {
	return indexMap[index].columnName;
}
/*获取索引建立在哪个字段*/
bool CatalogManager::addIndex(string index, string table, string attr) {
	if (currentdatabase != "nullptr") {
		if (indexExists(index)) {
			cout << "the index have existed" << endl;
			return false;
		}
		Index i;
		i.columnName = attr;
		i.tableName = table;
		i.indexname = index;
		indexMap[index] = i;
		tableMap[table].num_of_index++;
		tableMap[table].colum[tableMap[table].colummap[attr]].index = &indexMap[index];
		savetable(table);
		return true;
	}
	cout << "you have't use any database" << endl;
	return false;
}
/*添加索引*/
bool CatalogManager::deleteIndex(string index) {
	map<string, Index>::iterator it = indexMap.find(index);
	if (it != indexMap.end()) {
		tableMap[it->second.tableName].colum[tableMap[it->second.tableName].colummap[it->second.columnName]].index = nullptr;
		tableMap[it->second.tableName].num_of_index--;
		savetable(it->second.tableName);
		indexMap.erase(it);
		return true;
	}
	cout << "No index to delect !" << endl;
	return false;
}
void CatalogManager::Insert_Data(string tablename) {
	tableMap[tablename].recordindex++;
	savetable(tablename);
}
