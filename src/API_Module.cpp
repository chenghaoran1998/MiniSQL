#include "API_Module.h"
#include "Catalog_Manager.h"
#include "Record_Manager.h"
#include "Index_Manager.h"
using namespace std;

int API_Module(string SQL)
{
	int index;
	if (SQL == "99") return 1;
	else if (SQL.find('|') != string::npos)
	{
		while (SQL.length() != 0 && SQL.find('|') != string::npos)
		{
			string sql;
			index = SQL.find('|');
			sql = SQL.substr(0, index);
			SQL = SQL.substr(index + 1);
			if( Operate(sql) == 0) return 0;
		}
	}
	return Operate(SQL);
}

int Operate(string SQL)
{

	IndexManager IndexM;
	RecordManager RecordM;


	//处理SQL指令
	string Type;
	Type = SQL.substr(0, 2);
	SQL = SQL.substr(2);

	/////////////////////////////////////////////////////////////////////////////////////////
	//创建数据表
	if (Type == "01")
	{
		//SQL格式：01name,a + 0,b - 0,c 1 0,a #,

		//建Table
		int index = SQL.find(',');
		string Table_Name = SQL.substr(0, index);
		if (CataM.tableExists(Table_Name)) //检查表名是否存在
			cout << "error: There's already a table named" << Table_Name << "in the database." << endl;
		else
		{
			//建表
			CataM.Create_Table(Table_Name);	//调用Catalog M建空表
			SQL = SQL.substr(index + 1);
			//依次插入每一个表元素
			while (SQL.length() != 0)
			{
				//获取最前面的ATTR操作类型
				string ATTR;
				index = SQL.find(',');
				ATTR = SQL.substr(0, index);	//如 a,+,0 等等
				SQL = SQL.substr(index + 1);

				//解析每个建立表元素的命令  
				string Attr_Name, Attr_Type, Attr_Argu;
				index = ATTR.find(' ');
				Attr_Name = ATTR.substr(0, index);
				ATTR = ATTR.substr(index + 1);
				index = ATTR.find(' ');
				Attr_Type = ATTR.substr(0, index);
				Attr_Argu = ATTR.substr(index + 1);

				//执行属性建立命令
				//建立主键
				if (Attr_Type == "#")
				{
					string Index_Name = Table_Name + "_PrimaryKey_" + Attr_Name;

					CataM.setpk(Table_Name, Attr_Name);
					CataM.addIndex(Index_Name, Table_Name, Attr_Name);

					int 	i;
					int   	Table_Attr_Type[500];
					string 	Table_Attr_Name[500];
					attrbute pkATTR = CataM.get_attr_info(Table_Name, Attr_Name);
					Table    table = CataM.get_table_info(Table_Name);
					int Attr_N = CataM.attrCount(Table_Name);
					for (i = 0; i<Attr_N; i++)
					{
						Table_Attr_Type[i] = table.colum[i].type;
						Table_Attr_Name[i] = table.colum[i].name;
					}
					/*IndexM.Create_Index(Table_Name, "1", Attr_N);*/
				}
				// int类型 标入catalog类型为1
				else if (Attr_Type == "+")
				{
					if (CataM.attrExists(Table_Name, Attr_Name))
						cout << "There're too many attributes named" << Attr_Name << "in the table." << endl;
					else
					{
						if (Attr_Argu == "0") CataM.Create_Attr(Table_Name, Attr_Name, 1, false);
						else CataM.Create_Attr(Table_Name, Attr_Name, 1, true);
					}
				}
				// float类型 标入catalog类型为2
				else if (Attr_Type == "-")
				{
					if (CataM.attrExists(Table_Name, Attr_Name))
						cout << "There're too many attributes named" << Attr_Name << "in the table." << endl;
					else
					{
						if (Attr_Argu == "0") CataM.Create_Attr(Table_Name, Attr_Name, 2, false);
						else CataM.Create_Attr(Table_Name, Attr_Name, 2, true);
					}
				}
				// char(n)类型 标入catalog类型为-n
				else
				{
					if (CataM.attrExists(Table_Name, Attr_Name))
						cout << "There're too many attributes named" << Attr_Name << "in the table." << endl;
					else
					{
						stringstream ss;
						int num;
						ss << Attr_Type;
						ss >> num;
						num = num * (-1);
						if (Attr_Argu == "0") CataM.Create_Attr(Table_Name, Attr_Name, num, false);
						else CataM.Create_Attr(Table_Name, Attr_Name, num, true);
					}
				}


			}
		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//创建索引
	else if (Type == "02")
	{
		//SQL格式：02name table attr
		//获取Index_Name, Table_Name, Attr_Name;
		int index;
		string Index_Name, Table_Name, Attr_Name;
		index = SQL.find(' ');
		Index_Name = SQL.substr(0, index);
		SQL = SQL.substr(index + 1);
		index = SQL.find(' ');
		Table_Name = SQL.substr(0, index);
		Attr_Name = SQL.substr(index + 1);

		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else if (!CataM.attrExists(Table_Name, Attr_Name))
			cout << "error: There's NO attributes called " << Attr_Name << endl;
		else if (CataM.hasIndex(Table_Name, Attr_Name))
			cout << "error: There's already an Index on " << Attr_Name << endl;
		else if (CataM.indexExists(Index_Name))
			cout << "error: There's already an Index callded " << Index_Name << endl;
		else
		{
			attrbute attr = CataM.get_attr_info(Table_Name, Attr_Name);
			CataM.addIndex(Index_Name, Table_Name, Attr_Name);	//向CatalogM传入记录

			int 	i;
			int   	Table_Attr_Type[500];
			string 	Table_Attr_Name[500];
			attrbute idATTR = CataM.get_attr_info(Table_Name, Attr_Name);
			Table    table = CataM.get_table_info(Table_Name);
			int Attr_N = CataM.attrCount(Table_Name);
			for (i = 0; i<Attr_N; i++)
			{
				Table_Attr_Type[i] = table.colum[i].type;
				Table_Attr_Name[i] = table.colum[i].name;
			}

			int N = CataM.recordCount(Table_Name);
			//IndexM.Create_Index(Table_Name, "1", Attr_N);	//向IndexM传入建索引请求
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除数据表
	else if (Type == "11")
	{
		//SQL格式：11name

		string Table_Name = SQL.substr(0);
		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{
			//检索table中所有index信息，删去所有index
			int   i;
			Table table = CataM.get_table_info(Table_Name);
			int Attr_N = CataM.attrCount(Table_Name);
			for (i = 0; i<Attr_N; i++)
				if (CataM.hasIndex(Table_Name, table.colum[i].name))
				{
					string Index_Name = table.colum[i].index->indexname;
					//IndexM.Delete_Index(Table_Name, Index_Name);	//向Index
					CataM.deleteIndex(Index_Name);		//向Catalog
				}

			int   	Table_Attr_Type[500];
			string 	Table_Attr_Name[500];
			string	Attr_Index[500];
			for (i = 0; i<Attr_N; i++)
			{
				Table_Attr_Type[i] = table.colum[i].type;
				Table_Attr_Name[i] = table.colum[i].name;
				Attr_Index[i] = "";
			}
			int N = CataM.recordCount(Table_Name);

			//向Record发出删除数据请求
			RecordM.Delete_All(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, Attr_Index);
			//向Catalog发出删除表记录的请求
			CataM.Drop_Table(Table_Name);

		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除索引
	else if (Type == "12")
	{
		//SQL格式：12name

		string Index_Name = SQL.substr(0);
		if (!CataM.indexExists(Index_Name))
			cout << "error: There's NO index called " << Index_Name << endl;
		else
		{
			//向Catalog发出删除数据请求
			CataM.deleteIndex(Index_Name);
			//向Index发出删除表记录的请求
			/*IndexM.Delete_Index("a", Index_Name);*/
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//选择语句(无where子句 查询所有记录)
	else if (Type == "20")
	{
		//SQL格式：20*,table

		string Table_Name = SQL.substr(2);
		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{

			int 	i;
			int   	Table_Attr_Type[500];
			string 	Table_Attr_Name[500];
			Table   table = CataM.get_table_info(Table_Name);
			int		Attr_N = CataM.attrCount(Table_Name);
			for (i = 0; i<Attr_N; i++)
			{
				Table_Attr_Type[i] = table.colum[i].type;
				Table_Attr_Name[i] = table.colum[i].name;
			}
			int N = CataM.recordCount(Table_Name);
			RecordM.Select_All(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name);
		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//选择语句(有where子句)
	else if (Type == "21")
	{
		//SQL格式：21*,table,a = 10, b >= 20
		SQL = SQL.substr(2);
		int index;
		index = SQL.find(',');

		string Table_Name;
		Table_Name = SQL.substr(0, index);
		SQL = SQL.substr(index + 1);

		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{
			string ATTR;
			string Attr_Name[500];
			int op[500];
			string Num[500];
			string Index_Name[500];
			int count = 0;
			int FLAG = 0;
			int op_N;

			Table table = CataM.get_table_info(Table_Name);

			while (SQL.length() != 0)
			{
				index = SQL.find(',');
				ATTR = SQL.substr(0, index);
				SQL = SQL.substr(index + 1);

				//确定Attr_Name[count] , Num[count]
				string OP;
				index = ATTR.find(' ');
				Attr_Name[count] = ATTR.substr(0, index);
				ATTR = ATTR.substr(index + 1);
				index = ATTR.find(' ');
				OP = ATTR.substr(0, index);
				Num[count] = ATTR.substr(index + 1);

				int flag = 0;

				//确定op[count]
				if (OP == "=") op[count] = 0;
				else if (OP == "<>") op[count] = 1;
				else if (OP == "<") op[count] = 2;
				else if (OP == ">") op[count] = 3;
				else if (OP == "<=") op[count] = 4;
				else if (OP == ">=") op[count] = 5;
				else flag = 1;

				//确定索引Index_Name[count]
				if (!CataM.attrExists(Table_Name, Attr_Name[count]))
				{
					cout << "error: There's NO attributes called " << Attr_Name[count] << endl;
					FLAG = 1;
					break;
				}
				else if (flag == 1)
				{
					cout << "error: The operator is illegal." << endl;
					FLAG = 1;
					break;
				}
				else
				{
					Index_Name[count] = "";
					int i;
					int Attr_N = CataM.attrCount(Table_Name);
					for (i = 0; i<Attr_N; i++)
						if (table.colum[i].index != NULL && table.colum[i].index->columnName == Attr_Name[count])
						{
							Index_Name[count] = table.colum[i].index->indexname;
							break;
						}
				}

				count++;
			}

			if (FLAG == 0)
			{
				int 	i;
				int   	Table_Attr_Type[500];
				string 	Table_Attr_Name[500];
				Table   table = CataM.get_table_info(Table_Name);
				int Attr_N = CataM.attrCount(Table_Name);
				for (i = 0; i<Attr_N; i++)
				{
					Table_Attr_Type[i] = table.colum[i].type;
					Table_Attr_Name[i] = table.colum[i].name;
				}

				int N = CataM.recordCount(Table_Name);
				op_N = count;
				RecordM.Select(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, op_N, Attr_Name, op, Num, Index_Name);
			}
		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//向表中插入数据
	else if (Type == "30")
	{
		//30name,12345678,wy,22,m
		int index;
		int i;
		string Table_Name;
		index = SQL.find(',');
		Table_Name = SQL.substr(0, index);
		SQL = SQL.substr(index + 1);
		int flag = 0;

		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{
			int count = 0;
			string	value[500];
			Table	table = CataM.get_table_info(Table_Name);
			int		Attr_N = CataM.attrCount(Table_Name);
			int		N = CataM.recordCount(Table_Name);
			int   	Table_Attr_Type[500];
			string 	Table_Attr_Name[500];
			for (i = 0; i<Attr_N; i++)
			{
				Table_Attr_Type[i] = table.colum[i].type;
				Table_Attr_Name[i] = table.colum[i].name;
			}
			for (i = 0; i<Attr_N; i++)
			{
				int Attr_Type = table.colum[i].type;
				index = SQL.find(',');
				string Key = SQL.substr(0, index);
				SQL = SQL.substr(index + 1);

				if (Attr_Type == 1)	// int
				{
					stringstream ss1, ss2;
					int tmp1;
					string tmp2;
					ss1 << Key;
					ss1 >> tmp1;
					ss2 << tmp1;
					ss2 >> tmp2;
					if (Key != tmp2) flag = 1;
				}
				else if (Attr_Type == 2)
				{
					float tmp1;
					stringstream ss1;
					ss1 << Key;
					ss1 >> tmp1;
					if (tmp1 == 0 && Key != "0") flag = 1;
				}
				else
				{
					if (Key.length() > Attr_Type*(-1))
						flag = 1;
				}

				value[count] = Key;
				count++;
			}

			//验证unique信息
			for (i = 0; i < Attr_N; i++)
				if (table.colum[i].unique == true)
				{
					int Exist = RecordM.ExistValue(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, value, i);
					if (Exist == 1)
					{
						flag = 2;
						break;
					}
				}

			if (flag == 1) cout << "error: The data inputed is not suit for the attributes." << endl;
			else if (flag == 2) cout << "error: The data is not unique." << endl;
			else
			{
				//Catalog中增加记录
				CataM.Insert_Data(Table_Name);

				//Record插入数据
				RecordM.Insert(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, value);

				//Index中增加记录
				count = 0;
				for (i = 0; i<Attr_N; i++)
				{
					if (table.colum[i].index != NULL)
					{
						int Key1 = 0;
						float Key2 = 0;
						string Key3 = "";
						if (table.colum[i].type == 1)
						{
							stringstream ss;
							ss << value[count];
							ss >> Key1;
						}
						else if (table.colum[i].type == 2)
						{
							stringstream ss;
							ss << value[count];
							ss >> Key2;
						}
						else  Key3 = value[count];
						/*IndexM.Insert_Index(table.colum[i].index->indexname, "1", "1", Key2, 1);*/
					}
					count++;
				}

			}

		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除表内所有数据
	else if (Type == "40")
	{
		//SQL格式：40name
		string Table_Name = SQL.substr(0);
		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{
			int 	i;
			int   	Table_Attr_Type[500];
			string 	Table_Attr_Name[500];
			string	Table_Attr_Index[500];
			Table   table = CataM.get_table_info(Table_Name);
			int		Attr_N = CataM.attrCount(Table_Name);
			for (i = 0; i<Attr_N; i++)
			{
				Table_Attr_Type[i] = table.colum[i].type;
				Table_Attr_Name[i] = table.colum[i].name;
				if (table.colum[i].index != NULL)
				{
					if (table.colum[i].index->indexname == "") Table_Attr_Index[i] = "";
					else Table_Attr_Index[i] = table.colum[i].index->indexname;
				}
				else Table_Attr_Index[i] = "";
			}
			int N = CataM.recordCount(Table_Name);

			RecordM.Delete_All(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, Table_Attr_Index);
			CataM.set_zero(Table_Name);
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//删除表内部分数据（含where操作）
	else if (Type == "41")
	{
		//SQL格式：41name,a = 10
		int index = SQL.find(',');
		string Table_Name = SQL.substr(0, index);
		SQL = SQL.substr(index + 1);

		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{
			string ATTR;
			string Attr_Name[500];
			int op[500];
			string Num[500];
			string Index_Name[500];
			int count = 0;
			int FLAG = 0;
			int op_N;
			int i;

			Table table = CataM.get_table_info(Table_Name);
			int Attr_N = CataM.attrCount(Table_Name);

			while (SQL.length() != 0)
			{
				index = SQL.find(',');
				ATTR = SQL.substr(0, index);
				SQL = SQL.substr(index + 1);

				//确定Attr_Name[count] , Num[count]
				string OP;
				index = ATTR.find(' ');
				Attr_Name[count] = ATTR.substr(0, index);
				ATTR = ATTR.substr(index + 1);
				index = ATTR.find(' ');
				OP = ATTR.substr(0, index);
				Num[count] = ATTR.substr(index + 1);

				int flag = 0;

				//确定op[count]
				if (OP == "=") op[count] = 0;
				else if (OP == "<>") op[count] = 1;
				else if (OP == "<") op[count] = 2;
				else if (OP == ">") op[count] = 3;
				else if (OP == "<=") op[count] = 4;
				else if (OP == ">=") op[count] = 5;
				else flag = 1;


				//确定索引Index_Name[count]
				if (!CataM.attrExists(Table_Name, Attr_Name[count]))
				{
					cout << "error: There's NO attributes called " << Attr_Name[count] << endl;
					FLAG = 1;
					break;
				}
				else if (flag == 1)
				{
					cout << "error: The operator is illegal." << endl;
					FLAG = 1;
					break;
				}
				else
				{
					Index_Name[count] = "";
					for (i = 0; i<Attr_N; i++)
						if (table.colum[i].index != NULL && table.colum[i].index->columnName == Attr_Name[count])
						{
							Index_Name[count] = table.colum[i].index->indexname;
							break;
						}
				}

				count++;
			}

			if (FLAG == 0)
			{
				op_N = count;
				int 	 count = 0;
				int   	 Table_Attr_Type[500];
				string 	 Table_Attr_Name[500];
				Table    table = CataM.get_table_info(Table_Name);
				for (i = 0; i<Attr_N; i++)
				{
					Table_Attr_Type[i] = table.colum[i].type;
					Table_Attr_Name[i] = table.colum[i].name;
				}
				int N = CataM.recordCount(Table_Name);
				RecordM.Delete(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, op_N, Attr_Name, op, Num, Index_Name);
			}

		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//帮助部分（help）
	else if (Type == "80")
	{
		cout << "HELP PART (TO BE FINISHED)" << endl;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//终止运行
	else if (Type == "70") return 0;

	return 1;

}
