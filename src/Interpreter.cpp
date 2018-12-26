#include "Interpreter.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û�����
string read_input()
{
	string SQL;
	string temp;
	char str[100];
	bool finish = false;
	while (!finish)
	{
		cin >> str;
		temp = str;
		SQL = SQL + " " + temp;
		if (SQL.at(SQL.length() - 1) == ';')
		{
			SQL.replace(SQL.length() - 1, 1, " ");
			SQL += ";";
			finish = true;
		}
	}
	//�������дת��ΪСд
	//SQL.MakeLower();
	//�����û�����
	return SQL;
}

string clear(string s)
{
	int len = s.length();
	while (s.at(0) == ' ') len--;
	s = s.substr(s.length() - len, len);
	while (s.at(s.length() - 1) == ' ') len--;
	s = s.substr(0, len+1);
	return s;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create database����Ƿ���Ч
string create_database(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error12:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�������
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string T, C;
	temp += " ";
	//���������
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	sql += T + " ";
	//�����������
	while (temp.at(start) == ' ')
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (T.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//��Ϊint 
	else if (T == "int")
		sql += "+";
	//��Ϊfloat
	else if (T == "float")
		sql += "-";
	//����
	else
	{
		index = T.find('(');
		C = T.substr(0, index);
		index++;
		//�����󣬴�ӡ������Ϣ
		if (C.empty())
		{
			cout << "error: " << T.c_str() << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//��Ϊchar
		else if (C == "char")
		{
			C = T.substr(index, T.length() - index - 1);
			if (C.empty())
			{
				cout << "error: the length of the data type char has not been specified!" << endl;
				sql = "99";
				return sql;
			}
			else
				sql += C;
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << C.c_str() << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//�Ƿ��и�����Ϣ
	while (start<temp.length() && temp.at(start) == ' ')
		start++;
	if (start<temp.length())
	{
		//��Ϊunique���壬������Ϣ
		end = temp.find(' ', start);
		T = temp.substr(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		else
		{
			cout << "error: " << temp.c_str() << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//���޸�����Ϣ
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create table����Ƿ���Ч
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int index, end, length;
	//��ȡ����
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	if ((end = SQL.find('(', start)) == -1)
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (!temp.empty())
	{
		while (SQL.at(start) == ' ')
			start++;
		length = temp.length() - 1;
		while (temp.at(length) == ' ')
			length--;
		temp = temp.substr(0, length + 1);
	}
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
	else if (temp.find(' ') != -1)
	{
		cout << "error: " << temp.c_str() << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		//��ȡÿ������
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���п����ԣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL.at(start) == ' ')
				start++;
		}
		//����������
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp.at(length) != ')'&&length >= 0)
			length--;
		//���ޣ���ӡ������Ϣ
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//�洢����
		else
		{
			temp = temp.substr(0, length);
			end = SQL.find(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//��Ϊ��������
			if (T == "primary")
			{
				//�ж��Ƿ��йؼ���key
				temp += ")";
				while (SQL.at(start) == ' ')
					start++;
				end = SQL.find('(', start);
				T = SQL.substr(start, end - start);
				start = end + 1;
				length = T.length() - 1;
				while (T.at(length) == ' ')
					length--;
				T = T.substr(0, length + 1);
				//��Ϊ�գ���ӡ������Ϣ
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//���У�������֤
				else if (T == "key")
				{
					//��ȡ����������
					while (SQL.at(start) == ' ')
						start++;
					end = SQL.find(')', start);
					T = SQL.substr(start, end - start);
					length = T.length() - 1;
					while (T.at(length) == ' ')
						length--;
					T = T.substr(0, length + 1);
					//���ޣ���ӡ������Ϣ
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if (T.find(' ') != -1)
					{
						cout << "error : " << T.c_str() << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//��������
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
				else
				{
					cout << "error : " << T.c_str() << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//��Ϊһ������
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
				{
					SQL = "99";
					return SQL;
				}
				else
					SQL = "01" + sql;
			}
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index on����Ƿ���Ч
string create_index_on(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	//��ȡ����
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find('(', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//�����Ƿ�Ϊ��Ч�ļ���
		length = temp.length() - 1;
		while (temp.at(length) == ' ')
			length--;
		temp = temp.substr(0, length + 1);
		//��Ч
		if (temp.find(' ') == -1)
		{
			sql += " " + temp;
			//��ȡ������
			while (SQL.at(start) == ' ')
				start++;
			end = SQL.find(')', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//�����Ƿ�Ϊ��Ч������
				length = temp.length() - 1;
				while (temp.at(length) == ' ')
					length--;
				temp = temp.substr(0, length + 1);
				//��Ч
				if (temp.find(' ') == -1)
				{
					sql += " " + temp;
					while (SQL.at(start) == ' ')
						start++;
					if (SQL.at(start) != ';' || start != SQL.length() - 1)
					{
						cout << "syntax error: syntax error for quit!" << endl;
						SQL = "99";
						return SQL;
					}
					//����create index�����ڲ���ʽ
					else
						SQL = "02" + sql;
				}
				//��Ч,��ӡ������Ϣ
				else
				{
					cout << "error:" << " " << temp.c_str() << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//��Ч,��ӡ������Ϣ
		else
		{
			cout << "error:" << " " << temp.c_str() << "---is not a valid table name!" << endl;
			SQL = "99";
			return SQL;
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create index����Ƿ���Ч
string create_index(string SQL, int start)
{
	string temp, sql;
	int end;
	//��ȡ����������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//��ȡ���ĸ�����
		while (SQL.at(start) == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//��Ϊon,������֤
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
		else
		{
			cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤create����Ƿ���Ч
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop database����Ƿ���Ч
string drop_database(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//����drop database�����ڲ���ʽ
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop table����Ƿ���Ч
string drop_table(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//����drop table�����ڲ���ʽ
		else
			SQL = "11" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop index����Ƿ���Ч
string drop_index(string SQL, int start)
{
	string temp;
	int index, end;
	//��ȡ����������
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (temp.empty())
	{
		cout << "error: index name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//����drop index�����ڲ���ʽ
		else
			SQL = "12" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤drop����Ƿ���Ч
string drop_clause(string SQL, int start)
{
	string temp;
	int end;
	//��ȡ�ڶ�������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//��Ϊdatabase,������֤
	else if (temp == "database")
		SQL = drop_database(SQL, start);
	//��Ϊtable,������֤
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//��Ϊindex,������֤
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����create�����ڲ���ʽ
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�����������ļ����ÿһ��
string get_part(string temp, string sql, string kind)
{
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//�����ʽת��Ϊ�ڲ���ʽ
string get_expression(string temp, string sql)
{
	int start = 0, end;
	string T;
	//��ȡ������
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (T.empty())
	{
		cout << "error: error in where statement!" << endl;
		sql = "99";
		return sql;
	}
	sql += T + " ";
	//��ȡopration
	while (temp.at(start) == ' ')
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//��operation������
	if (T.empty() && (T == "<" || T == "<>" || T == ">" || T == "=" || T == ">=" || T == "<="))
	{
		cout << "error: " << T.c_str() << "is not a valid operation!" << endl;
		sql = "99";
		return sql;
	}
	sql += T + " ";
	//��ȡֵ
	while (temp.at(start) == ' ')
		start++;
	T = temp.substr(start, temp.length() - start);
	sql += T;

	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ���ʽ���ÿ�����ʽ
string get_each(string T, string sql, string condition)
{
	int start = 0, end, length;
	string temp;
	//��ȡ���ʽ��ķ־�
	while ((end = T.find(condition, start)) != -1)
	{
		temp = T.substr(start, end - start - 1);
		start = end + condition.length();
		if (!temp.empty())
		{
			while (T.at(start) == ' ')
				start++;
			length = temp.length() - 1;
			while (temp.at(length) == ' ')
				length--;
			temp = temp.substr(0, length + 1);
		}
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "error: error in where statement!" << endl;
			sql = "99";
			return sql;
		}
		else
		{
			sql = get_expression(temp, sql);
			if (sql == "99") return sql;
		}
		sql += ",";
	}
	//��ȡ���ı��ʽ
	temp = T.substr(start, T.length() - start);
	length = temp.length() - 1;
	while (temp.at(length) == ' ')
		length--;
	temp = temp.substr(0, length + 1);
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: error in where statement!" << endl;
		sql = "99";
		return sql;
	}
	else
	{
		sql = get_expression(temp, sql);
		if (sql == "99") return sql;
		else sql += ",";
	}
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��֤select����Ƿ���Ч
string select_clause(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//��ȡ������
	while ((end = SQL.find(',', start)) != -1)
	{
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���п����ԣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "error: error in select statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			sql += temp;
			sql += "-";
		}
		while (SQL.at(start) == ' ')
			start++;
	}
	//����������
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	if (temp.empty())
	{
		cout << "error: error in select statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql += temp;
		sql += ",";
		while (SQL.at(start) == ' ')
			start++;
		//���from
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//���ޣ���ӡ������Ϣ
		if (temp.empty())
		{
			cout << "error: error in select statement!" << endl;
			SQL = "99";
			return SQL;
		}
		else
		{
			if (temp != "from")
			{
				cout << "error: there is no from in select statement" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				while (SQL.at(start) == ' ')
					start++;
				//û��where�����
				if ((end = SQL.find("where", start)) == -1)
				{
					end = SQL.find(';', start);
					temp = SQL.substr(start, end - start - 1);
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					if (temp.find(' ') != -1)
					{
						cout << "error: " << temp.c_str() << "---is not a valid table name" << endl;
						SQL = "99";
						return SQL;
					}
					else
					{
						sql += temp;
						SQL = "20" + sql;
					}
				}
				//��where�����
				else
				{
					//��ȡ����
					temp = SQL.substr(start, end - start - 1);
					length = temp.length() - 1;
					while (temp.at(length) == ' ')
						length--;
					temp = temp.substr(0, length + 1);
					start = end + 5;
					//���������Ϊ��
					if (!temp.empty())
					{
						while (SQL.at(start) == ' ')
							start++;
						length = temp.length() - 1;
						while (temp.at(length) == ' ')
							length--;
						temp = temp.substr(0, length + 1);
					}
					//���ޣ���ӡ������Ϣ
					if (temp.empty())
					{
						cout << "error: error in select statement!" << endl;
						SQL = "99";
						return SQL;
					}
					//��Ϊ�Ƿ���Ϣ����ӡ������Ϣ
					else if (temp.find(' ') != -1)
					{
						cout << "error: " << temp.c_str() << "---is not a valid table name!" << endl;
						SQL = "99";
						return SQL;
					}
					else
					{
						sql += temp + ",";
						//��ȡ���ʽ
						end = SQL.find(';', start);
						temp = SQL.substr(start, end - start);
						//������ʽ�鲻Ϊ��
						if (!temp.empty())
						{
							length = temp.length() - 1;
							while (temp.at(length) == ' ')
								length--;
							temp = temp.substr(0, length + 1);
						}
						//���ޣ���ӡ������Ϣ
						if (temp.empty())
						{
							cout << "error: error in select statement!" << endl;
							SQL = "99";
							return SQL;
						}
						else
						{
							sql = get_each(temp, sql, "and");
							if (sql == "99") return sql;
							else SQL = "21" + sql;
						}
					}
				}
			}
		}
	}
	return SQL;
}

//��֤insert into values����Ƿ���Ч
string insert_into_values(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	while (SQL.at(start) == ' ')
		start++;
	if ((end = SQL.find('(', start)) == -1)
	{
		cout << "error: missing ( in the statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql += ",";
		start = end + 1;
		while (SQL.at(start) == ' ')
			start++;
		//��ȡ����
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//���п����ԣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "error: error in insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//��������
			else
			{
				length = temp.length() - 1;
				while (temp.at(length) == ' ')
					length--;
				temp = temp.substr(0, length + 1);
				if (temp.at(0) == '\'') temp = temp.substr(1, temp.length() - 1);
				if (temp.at(temp.length() - 1) == '\'') temp = temp.substr(0, temp.length() - 1);
				sql += temp + ",";
			}
			while (SQL.at(start) == ' ')
				start++;
		}
		//��ȡ�������
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp.at(length) != ')'&&length >= 0)
			length--;
		//���ޣ���ӡ������Ϣ
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//�洢����
		else
		{
			temp = temp.substr(0, length);
			//temp = clear(temp);
			if (temp.at(0) == '\'') temp = temp.substr(1, temp.length() - 1);
			if (temp.at(temp.length()-1) == '\'') temp = temp.substr(0, temp.length() - 1);
			sql += temp + ",";
			sql = "30" + sql;
		}
	}
	return sql;
}

//��֤insert ����Ƿ���Ч
string insert_clause(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//��ȡ�ڶ�������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	//��Ϊinto,������֤
	else if (temp == "into")
	{
		//��ȡ����������
		while (SQL.at(start) == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		if (temp.empty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
		}
		else
		{
			sql = temp;
			//��ȡ���ĸ�����
			cout << 3;
			while (SQL.at(start) == ' ')
				start++;
			end = SQL.find('(', start);
			temp = SQL.substr(start, end - start);
			length = temp.length() - 1;
			while (temp.at(length) == ' ')
				length--;
			temp = temp.substr(0, length + 1);
			start = end;
			//���ޣ���ӡ������Ϣ
			if (temp.empty())
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
			}
			//��Ϊon,������֤
			else if (temp == "values")
				SQL = insert_into_values(SQL, start, sql);
			//��Ϊ�Ƿ���Ϣ����ӡ�Ƿ���Ϣ
			else
			{
				cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
				SQL = "99";
			}
		}
	}
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����insert�����ڲ���ʽ
	return SQL;
}

//��֤ delete from where ����Ƿ���Ч
string delete_from_where(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	sql += ",";
	while (SQL.at(start) == ' ')
		start++;
	//��ȡ���ʽ
	end = SQL.find(';', start);
	temp = SQL.substr(start, end - start - 1);
	//������ʽ�鲻Ϊ��
	if (!temp.empty())
	{
		length = temp.length() - 1;
		while (temp.at(length) == ' ')
			length--;
		temp = temp.substr(0, length + 1);
	}
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "error: error in delete statement!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = get_each(temp, sql, "and");
		if (sql == "99") return sql;
	}
	return sql;
}

//��֤delete����Ƿ���Ч
string delete_clause(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//��ȡ�ڶ�������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//���ޣ���ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	//��Ϊinto,������֤
	else if (temp == "from")
	{
		//��ȡ����������
		while (SQL.at(start) == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		if (temp.empty())
		{
			cout << "syntax error: syntax error for insert statement!" << endl;
			SQL = "99";
		}
		else
		{
			sql = temp;
			//��ȡ���ĸ�����
			while (SQL.at(start) == ' ')
				start++;
			//��û��where����ӡ������Ϣ
			if ((end = SQL.find("where", start)) == -1)
				SQL = "40" + sql;
			//����,������֤
			else
			{
				start = end + 5;
				SQL = delete_from_where(SQL, start, sql);
				if (SQL == "99") return SQL;
				else SQL = "41" + SQL;
			}
		}
	}
	//��Ϊ������Ϣ����ӡ������Ϣ
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//����delete�����ڲ���ʽ
	return SQL;
}

//��֤use����Ƿ���Ч
string use_clause(string SQL, int start)
{
	return SQL;
}

//��֤execfile����Ƿ���Ч
string execfile_clause(string SQL, int start)
{
	string sql = "";
	string tmp, T;
	string t;
	string file_name = SQL.substr(start, SQL.length() - start - 2);
	fstream ofs(file_name + ".txt", ios::in | ios::out);
	if (!ofs)
	{
		cout << "fail to open file" << endl;
		return "99";
	}
	while (!ofs.eof())
	{
		int _start = 0, end;
		getline(ofs, t);
		if (t == "") continue;
		tmp = " " + t;
		if (tmp.at(tmp.length() - 1) == ';')
		{
			tmp.replace(tmp.length() - 1, 1, " ");
			tmp += ";";
		}
		while (tmp.at(_start) == ' ')
			_start++;
		end = tmp.find(' ', _start);
		T = tmp.substr(_start, end - _start);
		_start = end + 1;
		//�������룬��ӡ������Ϣ
		if (T.empty())
		{
			cout << "syntax error: empty statement!" << endl;
			sql = "99";
			return sql;
		}
		//��Ϊcreate���
		else if (T == "create")
			tmp = create_clause(tmp, _start);
		//��Ϊdrop���
		else if (T == "drop")
			tmp = drop_clause(tmp, _start);
		//��Ϊselect���
		else if (T == "select")
			tmp = select_clause(tmp, _start);
		//��Ϊinsert���
		else if (T == "insert")
			tmp = insert_clause(tmp, _start);
		//��Ϊdelete���
		else if (T == "delete")
			tmp = delete_clause(tmp, _start);
		//��Ϊuse���
		else if (T == "use")
			tmp = use_clause(tmp, _start);
		//��Ϊexecfile���
		else if (T == "execfile")
			tmp = execfile_clause(tmp, _start);
		//��Ϊquit���
		else if (T == "quit")
			tmp = quit_clause(tmp, _start);
		//��ȡ����
		else if (T == "help")
			tmp = "80";
		//��Ϊ�Ƿ����
		else
		{
			cout << "syntax error:" << " " << T.c_str() << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
		sql += tmp + "|";
	}
	return sql.substr(0, sql.length()-1);
}

//��֤quit����Ƿ���Ч
string quit_clause(string SQL, int start)
{
	SQL = "70";
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//��ȡ�û����룬������������Ч�Լ�飬����ȷ�����������ڲ���ʾ��ʽ
string Interpreter(string statement)
{
	string SQL;
	string temp;
	string sql;
	int start = 0, end;
	if (statement.empty())
		//��ȡ�û�����
		SQL = read_input();
	else
		SQL = statement;
	//��ȡ����ĵ�һ������
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//�������룬��ӡ������Ϣ
	if (temp.empty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//��Ϊcreate���
	else if (temp == "create")
		SQL = create_clause(SQL, start);
	//��Ϊdrop���
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);
	//��Ϊselect���
	else if (temp == "select")
		SQL = select_clause(SQL, start);
	//��Ϊinsert���
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//��Ϊdelete���
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//��Ϊuse���
	else if (temp == "use")
		SQL = use_clause(SQL, start);
	//��Ϊexecfile���
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//��Ϊquit���
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//��ȡ����
	else if (temp == "help")
		SQL = "80";
	//��Ϊ�Ƿ����
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//�������������ڲ���ʽ
	return SQL;
}
