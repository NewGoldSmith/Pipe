
#include <CBStreamQue.h>

CBStreamQue::CBStreamQue()
	:CBStream()
	,m_Mtx{ 1 }
{
	Connect();
}

CBStreamQue::~CBStreamQue()
{
}

int CBStreamQue::Read(CBinaryString& BD)
{
	m_Mtx.acquire();

	if (CBStream::Read(BD))
	{
		m_Mtx.release();
		return -1;
	}

	if (m_BStringQue.empty())
	{
		m_Mtx.release();
		return 0;
	}
	int i = 0;
	for (i= m_BStringQue.size(); 0 < i; --i)
	{
		BD += m_BStringQue.front();
		m_BStringQue.pop();
	}
	m_Mtx.release();
	return BD.GetDataSize();
}

int CBStreamQue::Write(const CBinaryString& BD)
{
	m_Mtx.acquire();

	if (BD.empty())
	{
		m_Mtx.release();
		return -1;
	}

	if (CBStream::Write(BD))
	{
		m_Mtx.release();
		return -1;
	}

	CBinaryString str;
	str.Detach(BD);
	m_BStringQue.push(str);
	m_Mtx.release();
	return BD.GetDataSize();
}

bool CBStreamQue::Connect()
{
	Discard();
	CBStream::Connect();
	return m_bConnected;
}

bool CBStreamQue::Disconnect()
{
	Discard();
	CBStream::Disconnect();
	return true;
}

int CBStreamQue::Discard()
{
	CBinaryString BD;
	int contents_size(0);
	m_Mtx.acquire();
	int size=m_BStringQue.size();
	for (int i=size; 0 < i; --i)
	{
		BD = m_BStringQue.front();
		contents_size += BD.GetDataSize();
		m_BStringQue.pop();
	}
	m_Mtx.release();
	return contents_size;
}
