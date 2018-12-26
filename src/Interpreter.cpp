#include "Interpreter.h"

/////////////////////////////////////////////////////////////////////////////////////////////
//读取用户输入
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
	//将输入大写转化为小写
	//SQL.MakeLower();
	//返回用户输入
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
//验证create database语句是否有效
string create_database(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error12:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//返回drop database语句的内部形式
		else
			SQL = "00" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获得属性
string get_attribute(string temp, string sql)
{
	int start = 0, end, index;
	string T, C;
	temp += " ";
	//获得属性名
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	sql += T + " ";
	//获得数据类型
	while (temp.at(start) == ' ')
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (T.empty())
	{
		cout << "error : error in create table statement!" << endl;
		sql = "99";
		return sql;
	}
	//若为int 
	else if (T == "int")
		sql += "+";
	//若为float
	else if (T == "float")
		sql += "-";
	//其他
	else
	{
		index = T.find('(');
		C = T.substr(0, index);
		index++;
		//若有误，打印出错信息
		if (C.empty())
		{
			cout << "error: " << T.c_str() << "---is not a valid data type definition!" << endl;
			sql = "99";
			return sql;
		}
		//若为char
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
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << C.c_str() << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//是否有附加信息
	while (start<temp.length() && temp.at(start) == ' ')
		start++;
	if (start<temp.length())
	{
		//若为unique定义，保存信息
		end = temp.find(' ', start);
		T = temp.substr(start, end - start);
		if (T == "unique")
		{
			sql += " 1,";
		}
		//若为非法信息，打印出错信息
		else
		{
			cout << "error: " << temp.c_str() << "---is not a valid key word!" << endl;
			sql = "99";
			return sql;
		}
	}
	//若无附加信息
	else
		sql += " 0,";
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create table语句是否有效
string create_table(string SQL, int start)
{
	string temp, sql, T;
	int index, end, length;
	//获取表名
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
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: error in create table statement!" << endl;
		SQL = "99";
		return SQL;
	}
	//若为非法信息，打印出错信息
	else if (temp.find(' ') != -1)
	{
		cout << "error: " << temp.c_str() << "---is not a valid table name!" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		sql = temp + ",";
		//获取每个属性
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若有空属性，打印出错信息
			if (temp.empty())
			{
				cout << "error: error in create table statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//保存属性
			else
			{
				sql = get_attribute(temp, sql);
				if (sql == "99")
					return sql;
			}
			while (SQL.at(start) == ' ')
				start++;
		}
		//获得最后属性
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp.at(length) != ')'&&length >= 0)
			length--;
		//若无，打印出错信息
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//存储属性
		else
		{
			temp = temp.substr(0, length);
			end = SQL.find(' ', start);
			T = SQL.substr(start, end - start);
			start = end + 1;
			//若为主键定义
			if (T == "primary")
			{
				//判断是否有关键字key
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
				//若为空，打印出错信息
				if (T.empty())
				{
					cout << "syntax error: syntax error in create table statement!" << endl;
					cout << "\t missing key word key!" << endl;
					SQL = "99";
					return SQL;
				}
				//若有，继续验证
				else if (T == "key")
				{
					//获取主键属性名
					while (SQL.at(start) == ' ')
						start++;
					end = SQL.find(')', start);
					T = SQL.substr(start, end - start);
					length = T.length() - 1;
					while (T.at(length) == ' ')
						length--;
					T = T.substr(0, length + 1);
					//若无，打印出错信息
					if (T.empty())
					{
						cout << "error : missing primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//若为非法信息，打印出错信息
					else if (T.find(' ') != -1)
					{
						cout << "error : " << T.c_str() << "---is not a valid primary key attribute name!" << endl;
						SQL = "99";
						return SQL;
					}
					//保存主键
					else
					{
						sql += T + " #,";
						SQL = "01" + sql;
					}
				}
				//若为非法信息，打印出错信息
				else
				{
					cout << "error : " << T.c_str() << "---is not a valid key word!" << endl;
					SQL = "99";
					return SQL;
				}
			}
			//若为一般属性
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
//验证create index on语句是否有效
string create_index_on(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	//获取表名
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find('(', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		cout << "\t missing ( !" << endl;
		SQL = "99";
		return SQL;
	}
	else
	{
		//检验是否为有效文件名
		length = temp.length() - 1;
		while (temp.at(length) == ' ')
			length--;
		temp = temp.substr(0, length + 1);
		//有效
		if (temp.find(' ') == -1)
		{
			sql += " " + temp;
			//获取属性名
			while (SQL.at(start) == ' ')
				start++;
			end = SQL.find(')', start);
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若无，打印出错信息
			if (temp.empty())
			{
				cout << "syntax error: syntax error for create index statement!" << endl;
				cout << "\t missing ) !" << endl;
				SQL = "99";
				return SQL;
			}
			else
			{
				//检验是否为有效属性名
				length = temp.length() - 1;
				while (temp.at(length) == ' ')
					length--;
				temp = temp.substr(0, length + 1);
				//有效
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
					//返回create index语句的内部形式
					else
						SQL = "02" + sql;
				}
				//无效,打印出错信息
				else
				{
					cout << "error:" << " " << temp.c_str() << "---is not a valid attribute name!" << endl;
					SQL = "99";
					return SQL;
				}
			}
		}
		//无效,打印出错信息
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
//验证create index语句是否有效
string create_index(string SQL, int start)
{
	string temp, sql;
	int end;
	//获取第三个单词
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create index statement!" << endl;
		SQL = "99";
	}
	else
	{
		sql = temp;
		//获取第四个单词
		while (SQL.at(start) == ' ')
			start++;
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若无，打印出错信息
		if (temp.empty())
		{
			cout << "syntax error: syntax error for create index statement!" << endl;
			SQL = "99";
		}
		//若为on,继续验证
		else if (temp == "on")
			SQL = create_index_on(SQL, start, sql);
		//若为非法信息，打印非法信息
		else
		{
			cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
			SQL = "99";
		}
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证create语句是否有效
string create_clause(string SQL, int start)
{
	string temp;
	int end;
	//获取第二个单词
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for create statement!" << endl;
		SQL = "99";
	}
	//若为database,继续验证
	else if (temp == "database")
		SQL = create_database(SQL, start);
	//若为table,继续验证
	else if (temp == "table")
		SQL = create_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = create_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回create语句的内部形式
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop database语句是否有效
string drop_database(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: database name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid database name!" << endl;
			SQL = "99";
		}
		//返回drop database语句的内部形式
		else
			SQL = "10" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop table语句是否有效
string drop_table(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
	while (SQL.at(start) == ' ')
		start++;
	index = start;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "error: table name has not been specified!" << endl;
		SQL = "99";
	}
	else
	{
		while (SQL.at(start) == ' ')
			start++;
		//若为非法信息，打印出错信息
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid table name!" << endl;
			SQL = "99";
		}
		//返回drop table语句的内部形式
		else
			SQL = "11" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop index语句是否有效
string drop_index(string SQL, int start)
{
	string temp;
	int index, end;
	//获取第三个单词
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
		//若为非法信息，打印出错信息
		if (SQL.at(start) != ';' || start != SQL.length() - 1)
		{
			cout << "error:" << SQL.substr(index, SQL.length() - index - 2).c_str() << "---is not a valid index name!" << endl;
			SQL = "99";
		}
		//返回drop index语句的内部形式
		else
			SQL = "12" + temp;
	}
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//验证drop语句是否有效
string drop_clause(string SQL, int start)
{
	string temp;
	int end;
	//获取第二个单词
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for drop statement!" << endl;
		SQL = "99";
	}
	//若为database,继续验证
	else if (temp == "database")
		SQL = drop_database(SQL, start);
	//若为table,继续验证
	else if (temp == "table")
		SQL = drop_table(SQL, start);
	//若为index,继续验证
	else if (temp == "index")
		SQL = drop_index(SQL, start);
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回create语句的内部形式
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获得属性组或文件组的每一项
string get_part(string temp, string sql, string kind)
{
	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//将表达式转化为内部形式
string get_expression(string temp, string sql)
{
	int start = 0, end;
	string T;
	//获取属性名
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (T.empty())
	{
		cout << "error: error in where statement!" << endl;
		sql = "99";
		return sql;
	}
	sql += T + " ";
	//获取opration
	while (temp.at(start) == ' ')
		start++;
	end = temp.find(' ', start);
	T = temp.substr(start, end - start);
	start = end + 1;
	//若operation出问题
	if (T.empty() && (T == "<" || T == "<>" || T == ">" || T == "=" || T == ">=" || T == "<="))
	{
		cout << "error: " << T.c_str() << "is not a valid operation!" << endl;
		sql = "99";
		return sql;
	}
	sql += T + " ";
	//获取值
	while (temp.at(start) == ' ')
		start++;
	T = temp.substr(start, temp.length() - start);
	sql += T;

	return sql;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获取表达式组的每个表达式
string get_each(string T, string sql, string condition)
{
	int start = 0, end, length;
	string temp;
	//获取表达式组的分句
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
		//若无，打印出错信息
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
	//获取最后的表达式
	temp = T.substr(start, T.length() - start);
	length = temp.length() - 1;
	while (temp.at(length) == ' ')
		length--;
	temp = temp.substr(0, length + 1);
	//若无，打印出错信息
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
//验证select语句是否有效
string select_clause(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//获取属性组
	while ((end = SQL.find(',', start)) != -1)
	{
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若有空属性，打印出错信息
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
	//获得最后属性
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
		//检查from
		end = SQL.find(' ', start);
		temp = SQL.substr(start, end - start);
		start = end + 1;
		//若无，打印出错信息
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
				//没有where的情况
				if ((end = SQL.find("where", start)) == -1)
				{
					end = SQL.find(';', start);
					temp = SQL.substr(start, end - start - 1);
					//若为非法信息，打印出错信息
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
				//有where的情况
				else
				{
					//获取表名
					temp = SQL.substr(start, end - start - 1);
					length = temp.length() - 1;
					while (temp.at(length) == ' ')
						length--;
					temp = temp.substr(0, length + 1);
					start = end + 5;
					//如果表名不为空
					if (!temp.empty())
					{
						while (SQL.at(start) == ' ')
							start++;
						length = temp.length() - 1;
						while (temp.at(length) == ' ')
							length--;
						temp = temp.substr(0, length + 1);
					}
					//若无，打印出错信息
					if (temp.empty())
					{
						cout << "error: error in select statement!" << endl;
						SQL = "99";
						return SQL;
					}
					//若为非法信息，打印出错信息
					else if (temp.find(' ') != -1)
					{
						cout << "error: " << temp.c_str() << "---is not a valid table name!" << endl;
						SQL = "99";
						return SQL;
					}
					else
					{
						sql += temp + ",";
						//获取表达式
						end = SQL.find(';', start);
						temp = SQL.substr(start, end - start);
						//如果表达式组不为空
						if (!temp.empty())
						{
							length = temp.length() - 1;
							while (temp.at(length) == ' ')
								length--;
							temp = temp.substr(0, length + 1);
						}
						//若无，打印出错信息
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

//验证insert into values语句是否有效
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
		//获取属性
		while ((end = SQL.find(',', start)) != -1)
		{
			temp = SQL.substr(start, end - start);
			start = end + 1;
			//若有空属性，打印出错信息
			if (temp.empty())
			{
				cout << "error: error in insert statement!" << endl;
				SQL = "99";
				return SQL;
			}
			//保存属性
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
		//获取最后属性
		temp = SQL.substr(start, SQL.length() - start - 1);
		length = temp.length() - 1;
		while (temp.at(length) != ')'&&length >= 0)
			length--;
		//若无，打印出错信息
		if (length<1)
		{
			cout << "error: error in create table statement!" << endl;
			SQL = "99";
			return SQL;
		}
		//存储属性
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

//验证insert 语句是否有效
string insert_clause(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//获取第二个单词
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	//若为into,继续验证
	else if (temp == "into")
	{
		//获取第三个单词
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
			//获取第四个单词
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
			//若无，打印出错信息
			if (temp.empty())
			{
				cout << "syntax error: syntax error for insert statement!" << endl;
				SQL = "99";
			}
			//若为on,继续验证
			else if (temp == "values")
				SQL = insert_into_values(SQL, start, sql);
			//若为非法信息，打印非法信息
			else
			{
				cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
				SQL = "99";
			}
		}
	}
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回insert语句的内部形式
	return SQL;
}

//验证 delete from where 语句是否有效
string delete_from_where(string SQL, int start, string sql)
{
	string temp;
	int end, length;
	sql += ",";
	while (SQL.at(start) == ' ')
		start++;
	//获取表达式
	end = SQL.find(';', start);
	temp = SQL.substr(start, end - start - 1);
	//如果表达式组不为空
	if (!temp.empty())
	{
		length = temp.length() - 1;
		while (temp.at(length) == ' ')
			length--;
		temp = temp.substr(0, length + 1);
	}
	//若无，打印出错信息
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

//验证delete语句是否有效
string delete_clause(string SQL, int start)
{
	string temp, sql;
	int end, length;
	//获取第二个单词
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: syntax error for insert statement!" << endl;
		SQL = "99";
	}
	//若为into,继续验证
	else if (temp == "from")
	{
		//获取第三个单词
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
			//获取第四个单词
			while (SQL.at(start) == ' ')
				start++;
			//若没有where，打印出错信息
			if ((end = SQL.find("where", start)) == -1)
				SQL = "40" + sql;
			//若有,继续验证
			else
			{
				start = end + 5;
				SQL = delete_from_where(SQL, start, sql);
				if (SQL == "99") return SQL;
				else SQL = "41" + SQL;
			}
		}
	}
	//若为错误信息，打印出错信息
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回delete语句的内部形式
	return SQL;
}

//验证use语句是否有效
string use_clause(string SQL, int start)
{
	return SQL;
}

//验证execfile语句是否有效
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
		//若无输入，打印出错信息
		if (T.empty())
		{
			cout << "syntax error: empty statement!" << endl;
			sql = "99";
			return sql;
		}
		//若为create语句
		else if (T == "create")
			tmp = create_clause(tmp, _start);
		//若为drop语句
		else if (T == "drop")
			tmp = drop_clause(tmp, _start);
		//若为select语句
		else if (T == "select")
			tmp = select_clause(tmp, _start);
		//若为insert语句
		else if (T == "insert")
			tmp = insert_clause(tmp, _start);
		//若为delete语句
		else if (T == "delete")
			tmp = delete_clause(tmp, _start);
		//若为use语句
		else if (T == "use")
			tmp = use_clause(tmp, _start);
		//若为execfile语句
		else if (T == "execfile")
			tmp = execfile_clause(tmp, _start);
		//若为quit语句
		else if (T == "quit")
			tmp = quit_clause(tmp, _start);
		//获取帮助
		else if (T == "help")
			tmp = "80";
		//若为非法语句
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

//验证quit语句是否有效
string quit_clause(string SQL, int start)
{
	SQL = "70";
	return SQL;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//获取用户输入，并对输入作有效性检查，若正确，返回语句的内部表示形式
string Interpreter(string statement)
{
	string SQL;
	string temp;
	string sql;
	int start = 0, end;
	if (statement.empty())
		//获取用户输入
		SQL = read_input();
	else
		SQL = statement;
	//获取输入的第一个单词
	while (SQL.at(start) == ' ')
		start++;
	end = SQL.find(' ', start);
	temp = SQL.substr(start, end - start);
	start = end + 1;
	//若无输入，打印出错信息
	if (temp.empty())
	{
		cout << "syntax error: empty statement!" << endl;
		SQL = "99";
	}
	//若为create语句
	else if (temp == "create")
		SQL = create_clause(SQL, start);
	//若为drop语句
	else if (temp == "drop")
		SQL = drop_clause(SQL, start);
	//若为select语句
	else if (temp == "select")
		SQL = select_clause(SQL, start);
	//若为insert语句
	else if (temp == "insert")
		SQL = insert_clause(SQL, start);
	//若为delete语句
	else if (temp == "delete")
		SQL = delete_clause(SQL, start);
	//若为use语句
	else if (temp == "use")
		SQL = use_clause(SQL, start);
	//若为execfile语句
	else if (temp == "execfile")
		SQL = execfile_clause(SQL, start);
	//若为quit语句
	else if (temp == "quit")
		SQL = quit_clause(SQL, start);
	//获取帮助
	else if (temp == "help")
		SQL = "80";
	//若为非法语句
	else
	{
		cout << "syntax error:" << " " << temp.c_str() << "---is not a valid key word!" << endl;
		SQL = "99";
	}
	//返回输入语句的内部形式
	return SQL;
}
