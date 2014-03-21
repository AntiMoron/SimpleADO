﻿#pragma once
#include<iostream>
#include<string>
#include<icrsint.h>
#include<exception>
#include<mmsystem.h>

#pragma comment(lib,"winmm.lib")

#import "C:/Program Files/Common Files/System/ado/msado15.dll" rename("EOF","adoEOF")

using namespace std;

class DatabaseHelperClass
{
private:
	ADODB::_ConnectionPtr m_connection;
	ADODB::_RecordsetPtr  m_recordset;
	DatabaseHelperClass();
	~DatabaseHelperClass();
	static DatabaseHelperClass* instance;
public:
	static DatabaseHelperClass*& GetInst();
	bool Initialize();
	bool Shutdown();
	bool ExecuteSQL(char* SQLstr);
	bool ExecuteSQL(string SQLstr);
	ADODB::_RecordsetPtr& GetResult();

	bool Insert(string tablename,int valnum,...);


//record set operations.
	bool hasNext();
	bool next();
	bool MoveFirst();
	const string operator [](char*)const;
};


