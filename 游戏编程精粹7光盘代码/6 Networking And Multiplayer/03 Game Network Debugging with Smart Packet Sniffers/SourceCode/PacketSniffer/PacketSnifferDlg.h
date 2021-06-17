// PacketSnifferDlg.h : header file
//

#pragma once

typedef int			(*LPGETVERSION)(void);
typedef const char*	(*LPGETPROTOCOLNAME)(void);
typedef int			(*LPGETPROTOCOLPORT)(void);
typedef int			(*LPGETISEXCLUSIVEACCESS)(void);
typedef const char*	(*LPHANDLEPACKET)(void* pData, int nSize);// pass is a struct

struct t_ModuleDescriptor
{
	int			(*fnGetVersion)(void);
	const char*	(*fnGetProtocolName)(void);
	int			(*fnGetProtocolPort)(void);
	int			(*fnGetIsExclusiveAccess)(void);
	const char*	(*fnHandlePacket)(void* pData, int nSize);// pass is a struct
	HMODULE hModule;
};

typedef std::list<t_ModuleDescriptor*> ModuleDescriptorList;

// CPacketSnifferDlg dialog
class CPacketSnifferDlg : public CDialog
{
// Construction
public:
	CPacketSnifferDlg(CWnd* pParent = NULL);	// standard constructor

	void LoadPlugins();

	void HandlePacket(char* len, char* src, char* dest, char *type, int nType);
	void HandlePacket(int nIndex, bool bApplyFilter = false);

// Dialog Data
	enum { IDD = IDD_PACKETSNIFFER_DIALOG };

	void OnOK(){}
	int CalcOutgoingBandwidth(const char* szIpAddress, int nStartIdx, int nEndIndex);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	bool m_bCapturing;
	ModuleDescriptorList m_ModuleDescriptors;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	void EnumerateDevices();
	void StartCapture();
	void StopCapture();
	void ClearAll();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartstop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMClickPacketlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuSave();
	afx_msg void OnFileLoad();
	afx_msg void OnViewClearall();
	afx_msg void OnBandwidthCalculator();
	afx_msg void OnExportSpreadsheet();
	afx_msg void OnFileCloseApp();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedAutoscroll();
	afx_msg void OnClose();
};
