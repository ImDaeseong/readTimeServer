#include "stdafx.h"
#include "ReadHtml.h"
#include "ReadHtmlDlg.h"
#include <comdef.h>
#include <mshtml.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CReadHtmlDlg::CReadHtmlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReadHtmlDlg::IDD, pParent)
{
	m_bCompleted = FALSE;
}

void CReadHtmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_SUB, m_WebBrowser);
}

BEGIN_MESSAGE_MAP(CReadHtmlDlg, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CReadHtmlDlg::OnBnClickedButton1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON2, &CReadHtmlDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

BOOL CReadHtmlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	SetDlgItemText(IDC_EDIT1, "naver.com");

	return TRUE;  
}

void CReadHtmlDlg::OnPaint()
{
	CPaintDC dc(this);
}

void CReadHtmlDlg::Navigate(CString strURL)
{		
	VARIANT vtHeader, vtTarget, vtEmpty; 
	CString strHeader, strTarget; 
	
    strHeader = "Content-Type: application/x-www-form-urlencoded\r\n"; 
	
    strTarget = "_parent"; 

	::VariantInit(&vtHeader); 
	::VariantInit(&vtTarget); 
	::VariantInit(&vtEmpty); 
	
	vtHeader.vt = VT_BSTR; 
	
	vtHeader.bstrVal = strHeader.AllocSysString(); 
	
	vtTarget.vt = VT_BSTR; 
	
	vtTarget.bstrVal = strTarget.AllocSysString(); 
	
    m_WebBrowser.Navigate(strURL, &vtEmpty, &vtTarget, &vtEmpty, &vtHeader);

	SysFreeString(vtHeader.bstrVal);
	SysFreeString(vtTarget.bstrVal);
}

BEGIN_EVENTSINK_MAP(CReadHtmlDlg, CDialog)
	ON_EVENT(CReadHtmlDlg, IDC_EXPLORER_SUB, 259, CReadHtmlDlg::DocumentCompleteExplorerSub, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

CString GetSplitHtml(CString strInput)
{
	CString  strBuf = strInput;

	CString  strKey;
	CString first = "<DIV id=time_area";
	CString end = "</DIV>";
	int nTagLength = 6;
	
	int  Pos1 = 0;
    int  Pos2 = 0;		
	
	CString  strTemp = strBuf;

	Pos1 = strTemp.Find( first, Pos1 );
	Pos2 = strTemp.Find( end, Pos1 + 1);

	if(Pos1 == -1 && Pos2 == -1)
		return "";

	strKey = strBuf.Mid(Pos1, (Pos2 - Pos1 + nTagLength) );
	strBuf.Replace(strKey, "");
	strKey.TrimLeft();
	strKey.TrimRight();

	return strKey;
}

CString GetTimeData(CString strInput)
{		
	CString strOutput(_T(""));
	int nCountLeft = strInput.Find('>');
	int nCountRight = strInput.ReverseFind('<');
	if(nCountRight>nCountLeft)
	{
		strOutput = strInput.Mid(nCountLeft+1, nCountRight - nCountLeft - 1);
		return strOutput;
	}	
	return strOutput;
}

void CReadHtmlDlg::DocumentCompleteExplorerSub(LPDISPATCH pDisp, VARIANT* URL)
{
	if(m_bCompleted) return;

	IHTMLDocument2*	pHTMLDocument2;
	LPDISPATCH		lpDispatch;
	lpDispatch =	m_WebBrowser.GetDocument();

    if (lpDispatch)
	{
		HRESULT hr;
		hr = lpDispatch->QueryInterface(IID_IHTMLDocument2, (LPVOID*) &pHTMLDocument2);
		lpDispatch->Release();

		if (FAILED(hr))
			return;

		if (pHTMLDocument2 == NULL)
			return;

		IHTMLElement* pBody;
		hr = pHTMLDocument2->get_body(&pBody);

		if (FAILED(hr))
			return;

		if (pBody == NULL)
			return;

		BSTR bstr;
		pBody->get_innerHTML(&bstr);
		
		CString csSourceCode(bstr);
		
		CString sHtml = GetSplitHtml(csSourceCode);
		if(sHtml != "")
		{
			sHtml = GetTimeData(sHtml);
			if(sHtml != "")
			{
				m_bCompleted = TRUE;
				SetDlgItemText(IDC_EDIT2, sHtml);
				//OutputDebugString(sHtml);
			}
		}
		SysFreeString(bstr);
		pBody->Release();
	}
}

void CReadHtmlDlg::OnBnClickedButton1()
{
	CString sEditUrl = "";
	GetDlgItemText(IDC_EDIT1, sEditUrl);

	if(sEditUrl == "") return;


	KillTimer(1);
    SetTimer(1, 1000, NULL);
	InitWebLoad();
}

void CReadHtmlDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (m_bCompleted)
    {
        KillTimer(1);
        SetTimer(1, 1000, NULL);
		InitWebLoad();
    }

	CDialog::OnTimer(nIDEvent);
}

void CReadHtmlDlg::InitWebLoad()
{
	CString sEditUrl = "";
	GetDlgItemText(IDC_EDIT1, sEditUrl);

	m_bCompleted = FALSE;
	CString sUrl;
	sUrl.Format("https://time.navyism.com/?host=%s", sEditUrl);
	Navigate(sUrl);
}

void CReadHtmlDlg::OnBnClickedButton2()
{
	CDialog::OnOK();
}
