//Copyright (c) 2021, Gold Smith
//Released under the MIT license
//https ://opensource.org/licenses/mit-license.php

#include <CBStreamTerm.h>

CBStreamTerm::CBStreamTerm()
	:CBStream()
	,m_pStreamRead(nullptr)
	,m_pStreamWrite(nullptr)
{
	m_emAccessMode = CBStream::emAccessMode::Both;
}

CBStreamTerm::~CBStreamTerm()
{
}

int CBStreamTerm::Read(CBinaryString& BD)
{
	if (CBStream::Read(BD))
	{
		return -1;
	}
	return m_pStreamRead->Read(BD);
}

int CBStreamTerm::Write(const CBinaryString& BD)
{
	if (CBStream::Write(BD))
	{
		return -1;
	}
	return m_pStreamWrite->Write(BD);

}

bool CBStreamTerm::Connect()
{
	m_bConnected = (m_pStreamRead != nullptr) && (m_pStreamWrite != nullptr);
	return m_bConnected;
}

bool CBStreamTerm::Disconnect()
{
	m_bConnected=false;
	return true;
}

bool CBStreamTerm::IsConnect()
{
	return (m_pStreamRead!=nullptr)&&(m_pStreamWrite!=nullptr);
}

void CBStreamTerm::SetStream(CBStream* pStreamRead, CBStream* pStreamWrite)
{
	m_bConnected = false;
	m_pStreamRead = pStreamRead;
	m_pStreamWrite = pStreamWrite;
}

void CBStreamTerm::ClearStream()
{
	m_bConnected = false;
	m_pStreamRead = nullptr;
	m_pStreamWrite = nullptr;
}

int CBStreamTerm::Discard()
{
	if (!m_pStreamRead || !m_pStreamWrite)
	{
		return -1;
	}
	m_pStreamRead->Discard();
	m_pStreamWrite->Discard();
	return 0;
}

CBStreamTerm::emAccessMode CBStreamTerm::SetAccessMode(const emAccessMode emaccess)
{
	emAccessMode tmp= CBStream::SetAccessMode(emaccess);
	switch (m_emAccessMode)
	{
	case CBStream::emAccessMode::No:
		m_pStreamRead->SetAccessMode(emAccessMode::No);
		m_pStreamRead->Discard();
		m_pStreamWrite->SetAccessMode(emAccessMode::No);
		m_pStreamWrite->Discard();
		break;
	case CBStream::emAccessMode::Read:
		m_pStreamRead->SetAccessMode(emAccessMode::Both);
		m_pStreamWrite->SetAccessMode(emAccessMode::No);
		m_pStreamWrite->Discard();
		break;
	case CBStream::emAccessMode::Write:
		m_pStreamRead->SetAccessMode(emAccessMode::No);
		m_pStreamRead->Discard();
		m_pStreamWrite->SetAccessMode(emAccessMode::Both);
		break;
	case CBStream::emAccessMode::Both:
		m_pStreamRead->SetAccessMode(emAccessMode::Both);
		m_pStreamWrite->SetAccessMode(emAccessMode::Both);
		break;
	default:
		break;
	}
	return tmp;
}
