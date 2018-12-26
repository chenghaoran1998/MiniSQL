#include "MiniSQL.h"
#include "Interpreter.h"
#include "API_Module.h"

void main()
{
	string SQL;
	cout << endl;
	cout << "***********************************************" << endl;
	cout << endl;
	cout << "	              Version (1.0)  " << endl;
	cout << endl;
	cout << "        Copyright(2018) all right reserved		" << endl;
	cout << "***********************************************" << endl;
	cout << endl << endl;
	while (1)
	{
		cout << "MiniSQL-->> ";
		SQL = "";
		SQL = Interpreter(SQL);
		cout <<endl <<"[Interpreter传出指令]"<< endl << SQL << endl;
		cout <<"[API调用测试]" << endl;
		int signal = API_Module(SQL);
		cout << endl << endl;

		if (signal == 0)
		{
			cout << "See you next time~";
			break;
		}
	}
}


