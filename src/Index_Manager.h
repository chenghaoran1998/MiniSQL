#ifndef bptree_hpp
#define bptree_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <string>
#include "Buffer_Manager.h"
using namespace std;

class IndexManager
{
public:
	void Create_Index(string Table_Name, string Attr, int IndexType);
	void Delete_Index(string Table_Name, string Attr);
	void Insert_Index(string Table_Name, string Attr, string key, int Addr, int IndexType);
	void Deletein_Index(string Table_Name, string Attr, string key, int IndexType);
	int serach_index_equal(string tablename, string attribute, string key);
	int* serach_index_greater(string tablename, string attribute, string key);
	int* serach_index_less(string tablename, string attribute, string key);
};

extern IndexManager ind;

struct datacon
{
	string key;
	int Addr;
};

class bpnode
{
public:
	datacon * con[5];
	int last, next, child[6], fa;
	bpnode();
};

int get_index_root(string Index_Name);
int get_index_lastnode(string Index_Name);
int apply_index_newnode(string Index_Name);
void form_index_head(string Index_Name);

char* node_to_char(bpnode cur);
bpnode* char_to_node(char* cur);

bool isleaf(bpnode* root);
int bpgetmax(bpnode* root);

int bpinsert(string Index_Name, int cur, datacon* newcon); //把newcon插入root树中
int bpdelete(string Index_Name, int cur, datacon* newcon);

int bpseracheq(string Index_Name, int cur, datacon* newcon);
string bpserachless(string Index_Name, int cur, datacon* newcon);//小于等于key的最大数
string bpserachmore(string Index_Name, int cur, datacon* newcon);//大于等于key的最小数

int* lianjieint(int* a, int* b);

int* bpserach_allmore(string Index_Name, int cur, datacon* newcon);
int* bpserach_allless(string Index_Name, int cur, datacon* newcon);
int *bpserach_getall(string Index_Name, int cur, datacon* newcon);

///////////////////////////////////////////////////test
bool is_index_been(string filename);

#endif /* bptree_hpp */
