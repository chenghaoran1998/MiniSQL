
#include "Record_Manager.h"
//将字符数组里的某几位转化成字符串
string RecordManager::To_String(char*p, int offset1, int offset2)
{
	string s = "";
	int i = 0;
	for (i = offset1; i < offset2; i++)
		s = s + p[i];
	return s;
}
//通过属性表计算每条元组的长度 
int RecordManager::cal(int s[], int N)
{
	int i = 0;
	int sum = 0;
	while (i < N)
	{
		switch (s[i])
		{
		case 1:
			sum += 4;
			break;
		case 2:
			sum += 4;
			break;
		default:
			if (s[i] < 0)
				sum -= s[i];
			break;
		}
		i++;
	}
	return sum + 1;//最后加上是否被删除的标记，0代表有效，1代表被删除 
}
//打印表属性
int* RecordManager::Print_Head(int Attr_Type[], string Attr_Name[], int Attr_N)
{
	int* leng = new int[Attr_N];
	int i = 0;
	int j = 0;
	for (i = 0; i < Attr_N; i++)
	{
		switch (Attr_Type[i]) {
		case 1:
			leng[i] = 10;
			break;
		case 2:
			leng[i] = 10;
			break;
		default:
			if (Attr_Type[i] < 0)
				leng[i] = (-Attr_Type[i] > Attr_Name[i].length() ? -Attr_Type[i] : Attr_Name[i].length());
			break;
		}
		leng[i] = leng[i] > Attr_Name[i].length() ? leng[i] : Attr_Name[i].length();
	}
	Print_Row(leng, Attr_N);
	string T = "| ";
	string tmp = "";
	for (i = 0; i < Attr_N; i++)
	{
		tmp = Attr_Name[i];
		while (tmp.length() < leng[i])
			tmp += " ";
		T += tmp;
		T += " | ";
	}
	cout << T.c_str() << endl;
	Print_Row(leng, Attr_N);
	return leng;
}
//打印分行
void RecordManager::Print_Row(int leng[], int N)
{
	cout << "+";
	int i = 0, j = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < leng[i] + 2; j++)
			cout << "-";
		cout << "+";
	}
	cout << endl;
}
//执行没有where的select
void RecordManager::Select_All(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[])
{
	int length = cal(Attr_Type, Attr_N);//每行长度，根据属性固定 
	int *leng = Print_Head(Attr_Type, Attr_Name, Attr_N);

	string T;

	int num = FILE_SIZE / length;
	int index = N / num;
	cout << N << " " << index << " " << endl;
	int l = 0;
	int i = 0, j = 0;
	int k = 0;
	union F f;
	union I in;
	char* a = new char(4);
	string tmp;

	for (l = 0; l <= index; l++)//根据N来分页 
	{
		cout << l << endl;
		char * p = bm.readfromfile(Table_Name, l);
		//puts(p);
		//画select结果表格
		for (j = 0; j < (l == index ? N - index * num : num); j++)
		{
			if (*(p + (j + 1)*length - 1) == '1') continue;
			int tmp_offset = 0;
			T = "| ";
			for (i = 0; i < Attr_N; i++)//i小于长度或者该元组未被删除 
			{
				//拼接每一段 
				switch (Attr_Type[i]) {
				case 1:
					tmp = "";
					memcpy(in.byte, p + j * length + tmp_offset, 4);
					itoa(in.a, a, 10);
					tmp += a;
					while (tmp.length() < 10)
						tmp += " ";
					T += tmp;
					tmp_offset += 4;
					break;
				case 2:
					tmp = "";
					memcpy(f.byte, p + j * length + tmp_offset, 4);
					_gcvt((double)f.f, 6, a);
					tmp += a;
					while (tmp.length() < 10)
						tmp += "0";
					T += tmp;
					tmp_offset += 4;
					break;
				default:
					if (Attr_Type[i] < 0)
					{
						tmp = "";
						tmp += To_String(p, j*length + tmp_offset, j*length + tmp_offset - Attr_Type[i]);
						while (tmp.length() < leng[i])
							tmp += " ";
						T += tmp;
						tmp_offset -= Attr_Type[i];
					}
					break;
				}
				T += " | ";
			}
			if (T != "| ")
			{
				cout << T.c_str() << endl;
				k++;
			}
		}
	}
	//打印有多少行被选中 
	if (k == 0)
		cout << "empty set" << endl;
	else if (k == 1)
	{
		Print_Row(leng, Attr_N);
		cout << k << " row in set" << endl;
	}
	else if (k > 1)
	{
		Print_Row(leng, Attr_N);
		cout << k << " rows in set" << endl;
	}
	/*Pair* P = new Pair[N + 1];
	P = Get_Attr(N, Table_Name, "a", Attr_N, Attr_Type, Attr_Name);
	for (i = 0; P[i].n != -1; i++)
		cout << P[i].value << " " << P[i].n << endl;
	string value[1] = { "Tom" };
	cout << ExistValue(N, Table_Name, Attr_N, Attr_Type, Attr_Name, value, 0);*/
}
//select 和 delete 双用验证 
int RecordManager::verify(string temp, string Attr[], int op[], string Num[], int num, int Attr_Type[], string Attr_Name[], int n)
{
	int i = 0;
	int len = temp.length() - 1;
	while (temp.at(len) == ' ')
		len--;
	temp = temp.substr(0, len + 1);
	for (i = 0; i < num; i++)
	{
		if (Attr_Name[n] == Attr[i])
		{
			switch (op[i])
			{
			case 0:
				if (Attr_Type[n] != 2 && temp != Num[i])//与==相反 
					return 0;
				else if (Attr_Type[n] == 2 && (float)atof(temp.c_str()) != (float)atof(Num[i].c_str()))
					return 0;
				break;
			case 1:
				if (Attr_Type[n] != 2 && temp == Num[i])//与<>相反 
					return 0;
				else if (Attr_Type[n] == 2 && (float)atof(temp.c_str()) == (float)atof(Num[i].c_str()))
					return 0;
				break;
			case 2:
				if (Attr_Type[n] != 2 && temp >= Num[i])//。。 
					return 0;
				else if (Attr_Type[n] == 2 && (float)atof(temp.c_str()) >= (float)atof(Num[i].c_str()))
					return 0;
				break;
			case 3:
				if (Attr_Type[n] != 2 && temp <= Num[i])
					return 0;
				else if (Attr_Type[n] == 2 && (float)atof(temp.c_str()) <= (float)atof(Num[i].c_str()))
					return 0;
				break;
			case 4:
				if (Attr_Type[n] != 2 && temp > Num[i])
					return 0;
				else if (Attr_Type[n] == 2 && (float)atof(temp.c_str()) > (float)atof(Num[i].c_str()))
					return 0;
				break;
			case 5:
				if (Attr_Type[n] != 2 && temp < Num[i])
					return 0;
				else if (Attr_Type[n] == 2 && (float)atof(temp.c_str()) < (float)atof(Num[i].c_str()))
					return 0;
				break;
			}
		}
	}
	return 1;
}
//执行有索引查询
void RecordManager::Show(string Table_Name, int length, int Attr_N, int Attr_Type[], string Attr_Name[], int op_N, string Attr[], int op[], string Num[], int s[], int n)
{
	int i = 0, j = 0, k = 0;
	int num = FILE_SIZE / length;
	int index = 0;
	char * p = NULL;
	string T = "";
	string temp = "";
	int tmp_offset = 0;
	int flag = 1;
	for (j = 0; j < n; i++)
	{
		index = s[j] / num;
		p = bm.readfromfile(Table_Name, index);
		T = "";
		tmp_offset = (s[j] - index * num) * length;
		for (i = 0; i < Attr_N; i++)
		{
			T += "|";
			flag = 1;
			switch (Attr_Type[i]) {
			case 1:
				temp = To_String(p, j*length + tmp_offset, j*length + tmp_offset + 4);
				if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
				T += temp;
				tmp_offset += 4;
				break;
			case 2:
				temp = To_String(p, j*length + tmp_offset, j*length + tmp_offset + 4);
				if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
				T += temp;
				tmp_offset += 4;
				break;
			default:
				if (Attr_Type[i] < 0)
				{
					temp = To_String(p, j*length + tmp_offset, j*length + tmp_offset - Attr_Type[i]);
					if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
					T += temp;
					tmp_offset -= Attr_Type[i];
				}
				break;
			}
		}
		if (flag)//如果验证通过，就输出本条元组 
		{
			T += "|";
			cout << T.c_str() << endl;
			k++;
		}
	}
	//打印有多少行被选中
	if (k == 1)
		cout << k << " row in set" << endl;
	else if (k > 1)
		cout << k << " rows in set" << endl;
}
//执行有where的select
void RecordManager::Select(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], int op_N, string Attr[], int op[], string Num[], string Index[])
{
	int length = cal(Attr_Type, Attr_N);//每行长度，根据属性固定 
	int *leng = Print_Head(Attr_Type, Attr_Name, Attr_N);

	string T;
	string temp;

	int num = FILE_SIZE / length;
	int index = N / num;
	int l = 0;
	int i = 0, j = 0;
	int k = 0;
	union F f;
	union I in;
	char* a = new char(4);
	string tmp;

	int tmp_offset;
	int flag = 1;

	int **p = new int*[op_N];

	//	for (i = 0; i < op_N; i++)
	//	{
	//		for (j = 0; j < Attr_N; j++)
	//		{
	//			if (Attr_Name[j] == Attr[i] && Index[j] != "")
	//			{
	//				int *pre = NULL;
	//				int eq = 0;
	//				int *aft = NULL;
	//				int *s = NULL;
	//				int n = 0;
	//				switch(op[i])
	//				{
	//					case 0://==
	//						eq = serach_index_equal(Table_Name, Num[i]);
	//						s = new int[1];
	//						s[0] = eq; 
	//						n = 1;
	//						break;
	//					case 1://<>
	//						pre = serach_index_less(Table_Name, Num[i]);
	//						aft = search_index_greater(Table_Name, Num[i]);
	//						n = sizeof(pre)/sizeof(pre[0]) + sizeof(aft)/sizeof(aft[0]);
	//						s = new int [n];
	//						memcpy(s, pre, sizeof(pre)/sizeof(pre[0]));
	//						memcpy(s + sizeof(pre)/sizeof(pre[0], aft, sizeof(aft)/sizeof(aft[0])), aft, sizeof(aft)/sizeof(aft[0]));
	//						free(pre);
	//						free(aft);
	//						break;
	//					case 2://<
	//						s = serach_index_less(Table_Name, Num[i]);	
	//						n = sizeof(s)/sizeof(s[0]);	
	//						break;
	//					case 3://>
	//						s = search_index_greater(Table_Name, Num[i]);
	//						n = sizeof(s)/sizeof(s[0]);	
	//						break;
	//					case 4://<=
	//						pre = serach_index_less(Table_Name, Num[i]);
	//						eq = serach_index_equal(Table_Name, Num[i]);
	//						n = sizeof(pre)/sizeof(pre[0]) + 1;
	//						s = new int[n];
	//						s[0] = eq;
	//						memcpy(s+1, pre, n-1);
	//						free(pre);
	//						break;
	//					case 5://>=
	//						eq = serach_index_equal(Table_Name, Num[i]);
	//						aft = search_index_greater(Table_Name, Num[i]);
	//						n = sizeof(pre)/sizeof(pre[0]) + 1;
	//						s = new int[n];
	//						s[0] = eq;
	//						memcpy(s+1, aft, n-1);
	//						free(aft);
	//						break;
	//				}
	//				Show(Table_Name, length, Attr_N, Attr_Type, op_N, Attr, op, Num, s, n); 
	//				return;
	//			}
	//		}
	//	}

	for (l = 0; l <= index; l++)//根据N来分页 
	{
		char * p = bm.readfromfile(Table_Name, l);
		//画select结果表格 
		for (j = 0; j < (l == index ? N - index * num : num); j++)
		{
			if (*(p + (j + 1)*length - 1) == '1') continue;
			int tmp_offset = 0;
			T = "| ";
			flag = 1;
			for (i = 0; i < Attr_N; i++)
			{
				switch (Attr_Type[i]) {
				case 1:
					tmp = "";
					memcpy(in.byte, p + j * length + tmp_offset, 4);
					itoa(in.a, a, 10);
					tmp += a;
					if ((flag = verify(tmp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
					if (flag == 0) break;
					while (tmp.length() < 10)
						tmp += " ";
					T += tmp;
					tmp_offset += 4;
					break;
				case 2:
					tmp = "";
					memcpy(f.byte, p + j * length + tmp_offset, 4);
					_gcvt((double)f.f, 4, a);
					tmp += a;
					if ((flag = verify(tmp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
					if (flag == 0) break;
					while (tmp.length() < 10)
						tmp += "0";
					T += tmp;
					tmp_offset += 4;
					break;
				default:
					if (Attr_Type[i] < 0)
					{
						temp = To_String(p, j*length + tmp_offset, j*length + tmp_offset - Attr_Type[i]);
						if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
						if (flag == 0) break;
						while (temp.length() < leng[i])
							temp += " ";
						T += temp;
						tmp_offset -= Attr_Type[i];
					}
					break;
				}
				T += " | ";
				if (flag == 0) break;
			}
			if (flag && T != "| ")//如果验证通过，就输出本条元组 
			{
				cout << T.c_str() << endl;
				Print_Row(leng, Attr_N);
				k++;
			}
		}
	}
	//打印有多少行被选中
	if (k == 0)
		cout << "empty set" << endl;
	else if (k == 1)
		cout << k << " row in set" << endl;
	else if (k > 1)
		cout << k << " rows in set" << endl;
}
//执行insert
void RecordManager::Insert(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], string value[])
{
	int length = cal(Attr_Type, Attr_N);//每行长度，根据属性固定

	int num = FILE_SIZE / length;
	int index = N / num;
	//如果最后一页满了，就换新的一页 
	if (N == (index+1) * num) index++;
	char * p = bm.readfromfile(Table_Name, index);
	int i = 0;
	int offset = (N - index * num) * length;
	union F f;
	union I in;
	while (i < Attr_N)
	{
		char * newp = NULL;
		string news = value[i];
		switch (Attr_Type[i])
		{
		case 1:
			in.a = atoi(value[i].c_str());
			cout << in.a << " ";
			memcpy(p + offset, in.byte, 4);
			offset += 4;
			break;
		case 2:
			f.f = (float)atof(value[i].c_str());
			cout << f.f << " ";
			memcpy(p + offset, f.byte, 4);
			offset += 4;
			break;
		default:
			if (Attr_Type[i] < 0)
			{
				while (news.length() < -Attr_Type[i])
					news += " ";
				newp = (char *)news.c_str();
				cout << newp << " ";
				memcpy(p + offset, newp, -Attr_Type[i]);
				offset -= Attr_Type[i];
			}
			break;
		}
		i++;
	}
	memcpy(p + offset, "0", 1);//末尾补0
	/*cout << index << " " << N << " " << atoi(To_String(p, (N - index * num) * length, (N - index * num) * length + 4).c_str()) << endl;*/
	cout << index << " " << (N + 1 - index * num)*length << endl;
	if (!bm.writetofile(Table_Name, index, 0, p, (N + 1 - index*num)*length))//如果回写失败 
		cout << "Insert fail!!" << endl;
	else cout << "Insert success!" << endl;
}
//执行没有where的delete
void RecordManager::Delete_All(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], string Index[])
{
	int length = cal(Attr_Type, Attr_N);//每行长度，根据属性固定

	int num = FILE_SIZE / length;
	int index = N / num;
	int l = 0, j = 0, k = 0;

	int flag = 1;

	for (l = 0; l <= index; l++)
	{
		char * p = bm.readfromfile(Table_Name, index);
		for (j = 0; j < (l == index ? N - index * num : num); j++)
		{
			if (*(p + (j + 1)*length - 1) == '1') continue;
			*(p + (j + 1)*length - 1) = '1';
			k++;
		}
		if (!bm.writetofile(Table_Name, index, 0, p, (N - index * num)*length))
		{
			flag = 0;
			break;
		}
	}
	if (flag) cout << k << " tuples deleted" << endl;
	else cout << "Delete fail!!" << endl;
}
//执行有where的delete
void RecordManager::Delete(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], int op_N, string Attr[], int op[], string Num[], string Index[])
{
	int length = cal(Attr_Type, Attr_N);//每行长度，根据属性固定

	string T;
	string temp;

	int num = FILE_SIZE / length;
	int index = N / num;
	int l = 0;
	int i = 0, j = 0;
	int k = 0;
	union F f;
	union I in;
	char* a = new char(4);

	int tmp_offset;
	int flag = 1;
	int _flag = 1;

	for (l = 0; l <= index; l++)
	{
		char * p = bm.readfromfile(Table_Name, index);
		for (j = 0; j < (l == index ? N - index * num : num); j++)
		{
			if (*(p + (j + 1)*length - 1) == '1') continue;
			int tmp_offset = 0;
			for (i = 0; i < Attr_N; i++)
			{
				switch (Attr_Type[i]) {
				case 1:
					temp = "";
					memcpy(in.byte, p + j * length + tmp_offset, 4);
					itoa(in.a, a, 10);
					temp += a;
					if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
					if (flag == 0) break;
					tmp_offset += 4;
					break;
				case 2:
					temp = "";
					memcpy(f.byte, p + j * length + tmp_offset, 4);
					_gcvt((double)f.f, 4, a);
					temp += a;
					if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
					if (flag == 0) break;
					tmp_offset += 4;
					break;
				default:
					if (Attr_Type[i] < 0)
					{
						temp = To_String(p, j*length + tmp_offset, j*length + tmp_offset - Attr_Type[i]);
						if ((flag = verify(temp, Attr, op, Num, op_N, Attr_Type, Attr_Name, i)) == 0) break;
						if (flag == 0) break;
						tmp_offset -= Attr_Type[i];
					}
					break;
				}
				if (flag == 0) break;
			}
			if (flag)
			{
				*(p + (j + 1)*length - 1) = '1';
				k++;
			}
		}
		if (!bm.writetofile(Table_Name, index, 0, p, (N - index * num)*length))
		{
			_flag = 0;
			break;
		}
	}
	if (_flag) cout << k << " tuples deleted" << endl;
	else cout << "Delete fail!!" << endl;
}
//获取某表某属性的全部值
Pair* RecordManager::Get_Attr(int N, string Table_Name, string Attr, int Attr_N, int Attr_Type[], string Attr_Name[])
{
	Pair *P = new Pair[N + 1];
	//测试在select
	int i = 0;
	int offset = 0;
	int leng = 0;
	union F f;
	union I in;
	int attr = 0;
	char* a = new char(4);
	for (i = 0; i < Attr_N; i++)
	{
		if (Attr_Name[i] == Attr)
		{
			switch (Attr_Type[i]) {
			case 1:
				leng = 4;
				break;
			case 2:
				leng = 4;
				break;
			default:
				if (Attr_Type[i] < 0)
					leng = -Attr_Type[i];
				break;
			}
			attr = i;
			break;
		}
		switch (Attr_Type[i]) {
		case 1:
			offset += 4;
			break;
		case 2:
			offset += 4;
			break;
		default:
			if (Attr_Type[i] < 0)
				offset -= Attr_Type[i];
			break;
		}
	}
	int length = cal(Attr_Type, Attr_N);//每行长度，根据属性固定
	int num = FILE_SIZE / length;
	int index = N / num;
	int l = 0, k = 0;
	for (l = 0; l <= index; l++)
	{
		char * p = bm.readfromfile(Table_Name, l);
		for (i = 0; i < (l == index ? N - num * index : num); i++)//循环次数因是否为最后一页而变
		{
			if (*(p + (i + 1)*length - 1) == '1') continue;
			P[k].n = i + num * l;
			switch (Attr_Type[attr])
			{
			case 1:
				P[k].value = "";
				memcpy(in.byte, p + i * length + offset, leng);
				itoa(in.a, a, 10);
				P[k].value += a;
				break;
			case 2:
				P[k].value = "";
				memcpy(f.byte, p + i * length + offset, leng);
				_gcvt((double)f.f, 6, a);
				P[k].value += a;
				break;
			default:
				if (Attr_Type[attr] < 0)
					P[k].value = To_String(p, i*length + offset, i*length + offset + leng);
			}
			int len = P[k].value.length() - 1;
			while (P[k].value.at(len) == ' ')
				len--;
			P[k].value = P[k].value.substr(0, len + 1);
			k++;
		}
	}
	P[k].n = -1;
	return P;
}
//验证某属性是否已经存在在表中
int RecordManager::ExistValue(int N, string Table_Name, int Attr_N, int Attr_Type[], string Attr_Name[], string value[], int WhichOne)
{
	//测试在select
	Pair *P = new Pair[N + 1];
	P = Get_Attr(N, Table_Name, Attr_Name[WhichOne], Attr_N, Attr_Type, Attr_Name);
	int i = 0;
	for (i = 0; P[i].n != -1; i++)
		if (P[i].value == value[WhichOne]) return 1;
	return 0;
}