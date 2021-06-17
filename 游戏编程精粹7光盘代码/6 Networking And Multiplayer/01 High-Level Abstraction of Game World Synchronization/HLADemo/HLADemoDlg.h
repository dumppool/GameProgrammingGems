// HLADemoDlg.h : ��� ����
//

#pragma once

#include "resource.h"
#include "../MyHLA/Hla.h"
#include "afxwin.h"

class CClient;
class CServer;
class CMessage;

extern const float KnightWidth;
extern const float ViewportWidth;

enum CommandType
{
	Command_None,
	Command_MoveKnight,
	Command_ChangeKnightState,
	Command_CreateKnight,
	Command_DestroyKnight,
	Command_MoveViewport,
};
// CHLADemoDlg ��ȭ ����
class CHLADemoDlg : public CDialog
{
// ����
public:
	RefCount<CServer> m_server;
	RefCount<CClient> m_client;
	
	CommandType m_command;

	void AddMessageLog( CString& txt);

	CHLADemoDlg(CWnd* pParent = NULL);	// ǥ�� ������

// ��ȭ ���� ������
	enum { IDD = IDD_HLADEMO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ����

	void SetConnectOrDisconnectButtonCaption();

// ����
protected:
	HICON m_hIcon;

	// �޽��� �� �Լ��� �����߽��ϴ�.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHelp();
	CComboBox m_commandCtl;
	afx_msg void OnCbnSelchangeCommand();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	CButton m_connectOrNotButton;
	afx_msg void OnBnClickedConnectOrNot();
	CListBox m_messageLog;
};

