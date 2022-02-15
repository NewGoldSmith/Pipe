#include "CReadNumByte.h"

CReadNumByte::CReadNumByte()
	:m_strPathName()
	, m_hFile(nullptr)
	, m_iNumByte(1)
	,m_bRepeat(false)
	, m_bEnd(false)
{
}

CReadNumByte::~CReadNumByte()
{
	Close();
}

void CReadNumByte::SetFileName(CBinaryString& strPathName)
{
	m_strPathName = strPathName;
}

bool CReadNumByte::Open()
{
	if (m_strPathName.empty())
	{
		return false;
	}
	m_hFile = CreateFile(
		m_strPathName.c_strw()	//_In_ LPCWSTR lpFileName,
		, GENERIC_READ			//_In_ DWORD dwDesiredAccess,
		, FILE_SHARE_READ		//_In_ DWORD dwShareMode,
		, NULL					//_In_opt_ LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		, OPEN_EXISTING			//_In_ DWORD dwCreationDisposition,
		, FILE_ATTRIBUTE_NORMAL	//_In_ DWORD dwFlagsAndAttributes,
		, NULL					//_In_opt_ HANDLE hTemplateFile
	);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		m_hFile = nullptr;
		return false;
	}
	return true;
}

int CReadNumByte::ReadOneTime(CBinaryString& str)
{
	if (m_hFile == nullptr)
	{
		return -1;
	}
	if (m_bEnd == true)
	{
		return 0;
	}
	str.Detach();
	DWORD reads(0);
	if (!ReadFile(
		m_hFile
		, str.GetBufferA()
		, m_iNumByte
		, &reads
		, NULL
	))
	{
		return -1;
	}
	str.SetDataSize(reads);
	if (reads < m_iNumByte)
	{
		SetStartPos();
		if (m_bRepeat == true)
		{
		}
		else {
			m_bEnd = true;
		}
	}
	return reads;
}

void CReadNumByte::SetNumBytesInOneTime(int iNum)
{
	m_iNumByte = iNum;
}

bool CReadNumByte::SetStartPos()
{
	if (m_hFile == nullptr)
	{
		return false;
	}
	if (SetFilePointer(
		m_hFile
		, 0
		, NULL
		, FILE_BEGIN
	) == INVALID_SET_FILE_POINTER)
	{
		return false;
	}
	m_bEnd = false;
	return true;
}

bool CReadNumByte::Close()
{
	if (m_hFile == nullptr)
	{
		return true;
	}
	CloseHandle(m_hFile);
	m_hFile = nullptr;
	return true;
}

bool CReadNumByte::IsOpen()
{
	return m_hFile!=nullptr;
}

void CReadNumByte::SetRepeat(bool bRepeat)
{
	m_bRepeat = bRepeat;
}

bool CReadNumByte::IsEnd()
{
	return m_bEnd;
}
