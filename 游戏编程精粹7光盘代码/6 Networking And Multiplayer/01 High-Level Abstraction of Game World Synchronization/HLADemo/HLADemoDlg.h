// HLADemoDlg.h : 헤더 파일
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
// CHLADemoDlg 대화 상자
class CHLADemoDlg : public CDialog
{
// 생성
public:
	RefCount<CServer> m_server;
	RefCount<CClient> m_client;
	
	CommandType m_command;

	void AddMessageLog( CString& txt);

	CHLADemoDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_HLADEMO_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

	void SetConnectOrDisconnectButtonCaption();

// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
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

