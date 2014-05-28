#include<iostream>
#include<stdio.h>
#include<string>
#include<regex>
#include"DatabaseHelperClass.h"
using namespace std;

#define DATABASE (*(DatabaseHelperClass::GetInst()))

int main()
{
	if (DATABASE.Initialize())
		printf("database login succeed!\n");
	cout<<"Welcome to talk with me!"<<endl;
	string input_chat;
	
	DATABASE.ExecuteSQL("select * from cjks_wt where id = 10");
	while (DATABASE.hasNext())
	{
		cout << DATABASE["quest"] << endl;
		DATABASE.next();
	}
	DATABASE.ExecuteSQL("select * from cjks_wt where id = 15");
	while (DATABASE.hasNext())
	{
		cout << DATABASE["quest"] << endl;
		DATABASE.next();
	}
	DATABASE.Shutdown();
	return 0;
}