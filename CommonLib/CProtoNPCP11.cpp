#include "CProtoNPCP11.h"


CNPCP11::CNPCP11():
    m_ThisDeviceType{emDeviceType::UNDEF}
    ,m_PeerDeviceType{emDeviceType::UNDEF}
    ,m_pBStream{nullptr}
    , m_bConnected{false}
{
	mapLowFirstWords.emplace("NPCP", std::bind(&CNPCP11::OnLowNPCP, this, std::placeholders::_1));
    mapLowFirstWords.emplace("IM", std::bind(&CNPCP11::OnLowIM, this, std::placeholders::_1));
    mapLowFirstWords.emplace("EXEC", std::bind(&CNPCP11::OnLowEXEC, this, std::placeholders::_1));
    mapLowFirstWords.emplace("NOTE", std::bind(&CNPCP11::OnLowNOTE, this, std::placeholders::_1));
    mapLowFirstWords.emplace("QUIT", std::bind(&CNPCP11::OnLowQUIT, this, std::placeholders::_1));
    mapLowFirstWords.emplace("SEND", std::bind(&CNPCP11::OnLowSEND, this, std::placeholders::_1));
    mapLowFirstWords.emplace("REP", std::bind(&CNPCP11::OnLowREP, this, std::placeholders::_1));

    mapDeviceType.emplace("UNDEF", emDeviceType::UNDEF);
    mapDeviceType.emplace("PXY", emDeviceType::PXY);
    mapDeviceType.emplace("NPHUB", emDeviceType::NPHUB);
    mapDeviceType.emplace("STUB", emDeviceType::STUB);
    mapDeviceType.emplace("FORWARDER", emDeviceType::FORWARDER);

    mapUpFirstWords.emplace("NPCP", std::bind(&CNPCP11::OnUpNPCP, this, std::placeholders::_1));
    mapUpFirstWords.emplace("EXEC", std::bind(&CNPCP11::OnUpEXEC, this, std::placeholders::_1));
    mapUpFirstWords.emplace("NOTE", std::bind(&CNPCP11::OnUpNOTE, this, std::placeholders::_1));
    mapUpFirstWords.emplace("QUIT", std::bind(&CNPCP11::OnUpQUIT, this, std::placeholders::_1));
    mapUpFirstWords.emplace("SEND", std::bind(&CNPCP11::OnUpSEND, this, std::placeholders::_1));
    mapUpFirstWords.emplace("REP", std::bind(&CNPCP11::OnUpREP, this, std::placeholders::_1));
}

CNPCP11::~CNPCP11()
{
}

bool CNPCP11::Init(CBStream* pBStream, emDeviceType emThisType, std::string strThisID)
{
    m_pBStream = pBStream;
    m_ThisDeviceType = emThisType;
    m_strThisID = strThisID;
    return true;
}

bool CNPCP11::LowIN(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    bool rVal = { false };
    CBinaryString strFirst = tmp.TrimFirstCodeA(' ');
    for (auto iter : mapLowFirstWords) {
        if (string(strFirst.c_strA()) == iter.first) {
            rVal=iter.second(tmp);
            break;
        }
    }
    return rVal;
}

bool CNPCP11::UpIN(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString bstrID = tmp.TrimFirstCodeA(' ');
    bool rVal{ false };
    if (bstrID == m_strPeerID)
    {
        rVal=UpInternalIN(tmp);
    }
    return rVal;
}

bool CNPCP11::UpInternalIN(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    bool rVal = { false };
    CBinaryString strFirst = tmp.TrimFirstCodeA(' ');
    for (auto iter : mapUpFirstWords) {
        if (string(strFirst.c_strA()) == iter.first) {
            rVal = iter.second(tmp);
        }
    }
    return false;
}

bool CNPCP11::ConnectNPCPServer()
{
    CBinaryString strDeviceType;
    for (auto iter : mapDeviceType) {
        if (m_ThisDeviceType == iter.second) {
            strDeviceType = iter.first;
            break;
        }
    }
    if (strDeviceType.empty() || strDeviceType == "UNDEF")
    {
        return false;
    }
    CBinaryString strREP;
    CSecTimer timer;
    bool timeout{ false };
    timer.SetParam(&timeout, 10);
    timer.Start();
    while (strREP.empty() && !timeout)
    {
        m_pBStream->Read(strREP);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    if (strREP.empty())
        return false;
    m_bConnected = true;
    return true;
}

bool CNPCP11::OnLowNPCP(const CBinaryString&bstr)
{
    bool rVal{ false };
    bool rVal2{ false };
    CBinaryString tmp = bstr;
    CBinaryString strVar = tmp.TrimFirstCodeA(' ');
    if (strVar!="1.1")
    {
        rVal= false;
    }
    else {
        rVal = true;
    }
    m_strPeerID = tmp.TrimFirstCodeA(' ');
    CBinaryString strDeviceType = tmp.TrimFirstCodesA("\r\n");
    for (auto iter : mapDeviceType) {
        if (string(strDeviceType.c_strA()) == iter.first) {
            m_PeerDeviceType=iter.second;
            rVal2 = true;
        }
    }
    if (rVal&&rVal2)
    {
        m_pBStream->Write("REP OK\r\n");
        CBinaryString bstr = "CONNECTED " + m_strPeerID + " " + strDeviceType + "\r\n";
        m_queReadForUpper.push(bstr);

        CBinaryString strThisDeviceType;
        for (auto iter : mapDeviceType) {
            if (m_ThisDeviceType == iter.second) {
                strThisDeviceType = iter.first;
            }
        }
        bstr = "IM " + m_strThisID + " "+strThisDeviceType +"\r\n";
        m_pBStream->Write(bstr);
    }
    else {
        m_pBStream->Write("REP NG\r\n");
    }
	return rVal;
}

bool CNPCP11::OnLowIM(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    m_strPeerID = tmp.TrimFirstCodeA(' ');
    CBinaryString strDeviceType = tmp.TrimFirstCodesA("\r\n");
    bool rVal{ false };
    for (auto iter : mapDeviceType) {
        if (string(strDeviceType.c_strA()) == iter.first) {
            m_PeerDeviceType = iter.second;
            rVal = true;
        }
    }
    CBinaryString word = "CONNECTED " + m_strPeerID + " " + strDeviceType + "\r\n";
    m_queReadForUpper.push(word);
    return rVal;
}

bool CNPCP11::OnLowEXEC(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString strHTS = tmp.TrimFirstBFormat();
    CBinaryString send = m_strPeerID + " EXEC " + strHTS + "\r\n";
    m_queReadForUpper.push(send);
    tmp.TrimFirstCodesA("\r\n");
    bool rVal = { true };
    if (!tmp.empty())
    {
        rVal&=LowIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnLowNOTE(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString  word2 = tmp.TrimFirstCodeA(' ');
    CBinaryString word3;
    CBinaryString word4;
    bool rVal = { false };
    if (word2 == "DISCONNECTED")
    {
        word3 = tmp.TrimFirstCodesA("\r\n");
        word4 = m_strPeerID + " NOTE DISCONNECTED " + word3+"\r\n";
        m_queReadForUpper.push(word4);
        rVal = true;
    }
    if (word2 == "EXEC")
    {
        word3 = tmp.TrimFirstCodesA("\r\n");
        word4 = m_strPeerID + " NOTE EXEC " + word3+"\r\n";
        m_queReadForUpper.push(word4);
        rVal = true;
    }
    if (!tmp.empty())
    {
        rVal &= LowIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnLowQUIT(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString strQUITDevice = tmp.TrimFirstCodesA("\r\n");
    CBinaryString send = m_strPeerID + " QUIT " + strQUITDevice+"\r\n";
    m_queReadForUpper.push(send);
    bool rVal = { true };
    if (!tmp.empty())
    {
        rVal &= LowIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnLowSEND(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString strHTS = tmp.TrimFirstBFormat();
    CBinaryString send = m_strPeerID + " RECEIVED " + strHTS + "\r\n";
    m_queReadForUpper.push(send);
    tmp.TrimFirstCodesA("\r\n");
    bool rVal = { true };
    if (!tmp.empty())
    {
        rVal &= LowIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnLowREP(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString strOKNG = tmp.TrimFirstCodesA("\r\n");
    CBinaryString send = m_strPeerID + " QUIT " + strOKNG + "\r\n";
    m_queReadForUpper.push(send);
    bool rVal = { true };
    if (!tmp.empty())
    {
        rVal &= LowIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnUpNPCP(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString trimNPCP = tmp.TrimFirstCodesA("\r\n");
    bool rVal{ false };
    size_t len = m_pBStream->Write(trimNPCP);
    if (len == -1)
    {
        rVal = false;
    }
    else {
        rVal = true;
    }
    if (!tmp.empty())
    {
        rVal&=UpIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnUpEXEC(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString strTHS = tmp.TrimFirstBFormat();
    CBinaryString strSend = "EXEC " + strTHS + "\r\n";
    size_t len=m_pBStream->Write(strSend);
    bool rVal{ false };
    if (len == -1)
    {
        CBinaryString strRep = m_strPeerID + " NOTE EXEC NG\r\n";
        m_queReadForUpper.push(strRep);
        rVal = false;
    }
    else 
    {
        rVal = true;

        CBinaryString strREP;
        CSecTimer timer;
        bool timeout{ false };
        timer.SetParam(&timeout, 5);
        timer.Start();
        while (strREP.empty() && !timeout)
        {
            m_pBStream->Read(strREP);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        if (strREP.empty())
        {
            CBinaryString strRep = m_strPeerID + " NOTE EXEC NG\r\n";
            m_queReadForUpper.push(strRep);
            return false;
        }
        else 
        {
            CBinaryString strNote = strREP.TrimFirstCodeA(' ');
            CBinaryString strEXEC = strREP.TrimFirstCodeA(' ');
            CBinaryString strOKNG = strREP.TrimFirstCodesA("\r\n");
            if (strOKNG == "NG")
            {
                CBinaryString strRep = m_strPeerID + " NOTE EXEC NG\r\n";
                m_queReadForUpper.push(strRep);
                return false;
            }
            else if(strOKNG=="OK") 
            {
                CBinaryString strRep = m_strPeerID + " NOTE EXEC OK\r\n";
                m_queReadForUpper.push(strRep);
                return true;
            }
        }
    }
    return rVal;
}

bool CNPCP11::OnUpNOTE(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString trim = tmp.TrimFirstCodesA("\r\n");
    bool rVal{ false };
    CBinaryString word = "NOTE " + trim + "\r\n";
    size_t len = m_pBStream->Write(word);
    if (len == -1)
    {
        rVal = false;
    }
    else {
        rVal = true;
    }
    if (!tmp.empty())
    {
        rVal &= UpIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnUpQUIT(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString trim = tmp.TrimFirstCodesA("\r\n");
    CBinaryString word = "QUIT " + trim + "\r\n";
    bool rVal{ false };
    size_t len = m_pBStream->Write(word);
    if (len == -1)
    {
        rVal = false;
    }
    else {
        rVal = true;
    }
    if (!tmp.empty())
    {
        rVal &= UpIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnUpSEND(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString trim = tmp.TrimFirstBFormat();
    bool rVal{ false };
    CBinaryString send = "SEND " + trim + "\r\n";
    size_t len = m_pBStream->Write(send);
    if (len == -1)
    {
        rVal = false;
    }
    else {
        rVal = true;
    }
    tmp.TrimFirstCodesA("\r\n");
    if (!tmp.empty())
    {
        rVal &= UpIN(tmp);
    }
    return rVal;
}

bool CNPCP11::OnUpREP(const CBinaryString& bstr)
{
    CBinaryString tmp = bstr;
    CBinaryString trim = tmp.TrimFirstCodesA("\r\n");
    bool rVal{ false };
    size_t len = m_pBStream->Write(trim);
    if (len == -1)
    {
        rVal = false;
    }
    else {
        rVal = true;
    }
    if (!tmp.empty())
    {
        rVal &= UpIN(tmp);
    }
    return rVal;
}

int CNPCP11::Read(CBinaryString& BD)
{
    if (!m_queReadForUpper.empty())
    {
        BD = m_queReadForUpper.front();
        m_queReadForUpper.pop();
    }
    return BD.GetDataSize();
}

int CNPCP11::Write(const CBinaryString& BD)
{
    m_queWriteForUpper.push(BD);
	return BD.GetDataSize();
}

