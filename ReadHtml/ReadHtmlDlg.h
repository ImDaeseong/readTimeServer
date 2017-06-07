#pragma once
#include "webbrowser2.h"

class CReadHtmlDlg : public CDialog
{
public:
	CReadHtmlDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_READHTML_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	

protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()

public:	
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorerSub(LPDISPATCH pDisp, VARIANT* URL);

private:
	CWebBrowser2 m_WebBrowser;
	BOOL m_bCompleted;

	void Navigate(CString strURL);
	void InitWebLoad();	
public:
	afx_msg void OnBnClickedButton2();
};
