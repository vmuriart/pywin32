#ifndef __FILTER_CONTEXT_H__
#define __FILTER_CONTEXT_H__

class CFilterContext
{
public:
	CFilterContext(HTTP_FILTER_CONTEXT *pHFC, DWORD notType, void *data) :
		m_pHFC(pHFC),
                m_notificationType(notType),
                m_data(data)
	{
	}
	
	~CFilterContext()
	{
	}

	CFilterContext(const CFilterContext & rhs)
	{
		Copy(rhs);
	}

	CFilterContext & operator=(const CFilterContext & rhs)
	{
		if (this != &rhs)
			Copy(rhs);
		return *this;
	}

	// member retrieval functions

	void GetFilterData(HTTP_FILTER_CONTEXT **ppfc, DWORD *pNT, void **ppData)
	{
		if (ppfc) *ppfc = m_pHFC;
		if (pNT) *pNT = m_notificationType;
		if (ppData) *ppData = m_data;
	}

	DWORD WriteClient(LPCTSTR buffer, const int buffLen, const int reserved=0)
	{
		DWORD dwBufLen = buffLen;
		m_pHFC->WriteClient(m_pHFC, (void *) buffer, &dwBufLen, reserved);
		return dwBufLen;
	}

	BOOL AddResponseHeaders(LPSTR headers, const int reserved=0)
	{
		return m_pHFC->AddResponseHeaders(m_pHFC, headers, reserved);
	}


	bool GetServerVariable(LPCTSTR varName, LPSTR lpBuff, DWORD *pBuffSize)
	{
		BOOL bOK = m_pHFC->GetServerVariable(m_pHFC,(LPSTR) varName,lpBuff,pBuffSize);
		if (!bOK)
			*pBuffSize = 0;

		if (lpBuff[(*pBuffSize)-1]=='\0')
			(*pBuffSize)--;
		return (bOK) ? true : false;
	}
	BOOL ServerSupportFunction(enum SF_REQ_TYPE sfReq, PVOID pData, DWORD ul1, DWORD ul2)
	{
		return m_pHFC->ServerSupportFunction(m_pHFC,sfReq, pData, ul1, ul2);
	}
private:
	HTTP_FILTER_CONTEXT* m_pHFC;		// IIS filter context block
	DWORD m_notificationType;
	void *m_data;
private:

	void Copy(const CFilterContext & rhs)
	{
		m_pHFC = rhs.m_pHFC;
	}
};

#endif // __FILTER_CONTEXT_H__
