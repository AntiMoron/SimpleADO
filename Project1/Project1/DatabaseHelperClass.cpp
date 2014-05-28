#include "DatabaseHelperClass.h"
DatabaseHelperClass* DatabaseHelperClass::instance = new DatabaseHelperClass;

DatabaseHelperClass::DatabaseHelperClass()
{
}


DatabaseHelperClass::~DatabaseHelperClass()
{
}

DatabaseHelperClass*& DatabaseHelperClass::GetInst()
{
	return instance;
}

bool DatabaseHelperClass::Initialize()
{
	HRESULT hr = S_OK;
	CoInitialize(nullptr);
	try{
		m_connection.CreateInstance(__uuidof(ADODB::Connection));
		m_pCommand.CreateInstance(__uuidof(ADODB::Command));
	}
	catch (_com_error &ex)
	{
		MessageBox(NULL, ex.ErrorMessage(), L"error", MB_OK);
		return false;
	}
	_bstr_t ConnectionStr = "Driver={sql server};server=127.0.0.1;database=ks";
	try
	{
		m_connection->Open(ConnectionStr, "sa", "123", NULL);
		m_pCommand->ActiveConnection = m_connection;
	}
	catch (_com_error &ex)
	{
		MessageBox(NULL, ex.ErrorMessage(), L"error", MB_OK);
		return false;
	}
	if (FAILED(hr)) return false;
	return true;
};

bool DatabaseHelperClass::Shutdown()
{
	return false;
}

bool DatabaseHelperClass::ExecuteSQL(const char* sqlstr)
{
	HRESULT hr;
	try
	{
		m_pCommand->CommandText = sqlstr;
		m_recordset = m_pCommand->Execute(NULL,NULL,ADODB::adCmdText);
/*		m_recordset->Open((_variant_t)sqlstr,m_connection.GetInterfacePtr()
			,ADODB::adOpenDynamic,
			ADODB::adLockOptimistic,
			ADODB::adCmdText);
*/		m_recordset->MoveFirst();
	}
	catch (_com_error &ex)
	{
		return false;
	}
	return true;
}



ADODB::_RecordsetPtr& DatabaseHelperClass::GetResult()
{
	return m_recordset;
}

bool DatabaseHelperClass::next()
{
	HRESULT hr;
	try
	{
		hr = m_recordset->MoveNext();
		if (FAILED(hr))
			return false;
	}
	catch (_com_error ex)
	{
		return false;
	}
	return false;
}

bool DatabaseHelperClass::MoveFirst()
{
	HRESULT hr = S_OK;
	try
	{
		hr = m_recordset->MoveFirst();
	}
	catch (_com_error e)
	{
		MessageBox(NULL,e.ErrorMessage(),L"Error",MB_OK);
		return false;
	}
	return true;
}

const string DatabaseHelperClass::operator[](char* str)const
{
	_variant_t var = m_recordset->GetCollect(str);
	string ret = (_bstr_t) var;
	return 	ret;
}

bool DatabaseHelperClass::hasNext()
{
	try
	{
		return !m_recordset->adoEOF;
	}
	catch (_com_error ee)
	{
		return false;
	}
	return false;
}

bool DatabaseHelperClass::Insert(string tablename,int vNum,...)
{
	try
	{
		string SQLstr = "select * from " + tablename + " ";
		ExecuteSQL(SQLstr.c_str());
		va_list lst;
		va_start(lst, vNum);
		m_recordset->AddNew();
		for (int i = 0; i < vNum; i++)
		{
			const char* val= va_arg(lst, const char*);
			m_recordset->Fields->GetItem(m_recordset->Fields->GetItem((long)i)->GetName())->Value = (_bstr_t)val;
		}
		m_recordset->Update();
	}
	catch (_com_error &ex)
	{
		MessageBox(NULL,ex.ErrorMessage(),L"Error",MB_OK);
		return false;
	}
	return true;
}