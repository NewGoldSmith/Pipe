#pragma once


// CEditNum

class CEditNum : public CEdit
{
	DECLARE_DYNAMIC(CEditNum)

public:
	CEditNum();
	virtual ~CEditNum();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int GetNum();
	void SetNum(int num);
};


