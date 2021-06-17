#pragma once


// COutgoingBandwidthDialog dialog

class COutgoingBandwidthDialog : public CDialog
{
	DECLARE_DYNAMIC(COutgoingBandwidthDialog)

public:
	COutgoingBandwidthDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutgoingBandwidthDialog();

// Dialog Data
	enum { IDD = IDD_OUTGOINGBANDCALC };

	void OnOK(){}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetBuffer(const char* szBuf);

	CWnd* m_pParent;
	char* m_pString;
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
