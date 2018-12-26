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


	//����SQLָ��
	string Type;
	Type = SQL.substr(0, 2);
	SQL = SQL.substr(2);

	/////////////////////////////////////////////////////////////////////////////////////////
	//�������ݱ�
	if (Type == "01")
	{
		//SQL��ʽ��01name,a + 0,b - 0,c 1 0,a #,

		//��Table
		int index = SQL.find(',');
		string Table_Name = SQL.substr(0, index);
		if (CataM.tableExists(Table_Name)) //�������Ƿ����
			cout << "error: There's already a table named" << Table_Name << "in the database." << endl;
		else
		{
			//����
			CataM.Create_Table(Table_Name);	//����Catalog M���ձ�
			SQL = SQL.substr(index + 1);
			//���β���ÿһ����Ԫ��
			while (SQL.length() != 0)
			{
				//��ȡ��ǰ���ATTR��������
				string ATTR;
				index = SQL.find(',');
				ATTR = SQL.substr(0, index);	//�� a,+,0 �ȵ�
				SQL = SQL.substr(index + 1);

				//����ÿ��������Ԫ�ص�����  
				string Attr_Name, Attr_Type, Attr_Argu;
				index = ATTR.find(' ');
				Attr_Name = ATTR.substr(0, index);
				ATTR = ATTR.substr(index + 1);
				index = ATTR.find(' ');
				Attr_Type = ATTR.substr(0, index);
				Attr_Argu = ATTR.substr(index + 1);

				//ִ�����Խ�������
				//��������
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
				// int���� ����catalog����Ϊ1
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
				// float���� ����catalog����Ϊ2
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
				// char(n)���� ����catalog����Ϊ-n
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
	//��������
	else if (Type == "02")
	{
		//SQL��ʽ��02name table attr
		//��ȡIndex_Name, Table_Name, Attr_Name;
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
			CataM.addIndex(Index_Name, Table_Name, Attr_Name);	//��CatalogM�����¼

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
			//IndexM.Create_Index(Table_Name, "1", Attr_N);	//��IndexM���뽨��������
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ�����ݱ�
	else if (Type == "11")
	{
		//SQL��ʽ��11name

		string Table_Name = SQL.substr(0);
		if (!CataM.tableExists(Table_Name))
			cout << "error: There's NO table called " << Table_Name << endl;
		else
		{
			//����table������index��Ϣ��ɾȥ����index
			int   i;
			Table table = CataM.get_table_info(Table_Name);
			int Attr_N = CataM.attrCount(Table_Name);
			for (i = 0; i<Attr_N; i++)
				if (CataM.hasIndex(Table_Name, table.colum[i].name))
				{
					string Index_Name = table.colum[i].index->indexname;
					//IndexM.Delete_Index(Table_Name, Index_Name);	//��Index
					CataM.deleteIndex(Index_Name);		//��Catalog
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

			//��Record����ɾ����������
			RecordM.Delete_All(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, Attr_Index);
			//��Catalog����ɾ�����¼������
			CataM.Drop_Table(Table_Name);

		}

	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ɾ������
	else if (Type == "12")
	{
		//SQL��ʽ��12name

		string Index_Name = SQL.substr(0);
		if (!CataM.indexExists(Index_Name))
			cout << "error: There's NO index called " << Index_Name << endl;
		else
		{
			//��Catalog����ɾ����������
			CataM.deleteIndex(Index_Name);
			//��Index����ɾ�����¼������
			/*IndexM.Delete_Index("a", Index_Name);*/
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//ѡ�����(��where�Ӿ� ��ѯ���м�¼)
	else if (Type == "20")
	{
		//SQL��ʽ��20*,table

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
	//ѡ�����(��where�Ӿ�)
	else if (Type == "21")
	{
		//SQL��ʽ��21*,table,a = 10, b >= 20
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

				//ȷ��Attr_Name[count] , Num[count]
				string OP;
				index = ATTR.find(' ');
				Attr_Name[count] = ATTR.substr(0, index);
				ATTR = ATTR.substr(index + 1);
				index = ATTR.find(' ');
				OP = ATTR.substr(0, index);
				Num[count] = ATTR.substr(index + 1);

				int flag = 0;

				//ȷ��op[count]
				if (OP == "=") op[count] = 0;
				else if (OP == "<>") op[count] = 1;
				else if (OP == "<") op[count] = 2;
				else if (OP == ">") op[count] = 3;
				else if (OP == "<=") op[count] = 4;
				else if (OP == ">=") op[count] = 5;
				else flag = 1;

				//ȷ������Index_Name[count]
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
	//����в�������
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

			//��֤unique��Ϣ
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
				//Catalog�����Ӽ�¼
				CataM.Insert_Data(Table_Name);

				//Record��������
				RecordM.Insert(N, Table_Name, Attr_N, Table_Attr_Type, Table_Attr_Name, value);

				//Index�����Ӽ�¼
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
	//ɾ��������������
	else if (Type == "40")
	{
		//SQL��ʽ��40name
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
	//ɾ�����ڲ������ݣ���where������
	else if (Type == "41")
	{
		//SQL��ʽ��41name,a = 10
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

				//ȷ��Attr_Name[count] , Num[count]
				string OP;
				index = ATTR.find(' ');
				Attr_Name[count] = ATTR.substr(0, index);
				ATTR = ATTR.substr(index + 1);
				index = ATTR.find(' ');
				OP = ATTR.substr(0, index);
				Num[count] = ATTR.substr(index + 1);

				int flag = 0;

				//ȷ��op[count]
				if (OP == "=") op[count] = 0;
				else if (OP == "<>") op[count] = 1;
				else if (OP == "<") op[count] = 2;
				else if (OP == ">") op[count] = 3;
				else if (OP == "<=") op[count] = 4;
				else if (OP == ">=") op[count] = 5;
				else flag = 1;


				//ȷ������Index_Name[count]
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
	//�������֣�help��
	else if (Type == "80")
	{
		cout << "HELP PART (TO BE FINISHED)" << endl;
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//��ֹ����
	else if (Type == "70") return 0;

	return 1;

}
