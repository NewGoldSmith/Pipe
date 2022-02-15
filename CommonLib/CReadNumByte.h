#pragma once
#include <CBStream.h>
#include <CBinaryString.h>
#include <Windows.h>
class CReadNumByte
{
public:
	CReadNumByte();
	CReadNumByte(CReadNumByte& otherobj) = delete;
	CReadNumByte(CReadNumByte&& otherobj) = delete;
	CReadNumByte& operator=(CReadNumByte& otherobj) = delete;
	CReadNumByte& operator=(CReadNumByte&& otherobj) = delete;
	CReadNumByte& operator()(CReadNumByte& otherobj) = delete;
	CReadNumByte& operator()(CReadNumByte&& otherobj) = delete;
	virtual ~CReadNumByte();
	void SetFileName(CBinaryString& strPathName);
	bool Open();
	int ReadOneTime(CBinaryString& str);
	void SetNumBytesInOneTime(int iNum);
	bool SetStartPos();
	void SetRepeat(bool bRepeat);
	bool Close();
	bool IsOpen();
	bool IsEnd();
protected:
	CBinaryString m_strPathName;
	HANDLE m_hFile;
	int m_iNumByte;
	bool m_bRepeat;
	bool m_bEnd;
};

