#include <CProtocolBD.h>

CProtocolBD::CProtocolBD():
    m_pBDStream{ nullptr }
{
}

CProtocolBD::~CProtocolBD()
{
}

bool CProtocolBD::Init(CBStream* pBDStream)
{
    m_pBDStream = pBDStream;
    return true;
}

int CProtocolBD::Read(CBinaryString& BD)
{
    if (m_pBDStream == nullptr)
    {
        BD.SetDataSize(0);
        return 0;
    }
    return m_pBDStream->Read(BD);
}

int CProtocolBD::Write(const CBinaryString& BD)
{
    if (m_pBDStream == nullptr)
    {
        return 0;
    }
    return m_pBDStream->Write(BD);
}

