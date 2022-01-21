#include "CBStream.h"

CBStream::CBStream():
    m_bConnected{false}
    , m_pEvOnDisconnect{nullptr}
{
}

CBStream::~CBStream()
{
}

bool CBStream::IsConnect()
{
    return m_bConnected;
}

void CBStream::SetEventHandler(type_pStreamEventHandler pEvent)
{
    m_pEvOnDisconnect = pEvent;
}

void CBStream::ClearEventHandler()
{
    m_pEvOnDisconnect = nullptr;
}
