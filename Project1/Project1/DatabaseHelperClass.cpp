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
		m_connection = nullptr;
		hr = m_connection.CreateInstance(__uuidof(ADODB::Connection));
	}
	catch (_com_error &ex)
	{
		MessageBox(NULL, ex.ErrorMessage(), L"error", MB_OK);
		return false;
	}
	if(FAILED(hr)) return false;
	_bstr_t ConnectionStr = "Driver={sql server};server=127.0.0.1;database=MyAI";
	try
	{
		m_connection->Open(ConnectionStr,"sa","123",NULL);
	}
	catch (_com_error &ex)
	{
		MessageBox(NULL, ex.ErrorMessage(), L"error", MB_OK);
		return false;
	}
	hr = m_recordset.CreateInstance(__uuidof(ADODB::Recordset));
	if (FAILED(hr)) return false;
	return true;
};

bool DatabaseHelperClass::Shutdown()
{
	return false;
}

bool DatabaseHelperClass::ExecuteSQL(char* sqlstr)
{
	HRESULT hr;
	try
	{
		m_recordset->Open((_variant_t)sqlstr,m_connection.GetInterfacePtr()
			,ADODB::adOpenDynamic,
			ADODB::adLockOptimistic,
			ADODB::adCmdText);
		m_recordset->MoveFirst();
	}
	catch (_com_error &ex)
	{
		return false;
	}
	return true;
}

bool DatabaseHelperClass::ExecuteSQL(string sqlstr)
{
	HRESULT hr;
	try
	{
		m_recordset->Open((_variant_t)sqlstr.c_str(), m_connection.GetInterfacePtr()
			, ADODB::adOpenDynamic,
			ADODB::adLockOptimistic,
			ADODB::adCmdText);
		m_recordset->MoveFirst();
	}
	catch (_com_error &ex)
	{
		printf("%X\n", m_recordset);
		MessageBox(NULL, ex.ErrorMessage(), L"error", MB_OK);
		return false;
	}
	return true;
}


ADODB::_RecordsetPtr& DatabaseHelperClass::GetResult()
{
	return m_recordset;
}


bool DatabaseHelperClass::hasNext()
{
	return m_recordset->adoEOF == VARIANT_FALSE;
}

bool DatabaseHelperClass::next()
{
	HRESULT hr;
	hr = m_recordset->MoveNext();
	if (FAILED(hr))
		return false;
	return true;
}

bool DatabaseHelperClass::MoveFirst()
{
	HRESULT hr = S_OK;
	hr = m_recordset->MoveFirst();
	if (FAILED(hr)) return false;
	return true;
}

const string DatabaseHelperClass::operator[](char* str)const
{
	string ret = (char*)(_bstr_t)(m_recordset->Fields->GetItem(_variant_t(str))->Value);
	return 	ret;
}

bool DatabaseHelperClass::Insert(string tablename,int vNum,...)
{
	try
	{
		string SQLstr = "select * from " + tablename + " ";
		ExecuteSQL(SQLstr);
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