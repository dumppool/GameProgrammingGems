// COutgoingBandwidthDialog.cpp : implementation file
//

#include "stdafx.h"
#include "PacketSniffer.h"
#include "COutgoingBandwidthDialog.h"
#include ".\coutgoingbandwidthdialog.h"
#include "PacketSnifferDlg.h"


// COutgoingBandwidthDialog dialog

IMPLEMENT_DYNAMIC(COutgoingBandwidthDialog, CDialog)
COutgoingBandwidthDialog::COutgoingBandwidthDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COutgoingBandwidthDialog::IDD, pParent)
{
	m_pParent = pParent;
	m_pString = NULL;
}

COutgoingBandwidthDialog::~COutgoingBandwidthDialog()
{
}

void COutgoingBandwidthDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COutgoingBandwidthDialog, CDialog)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// COutgoingBandwidthDialog message handlers

void COutgoingBandwidthDialog::SetBuffer(const char* szBuf)
{
	m_pString = (char*)szBuf;
}

void COutgoingBandwidthDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	if(m_pString)
	{
		CEdit* pOutput = (CEdit*)GetDlgItem(IDC_OUTBANDOUTPUT);
		pOutput->SetWindowText(m_pString);
	}
}
