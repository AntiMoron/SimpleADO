#include<iostream>
#include<stdio.h>
#include<string>
#include<regex>
#include"DatabaseHelperClass.h"
using namespace std;

#define DATABASE (*(DatabaseHelperClass::GetInst()))

int main()
{
	;
	if (DATABASE.Initialize())
		printf("database login succeed!\n");
	cout<<"Welcome to talk with me!"<<endl;
	string input_chat;
	DATABASE.ExecuteSQL("select * from testtable");
	while (cin >> input_chat)
	{
		DATABASE.Insert("testtable",1,input_chat);
	}
	DATABASE.Shutdown();
	return 0;
}