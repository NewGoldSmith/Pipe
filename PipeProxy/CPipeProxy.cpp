#include "CPipeProxy.h"


CPipeProxy::CPipeProxy()
	:CNPCP11()
	, m_hChildDownWr(nullptr)
	, m_hChildUpRd(nullptr)
	, m_ExeEngine()
	,pEventFunction(nullptr)
	,m_bEndFlag(false)
{
	m_ExeEngine.SetEventHandler(std::bind(&CPipeProxy::OnEvExeEnd,this));
}

CPipeProxy::~CPipeProxy()
{
	m_ExeEngine.ClearEventHandler();
	if (m_ExeEngine.IsRun())
	{
		m_ExeEngine.ForcedTermination();
	}
	CloseHandle(m_hChildUpRd);
	CloseHandle(m_hChildDownWr);
}

bool CPipeProxy::OneWork()
{
	int rVal{};
	if (m_bEndFlag)
	{
		return false;
	}
	CBinaryString bstr;
	if(!m_pBStream->IsConnect())
		return false;

	m_pBStream->Read(bstr);
	if (!bstr.empty())
	{
		LowIN(bstr);
	}
	m_PipeChild.Read(bstr);
	if (!bstr.empty())
	{
		bstr.MakeBFormat();
		CBinaryString bstr2 = "SEND " + bstr + "\r\n";
		m_pBStream->Write(bstr2);
	}
	return true;
}

bool CPipeProxy::Init(CBStream* pBDStream, emDeviceType emThisType, std::string strThisID)
{
	CNPCP11::Init(pBDStream, emThisType, strThisID);
	pBDStream->SetEventHandler(std::bind(&CPipeProxy::OnEvSevDisconnected, this));
	return false;
}

bool CPipeProxy::OnLowEXEC(const CBinaryString& bstr)
{
	if (m_ExeEngine.IsRun())
	{
		m_pBStream->Write("NOTE EXEC ALREADY_RUNNING\r\n");
		return false;
	}
	CBinaryString tmp = bstr;
	SECURITY_ATTRIBUTES saAttr = {};

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	HANDLE  hChild_Up_Wr, hChild_Down_Rd;

	if (!CreatePipe(&m_hChildUpRd, &hChild_Up_Wr, &saAttr, 0))
		return false;
	if (!SetHandleInformation(m_hChildUpRd, HANDLE_FLAG_INHERIT, 0))
		return false;

	if (!CreatePipe(&hChild_Down_Rd, &m_hChildDownWr, &saAttr, 0))
		return false;
	if (!SetHandleInformation(m_hChildDownWr, HANDLE_FLAG_INHERIT, 0))
		return false;
	CBinaryString exename = tmp.TrimFirstBFormat();
	exename.UnMakeBFormat();
	std::wstring wstr = exename.c_strw();
	if (!m_ExeEngine.Execute(wstr, hChild_Down_Rd, hChild_Up_Wr, hChild_Up_Wr, STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW))
	{
		CBinaryString rep = "NOTE EXEC NG\r\n";
		m_pBStream->Write(rep);
		return false;
	}

	CloseHandle(hChild_Down_Rd);
	CloseHandle(hChild_Up_Wr);
	m_PipeChild.InitPipeWork(m_hChildUpRd, m_hChildDownWr);
	CBinaryString rep = "NOTE EXEC OK\r\n";
	m_pBStream->Write(rep);
	tmp.TrimFirstCodesA("\r\n");
	bool rVal = { true };
	if (!tmp.empty())
	{
		rVal &= LowIN(tmp);
	}
	return rVal;
}

bool CPipeProxy::OnLowNOTE(const CBinaryString& bstr)
{
    return false;
}

bool CPipeProxy::OnLowQUIT(const CBinaryString& bstr)
{
	CBinaryString tmp = bstr;
	CBinaryString strQUITDevice = tmp.TrimFirstCodesA("\r\n");
	if (strQUITDevice == "PEER")
	{
		m_bEndFlag = true;
	}
	else if (strQUITDevice == "EXEC")
	{
		m_ExeEngine.ForcedTermination();
	}

	if (!tmp.empty())
	{
		LowIN(tmp);
	}
	return true;
}

bool CPipeProxy::OnLowSEND(const CBinaryString& bstr)
{
	int rVal{ 0 };
	CBinaryString tmp = bstr;
	CBinaryString data = tmp.UnMakeBFormat();
	rVal=m_PipeChild.Write(tmp);
	if (rVal == -1)
	{
		return false;
	}
	data.TrimFirstCodesA("\r\n");
	if (!data.empty())
	{
		if(!LowIN(data))
			return false;
	}
	return true;
}

void CPipeProxy::OnEvExeEnd()
{
	m_pBStream->Write("NOTE DISCONNECTED EXEC\r\n");
	m_PipeChild.FinPipeWork();
	CloseHandle(m_hChildDownWr);
	m_hChildDownWr = nullptr;
	CloseHandle(m_hChildUpRd);
	m_hChildUpRd = nullptr;
//	m_bEndFlag = true;
}

void CPipeProxy::OnEvSevDisconnected()
{
	m_bEndFlag = true;
}
