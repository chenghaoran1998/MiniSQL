#include "Index_Manager.h"
#include "Buffer_Manager.h"
//#define ROOT_PATH "G://VisualStudioProject/Project1/Index"
#define MAXN 1000
#define MAXDATA 100000
#define LENPAGE 300
IndexManager ind;

//int main()
//{
//	int a, t, *tt;
//	string s("test"), Attr("test1");
//	ind.Delete_Index(s, Attr);
//	ind.Create_Index(s, Attr, 0);
//	/*	ind.Insert_Index(s, Attr, "testno3", 4, 0);
//	ind.Insert_Index(s, Attr, "testno4", 5, 0);
//	ind.Insert_Index(s, Attr, "testno5", 6, 0);
//	ind.Insert_Index(s, Attr, "testno6", 7, 0);
//	ind.Insert_Index(s, Attr, "testno7", 8, 0);
//	ind.Insert_Index(s, Attr, "testno8", 9, 0);
//	ind.Insert_Index(s, Attr, "testno9", 10, 0);
//	ind.Insert_Index(s, Attr, "testno10", 11, 0);
//	ind.Insert_Index(s, Attr, "testno11", 12, 0);
//	ind.Insert_Index(s, Attr, "testno12", 13, 0);
//	ind.Insert_Index(s, Attr, "testno13", 14, 0);
//	ind.Insert_Index(s, Attr, "testno14", 15, 0);*/
//
//	ind.Insert_Index(s, Attr, "testno3", 4, 0);
//	ind.Insert_Index(s, Attr, "testno4", 5, 0);
//	ind.Insert_Index(s, Attr, "testno5", 6, 0);
//	ind.Insert_Index(s, Attr, "testno6", 7, 0);
//	ind.Insert_Index(s, Attr, "testno7", 8, 0);
//	ind.Insert_Index(s, Attr, "testno8", 9, 0);
//	ind.Insert_Index(s, Attr, "testno9", 10, 0);
//	ind.Insert_Index(s, Attr, "testno10", 11, 0);
//	ind.Insert_Index(s, Attr, "testno11", 12, 0);
//	ind.Insert_Index(s, Attr, "testno12", 13, 0);
//	ind.Insert_Index(s, Attr, "testno13", 14, 0);
//	ind.Insert_Index(s, Attr, "testno14", 15, 0);
//	ind.Deletein_Index(s, Attr, "testno1", 0);
//	ind.Deletein_Index(s, Attr, "testno2", 0);
//	ind.Deletein_Index(s, Attr, "testno3", 0);
//	ind.Deletein_Index(s, Attr, "testno4", 0);
//	ind.Deletein_Index(s, Attr, "testno5", 0);
//	ind.Deletein_Index(s, Attr, "testno6", 0);
//	ind.Deletein_Index(s, Attr, "testno7", 0);
//	string tttt("testno11");
//	; tt = ind.serach_index_greater(s, Attr, tttt);
//	cout << *tt << *(tt + 1) << *(tt + 2) << *(tt + 3) << *(tt + 4) << *(tt + 5) << *(tt + 5);
//	while (1);
//	ind.Deletein_Index(s, Attr, "testno8", 0);
//	ind.Deletein_Index(s, Attr, "testno9", 0);
//	ind.Deletein_Index(s, Attr, "testno10", 0);
//	ind.Deletein_Index(s, Attr, "testno11", 0);
//	ind.Deletein_Index(s, Attr, "testno12", 0);
//	ind.Deletein_Index(s, Attr, "testno13", 0);
//	ind.Deletein_Index(s, Attr, "testno14", 0);
//
//	/*	string s("test"), Attr("test1");
//	ind.Delete_Index(s, Attr);
//	ind.Create_Index(s, Attr, 0);
//	ind.Insert_Index(s, Attr, "testno3", 4, 0);
//	ind.Insert_Index(s, Attr, "testno4", 5, 0);
//	ind.Insert_Index(s, Attr, "testno5", 6, 0);
//	ind.Insert_Index(s, Attr, "testno6", 7, 0);
//	string tttt("testno4");
//	t = ind.serach_index_equal(s, Attr, tttt);
//	tt = ind.serach_index_less(s, Attr, tttt);
//	tt = ind.serach_index_greater(s, Attr, tttt);
//	ind.Deletein_Index(s, Attr, "testno1", 0);
//	ind.Deletein_Index(s, Attr, "testno2", 0);
//	ind.Deletein_Index(s, Attr, "testno3", 0);
//	//	Delete_Index(s, Attr);*/
//	return 0;
//}

bpnode::bpnode(void)
{
	for (int i = 0; i <= 4; i++)
	{
		con[i] = new datacon;
		con[i]->Addr = 0;
		con[i]->key = "";
	}
	memset(child, 0, sizeof(child));
	last = next = fa = 0;
}

int get_index_root(string Index_Name)
{
	char* s = bm.read(Index_Name, 1 * LENPAGE);
	char t[500];
	int ti = 0;
	for (int i = 0; i < strlen(s); i++)
		if (*(s + i) == 'r')
		{
			for (i++; (i < strlen(s) && (*(s + i)) != '|'); i++)
				t[ti++] = *(s + i);
			t[ti] = 0;
			break;
		}
	return atoi(t);
}

void reset_index_root(string Index_Name, int root)
{
	char* s = bm.read(Index_Name, 1 * LENPAGE), t[500], t2[500];
	int ans, ti = 0, ti2 = 0, tistart;
	for (int i = 0; i < strlen(s); i++)
	{
		if (*(s + i) == 'r')
		{
			for (i++; (i < strlen(s) && *(s + i) != '|'); i++)
			{
				t[ti++] = *(s + i);
			}
			t[ti] = 0;
		}
		if (*(s + i) == 'l')
		{
			for (i++; (i < strlen(s) && *(s + i) != '|'); i++)
			{
				t2[ti2++] = *(s + i);
			}
			t2[ti2] = 0;
		}
	}
	memset(s, 0, sizeof(s));
	*s = 'r';
	_itoa_s(root, t, 10);
	strcat(s, t);
	strcat(s, "|l");
	strcat(s, t2);
	strcat(s, "|");
	bm.write(Index_Name, 1 * LENPAGE, s, strlen(s));
}

int get_index_lastnode(string Index_Name)
{
	;
	char* s = bm.read(Index_Name, 1 * LENPAGE), t[500];
	int ti = 0;
	for (int i = 0; i < strlen(s); i++)
		if (*(s + i) == 'l')
		{
			for (i++; (i < strlen(s) && (*(s + i)) != '|'); i++)
				t[ti++] = *(s + i);
			t[ti] = 0;
			break;
		}
	return atoi(t);
}

int apply_index_newnode(string Index_Name)
{
	;
	char* s = bm.read(Index_Name, 1 * LENPAGE), t[500], t2[500];
	int ans, ti = 0, ti2 = 0, tistart;
	for (int i = 0; i < strlen(s); i++)
		if (*(s + i) == 'l')
		{
			tistart = i;
			for (i++; (i < strlen(s) && (*(s + i)) != '|'); i++)
				t[ti++] = *(s + i);
			t[ti] = 0;
			for (; i <= strlen(s); i++)
				t2[ti2++] = *(s + i);
			*(s + tistart + 1) = 0;
			break;
		}
	ans = atoi(t) + 1;
	ti = 0;
	_itoa_s(ans, t, 10);
	strcat_s(s, sizeof(s) + 10, t);
	strcat_s(s, sizeof(s) + 10, t2);
	bm.write(Index_Name, 1 * LENPAGE, s, strlen(s));
	return ans;
}

void form_index_head(string Index_Name)
{
	ofstream mcfile; //创建个对象  
	mcfile.open(Index_Name); //创建文件  
	char s[10] = "r2|l2|";
	bm.write(Index_Name, 1 * LENPAGE, s, strlen(s));
	bpnode* newroot = new bpnode;
	char *s2 = node_to_char(*newroot);
	bm.write(Index_Name, 2 * LENPAGE, s2, strlen(s2));
	return;
}

char* node_to_char(bpnode cur)
{
	char *s = new char[1000]; *s = 0;
	char fenge[5] = "|";
	char *tt = new char[100]; *tt = 0;
	for (int i = 0; i <= 4; i++)
	{
		strcat_s(s, MAXN, "|");
		strcat_s(s, MAXN, cur.con[i]->key.c_str());
		strcat_s(s, MAXN, "|");
		_itoa_s(cur.con[i]->Addr, tt, sizeof(tt), 10);
		strcat_s(s, MAXN, tt);
	}
	_itoa_s(cur.last, tt, sizeof(tt), 20);
	strcat_s(s, MAXN, "|"); strcat_s(s, MAXN, tt);
	_itoa_s(cur.next, tt, sizeof(tt), 10);
	strcat_s(s, MAXN, "|"); strcat_s(s, MAXN, tt);
	_itoa_s(cur.fa, tt, sizeof(tt), 10);
	strcat_s(s, MAXN, "|"); strcat_s(s, MAXN, tt);
	for (int i = 0; i <= 5; i++)
	{
		strcat_s(s, MAXN, "|");
		_itoa_s(cur.child[i], tt, sizeof(tt), 10);
		strcat_s(s, MAXN, tt);
	}
	strcat_s(s, MAXN, "|");
	return s;
}

bpnode* char_to_node(char* cur)
{
	if (!cur)
		return NULL;
	bpnode *ans = new bpnode;
	int i = 0, ti;
	char t[10000];

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	string tcon0(t);
	ans->con[0]->key = tcon0;

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->con[0]->Addr = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	string tcon1(t);
	ans->con[1]->key = tcon1;

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->con[1]->Addr = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	string tcon2(t);
	ans->con[2]->key = tcon2;

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->con[2]->Addr = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	string tcon3(t);
	ans->con[3]->key = tcon3;

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->con[3]->Addr = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	string tcon4(t);
	ans->con[4]->key = tcon4;

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->con[4]->Addr = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->last = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->next = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->fa = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->child[0] = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->child[1] = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->child[2] = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->child[3] = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->child[4] = atoi(t);

	i++; ti = 0;
	for (; *(cur + i) != '|'; i++, ti++)
		t[ti] = *(cur + i);
	t[ti] = 0;
	ans->child[5] = atoi(t);

	return ans;
}

bool isleaf(bpnode* root)
{
	if (root->con[0]->key == "")
		return true;
	if (!root->child[0])
		return true;
	return false;
}

int bpgetmax(bpnode* root)
{
	int i = 4;
	while (i + 1)
	{
		char *s = (char*)root->con[i]->key.c_str();
		if (*s)
			return i;
		i--;
	}
	return -1;
}

int bpinsert(string Index_Name, int cur, datacon* newcon) //把newcon插入root树中
{
	BufferManager a;
	bpnode *root = char_to_node(a.read(Index_Name, cur * LENPAGE));
	if (root != NULL && root->con[0]->key == "")
	{
		//cout << newcon->key << '-' << root->con[0]->key << endl;
		root->con[0]->key = newcon->key;
		root->con[0]->Addr = newcon->Addr;
		//cout << newcon->key << '-' << root->con[0]->key << endl;
		char *s = new char[100];
		s = node_to_char(*root);
		bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
		return cur;
	}
	if (!isleaf(root))
	{
		if (*(root->con[0]->key.c_str()) && (newcon->key < root->con[0]->key))
			bpinsert(Index_Name, root->child[0], newcon);
		else if (!*root->con[1]->key.c_str() || (newcon->key < root->con[1]->key))
			bpinsert(Index_Name, root->child[1], newcon);
		else if (!*root->con[2]->key.c_str() || (newcon->key < root->con[2]->key))
			bpinsert(Index_Name, root->child[2], newcon);
		else if (!*root->con[3]->key.c_str() || (newcon->key < root->con[3]->key))
			bpinsert(Index_Name, root->child[3], newcon);
		else
			bpinsert(Index_Name, root->child[4], newcon);
		bpnode *root = char_to_node(a.read(Index_Name, cur * LENPAGE));
		if (root->con[4]->key != "")                //对内部节点进行分裂处理
		{
			int newchildno = apply_index_newnode(Index_Name);
			bpnode* newchild = new bpnode;//分裂
			newchild->fa = root->fa;
			newchild->last = cur;
			root->next = newchildno;
			*(newchild->con[0]) = *(root->con[2]); root->con[2]->key = "";
			*(newchild->con[1]) = *(root->con[3]); root->con[3]->key = "";
			*(newchild->con[2]) = *(root->con[4]); root->con[4]->key = "";

			if (!root->fa)
			{
				int newrootno = apply_index_newnode(Index_Name);
				bpnode* newroot = new bpnode;
				root->fa = newrootno;
				newchild->fa = newrootno;
				newroot->child[0] = cur;
				newroot->child[1] = newchildno;
				*(newroot->con[0]) = *(newchild->con[0]);
				char *s = new char[100];
				s = node_to_char(*root);
				bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
				s = node_to_char(*newchild);
				bm.write(Index_Name, newchildno * LENPAGE, s, strlen(s));
				s = node_to_char(*newroot);
				bm.write(Index_Name, newrootno * LENPAGE, s, strlen(s));
				return newrootno;
			}
			else
			{
				bpnode* rootfa = char_to_node(bm.read(Index_Name, root->fa * LENPAGE));
				if (newchild->con[0]->key < rootfa->con[0]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(rootfa->con[2]); rootfa->child[3] = rootfa->child[2];
					*(rootfa->con[2]) = *(rootfa->con[1]); rootfa->child[2] = rootfa->child[1];
					*(rootfa->con[1]) = *(rootfa->con[0]); rootfa->child[1] = rootfa->child[0];
					*(rootfa->con[0]) = *(newchild->con[0]); rootfa->child[0] = cur; rootfa->child[1] = newchildno;
				}
				else if (newchild->con[0]->key < rootfa->con[1]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(rootfa->con[2]); rootfa->child[3] = rootfa->child[2];
					*(rootfa->con[2]) = *(rootfa->con[1]); rootfa->child[2] = rootfa->child[1];
					*(rootfa->con[1]) = *(newchild->con[0]); rootfa->child[1] = cur; rootfa->child[2] = newchildno;
				}
				else if (newchild->con[0]->key < rootfa->con[2]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(rootfa->con[2]); rootfa->child[3] = rootfa->child[2];
					*(rootfa->con[2]) = *(newchild->con[0]); rootfa->child[2] = cur; rootfa->child[3] = newchildno;
				}
				else if (newchild->con[0]->key < rootfa->con[3]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(newchild->con[0]); rootfa->child[3] = cur; rootfa->child[4] = newchildno;
				}
				else
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(newchild->con[0]); rootfa->child[4] = cur; rootfa->child[5] = newchildno;
				}
				char *s = new char[100];
				s = node_to_char(*root);
				bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
				s = node_to_char(*newchild);
				bm.write(Index_Name, newchildno * LENPAGE, s, strlen(s));
				s = node_to_char(*rootfa);
				bm.write(Index_Name, root->fa * LENPAGE, s, strlen(s));
				return root->fa;
			}
		}
		return root->fa ? root->fa : cur;
	}
	else
	{
		int i = 4, j;                           //插入
		datacon t;
		while (i >= 0 && root->con[i]->key == "")
			i--;
		j = i;
		while (i >= 0 && root->con[i]->key > newcon->key)
			i--;
		for (; j > i; j--)
			*(root->con[j + 1]) = *(root->con[j]);
		root->con[j + 1]->key = newcon->key;
		root->con[j + 1]->Addr = newcon->Addr;

		if (root->con[4]->key != "")
		{
			bpnode* newchild = new bpnode;//分裂
			int newchildno = apply_index_newnode(Index_Name);
			newchild->fa = root->fa;
			newchild->last = cur;
			root->next = newchildno;
			*(newchild->con[0]) = *(root->con[2]); root->con[2]->key = "";
			*(newchild->con[1]) = *(root->con[3]); root->con[3]->key = "";
			*(newchild->con[2]) = *(root->con[4]); root->con[4]->key = "";

			if (!root->fa)
			{
				bpnode* newroot = new bpnode;
				int newrootno = apply_index_newnode(Index_Name);
				root->fa = newrootno;
				newchild->fa = newrootno;
				newroot->child[0] = cur;
				newroot->child[1] = newchildno;
				*(newroot->con[0]) = *(newchild->con[0]);

				char *s = new char[100];
				s = node_to_char(*root);
				bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
				s = node_to_char(*newchild);
				bm.write(Index_Name, newchildno * LENPAGE, s, strlen(s));
				s = node_to_char(*newroot);
				bm.write(Index_Name, newrootno * LENPAGE, s, strlen(s));
				return newrootno;
			}
			else
			{
				bpnode* rootfa = char_to_node(bm.read(Index_Name, root->fa * LENPAGE));
				if (newchild->con[0]->key < rootfa->con[0]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(rootfa->con[2]); rootfa->child[3] = rootfa->child[2];
					*(rootfa->con[2]) = *(rootfa->con[1]); rootfa->child[2] = rootfa->child[1];
					*(rootfa->con[1]) = *(rootfa->con[0]); rootfa->child[1] = rootfa->child[0];
					*(rootfa->con[0]) = *(newchild->con[0]); rootfa->child[0] = cur; rootfa->child[1] = newchildno;
				}
				else if (newchild->con[0]->key < rootfa->con[1]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(rootfa->con[2]); rootfa->child[3] = rootfa->child[2];
					*(rootfa->con[2]) = *(rootfa->con[1]); rootfa->child[2] = rootfa->child[1];
					*(rootfa->con[1]) = *(newchild->con[0]); rootfa->child[1] = cur; rootfa->child[2] = newchildno;
				}
				else if (newchild->con[0]->key < rootfa->con[2]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4] = rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3] = rootfa->con[2]); rootfa->child[3] = rootfa->child[2];
					*(rootfa->con[2] = newchild->con[0]); rootfa->child[2] = cur; rootfa->child[3] = newchildno;
				}
				else if (newchild->con[0]->key < rootfa->con[3]->key)
				{
					rootfa->child[5] = rootfa->child[4];
					*(rootfa->con[4]) = *(rootfa->con[3]); rootfa->child[4] = rootfa->child[3];
					*(rootfa->con[3]) = *(newchild->con[0]); rootfa->child[3] = cur; rootfa->child[4] = newchildno;
				}
				else
				{
					rootfa->child[bpgetmax(rootfa) + 1] = cur; rootfa->child[bpgetmax(rootfa) + 2] = newchildno;
					*(rootfa->con[bpgetmax(rootfa) + 1]) = *(newchild->con[0]);
				}
				char *s = new char[100];
				s = node_to_char(*root);
				bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
				s = node_to_char(*newchild);
				bm.write(Index_Name, newchildno * LENPAGE, s, strlen(s));
				s = node_to_char(*rootfa);
				bm.write(Index_Name, root->fa * LENPAGE, s, strlen(s));
				return root->fa;
			}
		}
		else
		{
			char *s = new char[100];
			s = node_to_char(*root);
			bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
			return root->fa ? root->fa : cur;
		}
	}
}

int bpdelete(string Index_Name, int cur, datacon* newcon)
{
	;
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	bpnode* rootlast = char_to_node(bm.read(Index_Name, root->last * LENPAGE));
	bpnode* rootfa = char_to_node(bm.read(Index_Name, root->fa * LENPAGE));
	bpnode* rootnext = char_to_node(bm.read(Index_Name, root->next * LENPAGE));

	int i, ret = 0;
	if (!isleaf(root))
	{
		if (*(root->con[0]->key.c_str()) && (newcon->key < root->con[0]->key))
			ret = bpdelete(Index_Name, root->child[0], newcon);
		else if (!(*(root->con[1]->key.c_str())) || (newcon->key < root->con[1]->key))
			ret = bpdelete(Index_Name, root->child[1], newcon);
		else if (!(*(root->con[2]->key.c_str())) || (newcon->key < root->con[2]->key))
			ret = bpdelete(Index_Name, root->child[2], newcon);
		else if (!(*(root->con[3]->key.c_str())) || (newcon->key < root->con[3]->key))
			ret = bpdelete(Index_Name, root->child[3], newcon);
		else
			ret = bpdelete(Index_Name, root->child[4], newcon);

		root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
		if (root->con[0]->key == "")
			return ret;

		ret = 0;
		if (!(*(root->con[1]->key.c_str())))                //对内部节点进行合并处理
		{
			if (rootlast && rootlast->fa == root->fa && rootlast->con[2]->key != "")//向前一个兄弟借
			{
				*(root->con[1]) = *(root->con[0]);
				root->child[2] = root->child[1];
				root->child[1] = root->child[0];
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == root->last)
						break;
				*(root->con[0]) = *(rootfa->con[i]);
				*(rootfa->con[i]) = *(rootlast->con[bpgetmax(rootlast)]);
				root->child[0] = rootlast->child[bpgetmax(rootlast) + 1];
				rootlast->child[bpgetmax(rootlast) + 1] = 0;
				rootlast->con[bpgetmax(rootlast) + 1]->key = "";
			}
			else if (root->next && rootnext->fa == root->fa && rootnext->con[2]->key != "")
			{
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == root->last)
						break;
				*(root->con[1]) = *(rootfa->con[i]);
				root->child[2] = rootnext->child[0];
				*(rootfa->con[i]) = *(rootnext->con[0]);
				*(rootnext->con[0]) = *(rootnext->con[1]);
				*(rootnext->con[1]) = *(rootnext->con[2]);
				*(rootnext->con[2]) = *(rootnext->con[3]);
				*(rootnext->con[3]) = *(rootnext->con[4]);
				rootnext->child[1] = rootnext->child[2];
				rootnext->child[2] = rootnext->child[3];
				rootnext->child[3] = rootnext->child[4];
				rootnext->child[4] = rootnext->child[5];
			}
			else if (root->last && rootlast->fa == root->fa)//与前一个兄弟合并
			{
				int maxlast = bpgetmax(rootlast);
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == root->last)
						break;
				*(rootlast->con[maxlast + 1]) = *(rootfa->con[i - 1]);
				*(rootlast->con[maxlast + 2]) = *(root->con[0]);
				rootlast->child[maxlast + 2] = root->child[0];
				rootlast->child[maxlast + 3] = root->child[1];
				while (rootfa->con[i - 1]->key != "")
				{
					*(rootfa->con[i - 1]) = *(rootfa->con[i]);
					rootfa->child[i] = rootfa->child[i + 1];
				}
				rootnext->last = root->last;
				rootlast->next = root->next;
				root->fa = root->last;//返回兄弟
			}
			else if (root->next && rootnext->fa == root->fa)
			{
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == root->last)
						break;
				*(rootnext->con[3]) = *(rootnext->con[1]);
				*(rootnext->con[2]) = *(rootnext->con[0]);
				rootnext->child[4] = rootnext->child[2];
				rootnext->child[3] = rootnext->child[1];
				rootnext->child[2] = rootnext->child[0];
				rootnext->child[1] = root->child[1];
				rootnext->child[0] = root->child[0];
				*(rootnext->con[1]) = *(rootfa->con[i]);
				*(rootnext->con[0]) = *(root->con[0]);
				while (rootfa->con[i]->key != "")
				{
					*(rootfa->con[i]) = *(rootfa->con[i + 1]);
					rootfa->child[i] = rootfa->child[i + 1];
				}
				rootfa->child[i + 1] = NULL;
				rootnext->last = root->last;
				rootlast->next = root->next;
				root->fa = root->next;
			}
		}

		char *s = new char[100];
		if (root->fa)
		{
			s = node_to_char(*rootfa);
			bm.write(Index_Name, root->fa * LENPAGE, s, strlen(s));
		}
		if (root->last && rootlast->fa == root->fa)
		{
			s = node_to_char(*rootlast);
			bm.write(Index_Name, root->last * LENPAGE, s, strlen(s));
		}
		if (root->next && rootnext->fa == root->fa)
		{
			s = node_to_char(*rootnext);
			bm.write(Index_Name, root->next * LENPAGE, s, strlen(s));
		}

		return (root->fa && rootfa->con[0]) ? (root->fa = 0) : cur;
	}
	else
	{
		if (root->con[0]->key == newcon->key)
		{
			*(root->con[0]) = *(root->con[1]);
			*(root->con[1]) = *(root->con[2]);
			*(root->con[2]) = *(root->con[3]);
			root->con[3]->key = "";
		}
		else if (root->con[1]->key == newcon->key)
		{
			*(root->con[1]) = *(root->con[2]);
			*(root->con[2]) = *(root->con[3]);
			root->con[3]->key = "";
		}
		else if (root->con[2]->key == newcon->key)
		{
			*(root->con[2]) = *(root->con[3]);
			root->con[3]->key = "";
		}
		else if (root->con[3]->key == newcon->key)
		{
			root->con[3]->key = "";
		}
		else
			return -1;               //没有可删除的节点
		if (root->con[1]->key == "")
		{
			if (root->last && rootlast->fa == root->fa && rootlast->con[2]->key != "")//向前一个兄弟借
			{
				*(root->con[1]) = *(root->con[0]);
				*(root->con[0]) = *(rootlast->con[bpgetmax(rootlast)]);
				rootlast->con[bpgetmax(rootlast)]->key = "";
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == root->last)
						break;
				*(rootfa->con[i]) = *(root->con[0]);
			}
			else if (root->next && rootnext->fa == root->fa && rootnext->con[2]->key != "")
			{
				*(root->con[1]) = *(rootnext->con[0]);
				*(rootnext->con[0]) = *(rootnext->con[1]);
				*(rootnext->con[1]) = *(rootnext->con[2]);
				*(rootnext->con[2]) = *(rootnext->con[3]);
				rootnext->con[3]->key = "";
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == cur)
						break;
				*(rootfa->con[i]) = *(rootnext->con[0]);
			}
			else if (root->last && rootlast->fa == root->fa)//与前一个兄弟合并
			{
				*(rootlast->con[bpgetmax(rootlast) + 1]) = *(root->con[0]);
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == root->last)
						break;
				for (; i < 4; i++)
				{
					*(rootfa->con[i]) = *(rootfa->con[i + 1]);
					rootfa->child[i + 1] = rootfa->child[i + 2];
				}
				rootnext->last = root->last;
				rootlast->next = root->next;
				ret = root->last;//返回兄弟
			}
			else if (root->next && rootnext->fa == root->fa)
			{
				*(rootnext->con[2]) = *(rootnext->con[1]);
				*(rootnext->con[1]) = *(rootnext->con[0]);
				*(rootnext->con[0]) = *(root->con[0]);
				for (i = 0; i < 4; i++)
					if (rootfa->child[i] == cur)
						break;
				for (; i < 4; i++)
				{
					*(rootfa->con[i]) = *(rootfa->con[i + 1]);
					rootfa->child[i] = rootfa->child[i + 1];
				}
				rootfa->child[4] = 0;
				rootlast->next = root->next;
				rootnext->last = root->last;
				ret = root->next;//返回兄弟
			}
		}
		char *s = new char[100];
		s = node_to_char(*root);
		bm.write(Index_Name, cur * LENPAGE, s, strlen(s));
		if (root->fa)
		{
			s = node_to_char(*rootfa);
			bm.write(Index_Name, root->fa * LENPAGE, s, strlen(s));
		}
		if (root->last && rootlast->fa == root->fa)
		{
			s = node_to_char(*rootlast);
			bm.write(Index_Name, root->last * LENPAGE, s, strlen(s));
		}
		if (root->next && rootnext->fa == root->fa)
		{
			s = node_to_char(*rootnext);
			bm.write(Index_Name, root->next * LENPAGE, s, strlen(s));
		}

		if (ret != 0)
			return ret;
		return (root->fa && rootfa->con[0]) ? (root->fa = 0) : cur;
	}
}

int bpseracheq(string Index_Name, int cur, datacon* newcon)
{
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	if (isleaf(root))
	{
		for (int i = 0; i <= 4; i++)
			if (root->con[i]->key != "" && root->con[i]->key == newcon->key)
				return root->con[i]->Addr;
		return -1;
	}
	else
	{
		for (int i = 0; i <= 4; i++)
			if (root->con[i]->key != "" && (root->con[i]->key > newcon->key))
				return bpseracheq(Index_Name, root->child[i], newcon);
		return bpseracheq(Index_Name, root->child[bpgetmax(root) + 1], newcon);
	}
}

string bpserachless(string Index_Name, int cur, datacon* newcon)//小于等于key的最大数
{
	BufferManager a;
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	if (isleaf(root))
	{
		for (int i = 1; i <= 4; i++)
			if (root->con[i]->key != "" && root->con[i]->key > newcon->key)
				return root->con[i - 1]->key;
		return root->con[bpgetmax(root)]->key;
	}
	else
	{
		for (int i = 0; i <= 4; i++)
			if (root->con[i]->key != "" && (root->con[i]->key > newcon->key))
				return bpserachless(Index_Name, root->child[i], newcon);
		return bpserachless(Index_Name, root->child[bpgetmax(root) + 1], newcon);
	}
}

string bpserachmore(string Index_Name, int cur, datacon *newcon)//大于等于key的最小数
{
	BufferManager a;
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	if (isleaf(root))
	{
		for (int i = 4; i >= 0; i--)
			if (root->con[i]->key != "" && root->con[i]->key < newcon->key)
				return root->con[i + 1]->key;
		return root->con[0]->key;
	}
	else
	{
		for (int i = 0; i <= 4; i++)
			if (root->con[i]->key != "" && (root->con[i]->key > newcon->key))
				return bpserachmore(Index_Name, root->child[i], newcon);
		return bpserachmore(Index_Name, root->child[bpgetmax(root) + 1], newcon);
	}
}

int* lianjieint(int* a, int* b)
{
	int i;
	for (i = 0; *(a + i); i++)
		;
	for (int j = 0; *(b + j); i++, j++)
		*(a + i) = *(b + j);
	return a;
}

int *bpserach_getall(string Index_Name, int cur, datacon* newcon)
{
	BufferManager a;
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	int *ans = new int[MAXDATA], *tans;
	memset(ans, 0, sizeof(ans) * MAXDATA);
	int i = 0;
	if (!isleaf(root))
	{
		for (int i = 0; i <= 4; i++)
			if (root->con[i]->key != "")
			{
				tans = bpserach_getall(Index_Name, root->child[i], newcon);
				for (int j = 0; *(tans + j); j++)
					ans[i++] = *(tans + j);
			}
		return ans;
	}
	else
	{
		for (int j = 0; j <= 4; j++)
			if (root->con[j]->key != "")
				ans[i++] = root->con[j]->Addr;
		return ans;
	}
}

int* bpserach_allmore(string Index_Name, int cur, datacon* newcon)
{
	int i, j;
	BufferManager a;
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	if (!isleaf(root))
	{
		for (i = 0; i <= 2; i++)
			if (root->con[i + 1]->key != "" && (root->con[i + 1]->key > newcon->key))
			{
				int* child1 = new int[MAXDATA], *child2 = new int[MAXDATA];
				memset(child1, 0, sizeof(child1));
				memset(child2, 0, sizeof(child2));
				child1 = bpserach_allmore(Index_Name, root->child[i], newcon);
				for (; root->con[i + 1]->key != ""; i++)
				{
					child2 = bpserach_getall(Index_Name, root->child[i + 1], newcon);
					child1 = lianjieint(child1, child2);
				}
				return child1;
			}
		return bpserach_allmore(Index_Name, root->child[bpgetmax(root) + 1], newcon);
	}
	else
	{
		int *ans = new int[MAXDATA], ii = 0;
		memset(ans, 0, sizeof(ans) * MAXDATA);
		for (int i = 0; i <= 3; i++)
			if (root->con[i]->key >= newcon->key)
			{
				*(ans + ii) = root->con[i]->Addr;
				ii++;
			}
		return ans;
	}
}

int* bpserach_allless(string Index_Name, int cur, datacon* newcon)
{
	int *child1, *child2;
	int i, j;
	BufferManager a;
	bpnode* root = char_to_node(bm.read(Index_Name, cur * LENPAGE));
	int *ans = new int[MAXDATA], ii = 0;
	memset(ans, 0, sizeof(ans) * MAXDATA);
	if (!isleaf(root))
	{
		for (i = 3; i >= 0; i--)
			if (root->con[i]->key != "" && root->con[i]->key < newcon->key)
			{
				ans = lianjieint(ans, bpserach_allless(Index_Name, root->child[i + 1], newcon));
				for (; i >= 0; i--)
				{
					ans = lianjieint(ans, bpserach_getall(Index_Name, root->child[i], newcon));
				}
				return ans;
			}
		return bpserach_allless(Index_Name, root->child[0], newcon);
	}
	else
	{
		for (int i = 0; i <= 3; i++)
			if (root->con[i]->key <= newcon->key)
			{
				*(ans + ii) = root->con[i]->Addr;
				ii++;
			}
		return ans;
	}
}

int IndexManager::serach_index_equal(string tablename, string attribute, string key)
{
	string filename = tablename + attribute;
	string s(".index");
	filename = filename + s;
	datacon* newcon = new datacon;
	newcon->key = key;
	newcon->Addr = 0;
	return bpseracheq(filename, get_index_root(filename), newcon);
}

int* IndexManager::serach_index_greater(string tablename, string attribute, string key)
{
	string filename = tablename + attribute;
	string s(".index");
	filename = filename + s;
	datacon* newcon = new datacon;
	newcon->key = key;
	newcon->Addr = 0;
	string smallest = bpserachmore(filename, get_index_root(filename), newcon);
	newcon->key = smallest;
	int *ans = bpserach_allmore(filename, get_index_root(filename), newcon);
	return ans;
}

int* IndexManager::serach_index_less(string tablename, string attribute, string key)
{
	string filename = tablename + attribute;
	string s(".index");
	filename = filename + s;
	datacon* newcon = new datacon;
	newcon->key = key;
	newcon->Addr = 0;
	string biggest = bpserachless(filename, get_index_root(filename), newcon);
	newcon->key = biggest;
	int *ans = bpserach_allless(filename, get_index_root(filename), newcon);
	return ans;
}

/*test
*/
struct nodemess
{
	string value; int n;
};

void IndexManager::Create_Index(string Table_Name, string Attr, int IndexType)
{
	///////////////////////////////////////////int* rec = getrecord(Table_Name, Attr);
	struct nodemess rec[10];
	string messt("testno1");
	rec[0].value = messt;
	rec[0].n = 2;
	messt = "testno2";
	rec[1].value = messt;
	rec[1].n = 3;
	//test
	//	int *rec = getrecord(Table_Name, Attr);
	string filename = Table_Name + Attr;
	string s(".index");
	filename = filename + s;
	form_index_head(filename);
	for (int i = 0; (rec + i)->value != ""; i++)
	{
		int attr = (rec + i)->n;
		string key = (rec + i)->value;
		datacon* newcon = new datacon;
		newcon->key = key;
		newcon->Addr = attr;
		if (i == 0 || ind.serach_index_equal(Table_Name, Attr, key) == -1)
			bpinsert(filename, get_index_root(filename), newcon);
	}
}

void IndexManager::Delete_Index(string Table_Name, string Attr)
{
	string filename = Table_Name + Attr;
	string s(".index");
	filename = filename + s;
	BufferManager a;
	bm.freefile(filename);
}


void IndexManager::Insert_Index(string Table_Name, string Attr, string key, int Addr, int IndexType)
{
	string filename = Table_Name + Attr;
	string s(".index");
	filename = filename + s;
	datacon* newcon = new datacon;
	newcon->key = key;
	newcon->Addr = Addr;
	if (ind.serach_index_equal(Table_Name, Attr, key) == -1)
		reset_index_root(filename, bpinsert(filename, get_index_root(filename), newcon));
	return;
}

void IndexManager::Deletein_Index(string Table_Name, string Attr, string key, int IndexType)
{
	string filename = Table_Name + Attr;
	string s(".index");
	filename = filename + s;
	datacon* newcon = new datacon;
	newcon->key = key;
	newcon->Addr = get_index_lastnode(filename);
	int fafa = bpdelete(filename, get_index_root(filename), newcon);
	reset_index_root(filename, fafa);
	return;
}

bool is_index_been(string filename)/////////////////////////////////////////////////////test
{
	return true;
}