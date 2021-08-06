#include<iostream>
#include "DB.h"
using namespace std;
CDB g_DB;
const string strDB("test");
const string strServer("localhost");
const string strUser("root");
const string strPassword("root");
const int nPort = 3306;
int main() {
	int nchoice=1;
	string strInput = "";
	g_DB.InitConnect(strDB, strServer, strUser, strPassword, nPort);
	while (nchoice) {
		cout << "请选择功能(1.增 2.删 3.改 4.查)" << endl;
		cin >> nchoice;

		getchar();
		switch (nchoice)
		{
		case 1: {
			mysqlpp::Query query(0);
			g_DB.getQuery(query);
			query << "insert into d_user(account,name) values(%0q:account, %1q:name)";
			query.parse();
			cout << "请输入账号:";
			getline(cin, strInput);
			query.template_defaults["account"] = strInput.c_str();
			cout << "请输入名称:";
			getline(cin, strInput);
			query.template_defaults["name"] = strInput.c_str();
			//g_DB.Insert(query); 
			cout << "Query:" << query.str() << endl;
		} break;
		case 2: {
			mysqlpp::Query query(0);
			g_DB.getQuery(query);
			query << "delete from d_user where %0:column = %1q:value;";
			query.parse();
			cout << "请输入要删除的列名:";
			getline(cin, strInput);
			query.template_defaults["column"] = strInput.c_str();
			cout << "请输入对应列具体值:";
			getline(cin, strInput);
			query.template_defaults["value"] = strInput.c_str();
			//g_DB.Delete(query);
			cout << "Query:" << query.str() << endl;
		}  break;
		case 3: {
			mysqlpp::Query query(0);
			g_DB.getQuery(query);
			query << "update `d_user` set lev=%0q:lev where accunt=%1q:id;";
			query.parse();
			cout << "请输入要升级的账号:";
			getline(cin, strInput);
			query.template_defaults["account"] = strInput.c_str();
			cout << "请输入要升的等级:";
			getline(cin, strInput);
			query.template_defaults["exp"] = stoi(strInput);
			//g_DB.Update(query); 
			cout << "Query:" << query.str() << endl;
		} break;
		case 4: {
			mysqlpp::Query query(0);
			g_DB.getQuery(query);
			query << "select * from d_user where %0:column = %1q:value;";
			query.parse();
			cout << "请输入要查询的列名:";
			getline(cin, strInput);
			query.template_defaults["column"] = strInput.c_str();
			cout << "请输入对应列具体值:";
			getline(cin, strInput);
			query.template_defaults["value"] = strInput.c_str();
			mysqlpp::UseQueryResult res;
			/*g_DB.Search(res, query);
			int len = res.num_fields();
			for (int i = 0; i < len; i++) {
				cout <<setw(25) << left <<res.field_name(i);
			}
			cout << endl;
			while (mysqlpp::Row  row = res.fetch_row()){
				for (int i = 0; i < len; i++) {
					cout << setw(25) << left << row[i] ;
				}cout << endl;
			}*/
			cout << "Query:" << query.str() << endl;

		}break;
		default:nchoice = 0;
			break;
		}
	}
}