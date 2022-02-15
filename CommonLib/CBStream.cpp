#include "CBStream.h"

CBStream::CBStream():
    m_bConnected(false)
    , m_pEvOnDisconnect(nullptr)
    , m_emAccessMode(emAccessMode::Both)
{
}

CBStream::~CBStream()
{
}

int CBStream::Read(CBinaryString& BD)
{
	BD.Clear();
	if (!m_bConnected || 
		!((m_emAccessMode == emAccessMode::Both)||(m_emAccessMode == emAccessMode::Read)))
	{
		return -1;
	}
	return 0;
}

int CBStream::Write(const CBinaryString& BD)
{
	if (!m_bConnected || 
		!((m_emAccessMode == emAccessMode::Both)||(m_emAccessMode == emAccessMode::Write)))
	{
		return -1;
	}
	return 0;
}

bool CBStream::Connect()
{
	m_bConnected = true;
	return true;
}

bool CBStream::Disconnect()
{
	m_bConnected = false;
	return true;
}

bool CBStream::IsConnect()
{
    return m_bConnected;
}

int CBStream::Discard()
{
    return 0;
}

void CBStream::SetEventHandler(const type_pStreamEventHandler pEvent)
{
    m_pEvOnDisconnect = pEvent;
}

void CBStream::ClearEventHandler()
{
    m_pEvOnDisconnect = nullptr;
}

CBStream::emAccessMode CBStream::SetAccessMode(const CBStream::emAccessMode emaccess)
{
    emAccessMode tmp = m_emAccessMode;
    m_emAccessMode = emaccess;
    return tmp;
}


