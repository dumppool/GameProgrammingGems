// PacketSnifferDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PacketSniffer.h"
#include "PacketSnifferDlg.h"
#include "COutgoingBandwidthDialog.h"

#include "pcap.h"
#include <vector>
#include <string>
#include <map>
#include ".\packetsnifferdlg.h"

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "net_headers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifndef _DEBUG
	#include "crashhandler.h"
	CrashHandler crash(true);
#endif

#define SAVE_VERSION 1
#define SAVE_VERSION_STRING "1"

//Globals
namespace
{
	std::vector<std::string> device_list;
	pcap_t *g_pCap = NULL;
	char errbuf[PCAP_ERRBUF_SIZE];

	CPacketSnifferDlg *g_App = NULL;
	CListBox* g_pList = NULL;
	CListCtrl* g_pListCtrl = NULL;

	typedef std::vector<unsigned char> ByteVector;
	typedef std::vector<ByteVector> PacketDataVector;
	PacketDataVector g_PacketData;

	typedef std::vector<int> PacketTypeIgnoreVector;
	PacketTypeIgnoreVector g_PacketTypeIgnores;

	char g_szGlobalBuf[1024*4];

	bool g_bAutoScroll = false;

	typedef struct t_PacketInfo
	{
		unsigned int nFrame;
		char szTime[16];
		unsigned int nLength;
		char szSource[16];
		unsigned int nSrcPort;
		char szDest[16];
		unsigned int nDestPort;
	} PacketInfo;

	typedef std::vector<PacketInfo> PacketInfoVector;
	PacketInfoVector g_PacketInfo;
}

#define TIMER_ID 10001

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPacketSnifferDlg dialog



CPacketSnifferDlg::CPacketSnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPacketSnifferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bCapturing = false;

	g_App = this;
}

void CPacketSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPacketSnifferDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_STARTSTOP, OnBnClickedStartstop)
	ON_WM_TIMER()

ON_NOTIFY(NM_CLICK, IDC_PACKETLIST, OnNMClickPacketlist)
ON_COMMAND(ID_Menu, OnMenuSave)
ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
ON_COMMAND(ID_VIEW_CLEARALL, OnViewClearall)
ON_COMMAND(ID_TOOLS_BANDWIDTHCALCULATOR, OnBandwidthCalculator)
ON_COMMAND(ID_TOOLS_EXPORTSPREADSHEET, OnExportSpreadsheet)
ON_COMMAND(ID_FILE_CLOSE32771, OnFileCloseApp)
ON_WM_SIZE()
ON_BN_CLICKED(IDC_AUTOSCROLL, OnBnClickedAutoscroll)
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPacketSnifferDlg message handlers

BOOL CPacketSnifferDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	LoadPlugins();

	CButton* pCheck = (CButton*)GetDlgItem(IDC_CAPTUREMODE);
	pCheck->SetCheck(1);
	
	g_pList = (CListBox*)GetDlgItem(IDC_PACKETDATA);

	g_pListCtrl = (CListCtrl*)GetDlgItem(IDC_PACKETLIST);
	g_pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT);

	g_pListCtrl->InsertColumn(0, "Frame", LVCFMT_LEFT, 60);
	g_pListCtrl->InsertColumn(1, "Time", LVCFMT_LEFT, 80);
	g_pListCtrl->InsertColumn(2, "Length", LVCFMT_LEFT, 80);
	g_pListCtrl->InsertColumn(3, "Source", LVCFMT_LEFT, 125);
	g_pListCtrl->InsertColumn(4, "Source Port", LVCFMT_LEFT, 100);
	g_pListCtrl->InsertColumn(5, "Destination", LVCFMT_LEFT, 125);
	g_pListCtrl->InsertColumn(6, "Dest Port", LVCFMT_LEFT, 125);
	g_pListCtrl->InsertColumn(7, "Type", LVCFMT_LEFT, 80);
	g_pListCtrl->InsertColumn(8, "Game Msg", LVCFMT_LEFT, 150);

	//Enumerate our network adapters.
	EnumerateDevices();

	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_FILTER);
	pEdit->SetWindowText("udp");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPacketSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPacketSnifferDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPacketSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPacketSnifferDlg::EnumerateDevices()
{
	pcap_if_t *alldevs;
	pcap_if_t *d;

	if(pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		MessageBox("No network adapters detected!", "Error", MB_OK);
	}

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DEVICELIST);
	for(d=alldevs; d; d=d->next)
	{		
		if(pCombo == NULL)
		{
			break;
		}

		if(d->name)
		{
			device_list.push_back(d->name);
		}

		if (d->description)
		{
			pCombo->AddString(d->description);
		}
		else
		{
			pCombo->AddString(d->name);
		}
	}
	
	pCombo->SetCurSel(0);

	pcap_freealldevs(alldevs);
}

void CPacketSnifferDlg::LoadPlugins()
{
	struct _finddata_t file;
	intptr_t hFile;

	if( (hFile = _findfirst( "plug-ins\\*.dll", &file )) != -1L )
	{
		/* Find the rest of the files */
		do 
		{
			//ok.. load it and store it...
			char szBuf[1024];
			sprintf(szBuf, "Found: %s, size:%d\n", file.name, file.size);
			OutputDebugString(szBuf);

			std::string filename = "plug-ins\\";
			filename += file.name;

			HMODULE hModule = LoadLibrary(filename.c_str());
			if(hModule)
			{
				t_ModuleDescriptor* pModuleDescriptor = new t_ModuleDescriptor;
				if(pModuleDescriptor)
				{
					// We need to keep this handle so we can close it later.
					pModuleDescriptor->hModule = hModule;

					pModuleDescriptor->fnGetVersion = (LPGETVERSION)GetProcAddress(hModule, "fnGetVersion");
					pModuleDescriptor->fnGetProtocolName = (LPGETPROTOCOLNAME)GetProcAddress(hModule, "fnGetProtocolName");
					pModuleDescriptor->fnGetProtocolPort = (LPGETPROTOCOLPORT)GetProcAddress(hModule, "fnGetProtocolPort");
					pModuleDescriptor->fnGetIsExclusiveAccess = (LPGETISEXCLUSIVEACCESS)GetProcAddress(hModule, "fnGetIsExclusiveAccess");
					pModuleDescriptor->fnHandlePacket = (LPHANDLEPACKET)GetProcAddress(hModule, "fnHandlePacket");

					int nVersion = pModuleDescriptor->fnGetVersion();

					m_ModuleDescriptors.push_back(pModuleDescriptor);
				}
			}
		}while( _findnext( hFile, &file ) == 0 );

		_findclose( hFile );
	}
	else
	{
		//else we didn't find any...
	}
}

void CPacketSnifferDlg::StartCapture()
{
	u_int netmask;
	//TODO: grab filter from CEdit! (validate it first)
	char packet_filter[] = "udp";
	struct bpf_program fcode;

	//get game message ignore filter (not used currently)
#if 0
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_FILTER);
	if(pEdit)
	{
		g_PacketTypeIgnores.clear();

		char szBuf[512];
		memset(szBuf, 0, 512);
		pEdit->GetWindowText(szBuf, 511);

		char* pTok = strtok(szBuf, ",");
		while(pTok)
		{
			int nType = atoi(pTok);
			g_PacketTypeIgnores.push_back(nType);

			pTok = strtok(NULL, ",");
		}
	}
#endif

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_DEVICELIST);
	if(pCombo)
	{
		int nIdx = pCombo->GetCurSel();

		CString sString;
		pCombo->GetLBText(nIdx, sString);
		char sText[256];
		memset(sText, 0, 256);
		strncpy(sText, sString.GetBuffer(), 255);
		sString.ReleaseBuffer();

		//Ok use this 
#ifdef _DEBUG
		char szBuf[512];
		sprintf(szBuf, "Name: %s\n", device_list[nIdx].c_str());
		OutputDebugString(szBuf);
#endif

		CButton* pCheck = (CButton*)GetDlgItem(IDC_CAPTUREMODE);
		int bProm = pCheck->GetCheck();

			/* Open the adapter */
			if ((g_pCap= pcap_open_live(device_list[nIdx].c_str(),	// name of the device
				65536,			// portion of the packet to capture. 
				// 65536 grants that the whole packet will be captured on all the MACs.
				bProm,			// promiscuous mode (nonzero means promiscuous)
				10,			// read timeout
				errbuf			// error buffer
				)) == NULL)
			{
				MessageBox("Failed to open the adapter.\n", "Error", MB_OK);
				return;
			}

			/* Check the link layer. We support only Ethernet for simplicity. */
			if(pcap_datalink(g_pCap) != DLT_EN10MB)
			{
				MessageBox("Only supports ethernet networks.\n", "Error", MB_OK);
				return;
			}

			{
				pcap_if_t *alldevs;
				pcap_if_t *d;

				if(pcap_findalldevs(&alldevs, errbuf) == -1)
				{
					MessageBox("No network adapters detected!", "Error", MB_OK);
				}

				for(d=alldevs; d; d=d->next)
				{
					if(_stricmp(d->name, device_list[nIdx].c_str()) == 0)
					{	
						if(d->addresses != NULL)
						{
							/* Retrieve the mask of the first address of the interface */
							netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
						}
						else
						{	/* If the interface is without addresses we suppose to be in a C class network */
							netmask=0xffffff; 
						}
						break;
					}
				}

				pcap_freealldevs(alldevs);
			}

			//compile the filter
			if (pcap_compile(g_pCap, &fcode, packet_filter, 1, netmask) <0 )
			{
				MessageBox("Unable to compile the packet filter. Check the syntax.", "Error", MB_OK);
				return;
			}

			//set the filter
			if (pcap_setfilter(g_pCap, &fcode)<0)
			{
				MessageBox("Error setting the filter.", "Error", MB_OK);
				return;
			}

			SetTimer(TIMER_ID, 250, 0);
	}
}

void CPacketSnifferDlg::StopCapture()
{
	if(g_pCap)
	{
		KillTimer(TIMER_ID);
		pcap_close(g_pCap);
		g_pCap= NULL;
	}
}
void CPacketSnifferDlg::OnBnClickedStartstop()
{
	CButton *pButton = (CButton*)GetDlgItem(IDC_STARTSTOP);
	if(pButton == NULL)
	{
		return;
	}

	m_bCapturing = !m_bCapturing;

	if(m_bCapturing)
	{
		pButton->SetWindowText("Stop");
		StartCapture();
	}
	else
	{
		pButton->SetWindowText("Start");
		StopCapture();
	}

}

void CPacketSnifferDlg::HandlePacket(char* len, char* src, char* dest, char *type, int nType)
{
	int nIndex = g_pListCtrl->GetItemCount();
	g_pListCtrl->InsertItem(nIndex, "Item");

	char szBuf[32];
	sprintf(szBuf, "%d", nIndex);
	g_pListCtrl->SetItemText(nIndex, 0, szBuf);
	g_pListCtrl->SetItemText(nIndex, 1, len);
	g_pListCtrl->SetItemText(nIndex, 2, src);
	g_pListCtrl->SetItemText(nIndex, 3, dest);
	g_pListCtrl->SetItemText(nIndex, 4, "UNKNOWN");
	g_pListCtrl->SetItemText(nIndex, 5, "???");

	if(g_bAutoScroll)
	{
		CRect ItemRect;
		g_pListCtrl->GetSubItemRect(0, 0, LVIR_BOUNDS, ItemRect);
		int nYDelta = ItemRect.bottom - ItemRect.top;
		g_pListCtrl->Scroll( CSize(0, nYDelta) );
	}

}

void CPacketSnifferDlg::HandlePacket(int nIndex, bool bApplyFilter)
{
	if((nIndex >= (int)g_PacketInfo.size()) || (nIndex < 0))
	{
		return;
	}

	int nPacketLen = (int)g_PacketData.size();


	unsigned char *pkt_data = (unsigned char *)&((g_PacketData[nIndex])[0]);
	
	/* print timestamp and length of the packet */
	//	printf("%s.%.6d len:%d ", timestr, header->ts.tv_usec, header->len);

	// Filter the packet if needbe...
#if 0
	if(bApplyFilter)
	{
		int nNumPacketIgnoreTypes = g_PacketTypeIgnores.size();
		for(int i = 0; i < nNumPacketIgnoreTypes; ++i)
		{
			if(nGamePacketType == g_PacketTypeIgnores[i])
			{
				return;
			}
		}
	}
#endif

#if 0
	char szBufTemp[32];
	sprintf(szBufTemp, "Type: %d\n", nGamePacketType);
	OutputDebugString(szBufTemp);
#endif

	int nDataLen = nPacketLen;

	int nTmpIndex = g_pListCtrl->GetItemCount();
	g_pListCtrl->InsertItem(nTmpIndex, "Item");

	char szBuf[32];
	sprintf(szBuf, "%d", g_PacketInfo[nIndex].nFrame);
	g_pListCtrl->SetItemText(nTmpIndex, 0, szBuf);

	sprintf(szBuf, "%s", g_PacketInfo[nIndex].szTime);
	g_pListCtrl->SetItemText(nTmpIndex, 1, szBuf);

	sprintf(szBuf, "%d", g_PacketInfo[nIndex].nLength);
	g_pListCtrl->SetItemText(nTmpIndex, 2, szBuf);

	sprintf(szBuf, "%s", g_PacketInfo[nIndex].szSource);
	g_pListCtrl->SetItemText(nTmpIndex, 3, szBuf);

	sprintf(szBuf, "%d", g_PacketInfo[nIndex].nSrcPort);
	g_pListCtrl->SetItemText(nTmpIndex, 4, szBuf);

	sprintf(szBuf, "%s", g_PacketInfo[nIndex].szDest);
	g_pListCtrl->SetItemText(nTmpIndex, 5, szBuf);

	sprintf(szBuf, "%d", g_PacketInfo[nIndex].nDestPort);
	g_pListCtrl->SetItemText(nTmpIndex, 6, szBuf);
	
	//Loop our protocol handler plug-ins to handle this data
	{
		ModuleDescriptorList::iterator iter = m_ModuleDescriptors.begin();
		while(iter != m_ModuleDescriptors.end())
		{
			int nPluginPort = (*iter)->fnGetProtocolPort();
			if((nPluginPort == g_PacketInfo[nIndex].nSrcPort) || (nPluginPort == g_PacketInfo[nIndex].nDestPort))
			{
				//handle it...
				const char* pszProtocol = (*iter)->fnGetProtocolName();
				g_pListCtrl->SetItemText(nTmpIndex, 7, pszProtocol);

				int nTotalSize = (int)g_PacketData[nIndex].size();

				unsigned char* pkt_data = (unsigned char*)&(g_PacketData[nIndex][0]);
				ip_header* ih = (ip_header *) (pkt_data + sizeof(ethernet_frame_header)); //length of Ethernet header

				/* retrieve the position of the udp header */
				u_int len_mask = (ih->ver_ihl & 0xf);
				u_int ip_len = len_mask * 4;

				int nDataPos = sizeof(ethernet_frame_header) + ip_len + sizeof(udp_header);
				int nDataSize = nTotalSize - nDataPos;

				int nIdx = nDataPos;
				unsigned char* pData = (unsigned char*)&(g_PacketData[nIndex][nIdx]);

				const char* pszResult = (*iter)->fnHandlePacket(pData, nDataSize);

				g_pListCtrl->SetItemText(nTmpIndex, 8, pszResult );
				break;
			}

			++iter;
		}
	}

	if(g_bAutoScroll)
	{
		CRect ItemRect;
		g_pListCtrl->GetSubItemRect(0, 0, LVIR_BOUNDS, ItemRect);
		int nYDelta = ItemRect.bottom - ItemRect.top;
		g_pListCtrl->Scroll( CSize(0, nYDelta) );
	}
}

void CPacketSnifferDlg::OnTimer(UINT nIDEvent)
{	
	if(g_pCap)
	{
		pcap_dispatch(g_pCap, 0, packet_handler, NULL);
	}
}

void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	struct tm *ltime;
	char timestr[16];
	ip_header *ih;
	udp_header *uh;
	u_int ip_len;
	u_short sport,dport;

	/* convert the timestamp to readable format */
	__time64_t long_time = header->ts.tv_sec;
	ltime = localtime(&long_time);
	strftime( timestr, sizeof timestr, "%H:%M:%S", ltime);

	/* retrieve the position of the ip header */
	ih = (ip_header *) (pkt_data + 14); //length of Ethernet header

	/* retrieve the position of the udp header */
	ip_len = (ih->ver_ihl & 0xf) * 4;
	uh = (udp_header *) ((u_char*)ih + ip_len);

	/* convert from network byte order to host byte order */
	sport = ntohs( uh->sport );
	dport = ntohs( uh->dport );

	/* print ip addresses and udp ports */
#if 0
	char szBuf[2048];
	sprintf(szBuf, "src: %d.%d.%d.%d\n", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3,	ih->saddr.byte4);
	OutputDebugString(szBuf);
	sprintf(szBuf, "dst: %d.%d.%d.%d\n", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
	OutputDebugString(szBuf);
	sprintf(szBuf, "prt: s:%d d:%d\n", sport, dport);
	OutputDebugString(szBuf);
#endif

	/*
	//We only care about packets that our protocol handlers can handle
	bool bHandlePacket = false;
	ModuleDescriptorList::iterator iter = m_ModuleDescriptors.begin();
	while(iter != m_ModuleDescriptors.end())
	{
		int nPluginPort = (*iter)->fnGetProtocolPort();
		if((sport == nPluginPort) || (dport == nPluginPort))
		{
			bHandlePacket = true;
		}
	}
	
	//None of our packet handlers want this one, ignore it.
	if(!bHandlePacket)
	{
		return;
	}
	*/

	PacketInfo packetInfo;
	packetInfo.nFrame = (unsigned int)g_PacketInfo.size();
	sprintf(packetInfo.szTime, "%s.%.6d", timestr, header->ts.tv_usec);  
	packetInfo.nLength = header->len; //total len..
	sprintf(packetInfo.szSource, "%d.%d.%d.%d", ih->saddr.byte1, ih->saddr.byte2, ih->saddr.byte3,	ih->saddr.byte4);
	packetInfo.nSrcPort = sport;
	sprintf(packetInfo.szDest, "%d.%d.%d.%d", ih->daddr.byte1, ih->daddr.byte2, ih->daddr.byte3, ih->daddr.byte4);
	packetInfo.nDestPort = dport;

	g_PacketInfo.push_back(packetInfo);

	//save the data
	ByteVector data;
	data.reserve(header->len);

	//copy the data
	for(bpf_u_int32 i = 0; i < header->len; ++i)
	{
		data.push_back(pkt_data[i]);
	}

	g_PacketData.push_back(data);

	g_App->HandlePacket((int)g_PacketData.size() - 1);
}

void CPacketSnifferDlg::OnNMClickPacketlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	//get the selection
	int nIndex = g_pListCtrl->GetNextItem(-1, LVIS_SELECTED);
	
	if((nIndex >= (int)g_PacketData.size()) || (nIndex < 0) )
	{
		// make sure we don't go out of bounds
		return;
	}

	//empty the list
	g_pList->ResetContent();

	//Parse the data and fill our hex window.

	//push it onto the list.
	int nLen = (int)(g_PacketData[nIndex]).size();

	char szOutBuf[4*1024];
	memset(szOutBuf, 0, 4*1024);
	char szSmallBuf[16];

	int nLines = nLen / 16;
	int nBytesRem = nLen % 16;

	int nCharIdx = 0;
	for(int line = 0; line < nLines; ++line)
	{
		memset(szOutBuf, 0, 4*1024);
		sprintf(szSmallBuf, "%04x  ", line*16);
		strcat(szOutBuf, szSmallBuf);

		char szSymbol[16];
		for(int j = 0; j < 8; ++j)
		{
			sprintf(szSmallBuf, "%02x ", (g_PacketData[nIndex])[nCharIdx]);
			strcat(szOutBuf, szSmallBuf);

			szSymbol[j] = isprint( (int)(g_PacketData[nIndex])[nCharIdx] ) ? (g_PacketData[nIndex])[nCharIdx]:'.';

			++nCharIdx;
		}

		strcat(szOutBuf, " ");

		for(int k = 0; k < 8; ++k)
		{
			sprintf(szSmallBuf, "%02x ", (g_PacketData[nIndex])[nCharIdx]);
			strcat(szOutBuf, szSmallBuf);

			szSymbol[k+8] = isprint( (int)(g_PacketData[nIndex])[nCharIdx]) ? (g_PacketData[nIndex])[nCharIdx]:'.';

			++nCharIdx;
		}

		for(int l = 0; l < 16; ++l)
		{
			char szTempBuf[2];
			sprintf(szTempBuf, "%c", szSymbol[l]);
			strcat(szOutBuf, szTempBuf);
			if(l == 7)
			{
				strcat(szOutBuf, " ");
			}
		}

		g_pList->AddString(szOutBuf);		
	}

	//now do the remainder
	memset(szOutBuf, 0, 4*1024);
	if(nBytesRem > 0)
	{
		sprintf(szSmallBuf, "%04x  ", (nLines)*16);
		strcat(szOutBuf, szSmallBuf);

		//now loop it
		char szSymbol[16];
		for(int j = 0; j < 16; ++j)
		{
			if(j >= nBytesRem)
			{
				strcat(szOutBuf, "   ");

				if(j == 7)
				{
					strcat(szOutBuf, " ");
				}

				continue;
			}

			sprintf(szSmallBuf, "%02x ", (g_PacketData[nIndex])[nCharIdx]);
			strcat(szOutBuf, szSmallBuf);

			szSymbol[j] = isprint( (int)(g_PacketData[nIndex])[nCharIdx] ) ? (g_PacketData[nIndex])[nCharIdx]:'.';

			if(j == 7)
			{
				strcat(szOutBuf, " ");
			}

			++nCharIdx;
		}

		for(int l = 0; l < nBytesRem; ++l)
		{
			char szTempBuf[2];
			sprintf(szTempBuf, "%c", szSymbol[l]);
			strcat(szOutBuf, szTempBuf);
			if(l == 7)
			{
				strcat(szOutBuf, " ");
			}
		}

		g_pList->AddString(szOutBuf);
	}

	*pResult = 0;
}

void CPacketSnifferDlg::OnMenuSave()
{
	CFileDialog dialog(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT,"Sniffer Files (*.xsnf)|*.xsnf||"); //|Comma Separated Values(*.csv)|*.csv||
	int iRet = (int)dialog.DoModal();
	CString l_strFileName;
	l_strFileName = dialog.GetPathName();

	char szFilename[1024];

	if(iRet != IDOK)
	{
		return;
	}

	char* s = l_strFileName.GetBuffer();
	strncpy(szFilename, s, 1023);

	// Make sure it ends in .xsnf
	char* pStrstr = strstr(s, ".xsnf");
	if(!pStrstr)
	{
		strcat(szFilename, ".xsnf");
	}

	//XML test
	xmlDocPtr doc = NULL;       /* document pointer */
	xmlNodePtr root_node = NULL, node = NULL, node2 = NULL, node3 = NULL;/* node pointers */

	doc = xmlNewDoc(BAD_CAST "1.0");
	root_node = xmlNewNode(NULL, BAD_CAST "root");
	xmlDocSetRootElement(doc, root_node);


	xmlNewChild(root_node, NULL, BAD_CAST "version", BAD_CAST SAVE_VERSION_STRING);

	node = xmlNewChild(root_node, NULL, BAD_CAST "packets", NULL);

	char szBuf[8 * 1024];

	//for packets...
	for(size_t i = 0; i < g_PacketInfo.size(); ++i)
	{
		node2 = xmlNewChild(node, NULL, BAD_CAST "packet", NULL);
		
		sprintf(szBuf, "%d", g_PacketInfo[i].nFrame);
		xmlNewChild(node2, NULL, BAD_CAST "frame", BAD_CAST szBuf);

		sprintf(szBuf, "%s", g_PacketInfo[i].szTime);
		xmlNewChild(node2, NULL, BAD_CAST "time", BAD_CAST szBuf);

		sprintf(szBuf, "%d", g_PacketInfo[i].nLength);
		xmlNewChild(node2, NULL, BAD_CAST "length", BAD_CAST szBuf);

		sprintf(szBuf, "%s", g_PacketInfo[i].szSource);
		xmlNewChild(node2, NULL, BAD_CAST "source", BAD_CAST szBuf);

		sprintf(szBuf, "%d", g_PacketInfo[i].nSrcPort);
		xmlNewChild(node2, NULL, BAD_CAST "sport", BAD_CAST szBuf);

		sprintf(szBuf, "%s", g_PacketInfo[i].szDest);
		xmlNewChild(node2, NULL, BAD_CAST "dest", BAD_CAST szBuf);

		sprintf(szBuf, "%d", g_PacketInfo[i].nDestPort);
		xmlNewChild(node2, NULL, BAD_CAST "dport", BAD_CAST szBuf);
		
		szBuf[0] = '\0';
		for(size_t j = 0; j < g_PacketData[i].size(); ++j)
		{
			char szTempBuf[16];
			sprintf(szTempBuf, "%d,", g_PacketData[i][j]);
			strcat(szBuf, szTempBuf);
		}
		
		xmlNewChild(node2, NULL, BAD_CAST "data", BAD_CAST szBuf);
	}

	/* 
	* Dumping document to stdio or file
	*/
	xmlSaveFormatFileEnc(szFilename, doc, "UTF-8", 1);

	/*free the document */
	xmlFreeDoc(doc);

	/*
	*Free the global variables that may
	*have been allocated by the parser.
	*/
	xmlCleanupParser();

	l_strFileName.ReleaseBuffer();
}

// 
// 0 = summary data
// 1 = raw packet data
//

void CPacketSnifferDlg::OnFileLoad()
{
	//CFileDialog dialog;
	CFileDialog dialog(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,"Sniffer Files (*.xsnf)|*.xsnf||"); //|Comma Separated Values(*.csv)|*.csv||
	int iRet = (int)dialog.DoModal();
	CString l_strFileName;
	l_strFileName = dialog.GetPathName();

	if(iRet != IDOK)
	{
		return;
	}

	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	unsigned int nSaveVersion;

	char* s = l_strFileName.GetBuffer();

	doc = xmlReadFile(s, NULL, 0);

	if(doc)
	{
		// Clear all data and buffers
		ClearAll();

		/*Get the root element node */
		root_element = xmlDocGetRootElement(doc);

		xmlNode *cur_node = NULL;

		for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) 
		{
			{
				if(cur_node->name[0] == '\0')
				{
					continue;
				}
				//if version
				if(_strcmpi( (char*)cur_node->name, "version") == 0)
				{
					if(cur_node->children->content)
					{
						//FIXME: do something with this value
						nSaveVersion = atoi((char*)cur_node->children->content);
					}
				}

				//if packet
				if(_strcmpi((char*)cur_node->name, "packets") == 0)
				{

					if(cur_node->children)
					{
						xmlNode *child_node = NULL;
						for(child_node = cur_node->children; child_node; child_node = child_node->next)
						{
							if(_strcmpi((char*)child_node->name, "packet") == 0)
							{

								PacketInfo packetInfo;
								memset(&packetInfo, 0, sizeof(PacketInfo));

								xmlNode *packet_node = NULL;
								for(packet_node = child_node->children; packet_node; packet_node = packet_node->next)
								{
									if(_strcmpi((char*)packet_node->name, "frame") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											packetInfo.nFrame = atoi((char*)packet_node->children->content);
										}
									}
									if(_strcmpi((char*)packet_node->name, "time") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											strncpy(packetInfo.szTime, (char*)packet_node->children->content, 15);
										}
									}
									if(_strcmpi((char*)packet_node->name, "length") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											packetInfo.nLength = atoi((char*)packet_node->children->content);
										}
									}
									if(_strcmpi((char*)packet_node->name, "source") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											strncpy(packetInfo.szSource, (char*)packet_node->children->content, 15);
										}
									}
									if(_strcmpi((char*)packet_node->name, "sport") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											packetInfo.nSrcPort = atoi((char*)packet_node->children->content);
										}
									}
									if(_strcmpi((char*)packet_node->name, "dest") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											strncpy(packetInfo.szDest, (char*)packet_node->children->content, 15);
										}
									}
									if(_strcmpi((char*)packet_node->name, "dport") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											packetInfo.nDestPort = atoi((char*)packet_node->children->content);
										}
									}
									if(_strcmpi((char*)packet_node->name, "data") == 0)
									{
										if((packet_node->children)&& (packet_node->children->content))
										{
											//loop it
											ByteVector bytes;

											char* pTok = strtok((char*)packet_node->children->content, ",");
											while(pTok)
											{
												unsigned char nVal = atoi(pTok);
												bytes.push_back(nVal);
												pTok = strtok(NULL, ",");
											}

											g_PacketData.push_back(bytes);

										}
									}
								}
								g_PacketInfo.push_back(packetInfo);
							}							
						}
					}
				}
			}
		}

		/*free the document */
		xmlFreeDoc(doc);

		/*
		*Free the global variables that may
		*have been allocated by the parser.
		*/
		xmlCleanupParser();

	}
	else
	{
		OutputDebugString("Error opening file....!\n");
	}


	l_strFileName.ReleaseBuffer();

	//ok, now re-parse all the data for display
	size_t nIterations = g_PacketData.size();
	for(size_t i = 0; i < nIterations; ++i)
	{
		HandlePacket((int)i);
	}

}

void CPacketSnifferDlg::ClearAll()
{
	//clear the list control.
	g_pListCtrl->DeleteAllItems();

	//clear the list.
	g_pList->ResetContent();

	g_PacketInfo.clear();

	g_PacketData.clear();
}
void CPacketSnifferDlg::OnViewClearall()
{
	ClearAll();
}

//Not used currently
typedef std::map<std::string, int> BandwidthMap;
void CPacketSnifferDlg::OnBandwidthCalculator()
{
	char szBuffer[4096];
	szBuffer[0] = '\0';

	std::string sOutputBuffer;

	BandwidthMap m_OutgoingMap;
	BandwidthMap m_IncomingMap;

	POSITION pos = g_pListCtrl->GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		OutputDebugString("No items were selected!\n");
	}
	else
	{
		while (pos)
		{
			int nItem = g_pListCtrl->GetNextSelectedItem(pos);
			int nSize = (int)g_PacketData[nItem].size();

			//look up the source in our outgoing map
			BandwidthMap::iterator iter = m_OutgoingMap.find(g_PacketInfo[nItem].szSource);
			if(iter != m_OutgoingMap.end()) //found
			{
				//inc the count
				iter->second += nSize;

			}else //not found
			{
				//add a new entry
				m_OutgoingMap[g_PacketInfo[nItem].szSource] = nSize;
			}

			//look up the dest in our outgoing map
			iter = m_IncomingMap.find(g_PacketInfo[nItem].szDest);
			if(iter != m_IncomingMap.end()) //found
			{
				//inc the count
				iter->second += nSize;

			}else //not found
			{
				//add a new entry
				m_IncomingMap[g_PacketInfo[nItem].szDest] = nSize;
			}

		}
	}

	sOutputBuffer += "Outgoing:\r\n";

	//now give it up!
	BandwidthMap::iterator iter = m_OutgoingMap.begin();
	while(iter != m_OutgoingMap.end())
	{
		char szBuf[128];
		sprintf(szBuf, "%s = %d bytes.\r\n", iter->first.c_str(), iter->second);
		OutputDebugString(szBuf);
		sOutputBuffer += szBuf;
		++iter;
	}

	sOutputBuffer += "Incoming:\r\n";

	iter = m_IncomingMap.begin();
	while(iter != m_IncomingMap.end())
	{
		char szBuf[128];
		sprintf(szBuf, "%s = %d bytes.\r\n", iter->first.c_str(), iter->second);
		OutputDebugString(szBuf);
		sOutputBuffer += szBuf;
		++iter;
	}

	COutgoingBandwidthDialog dialog;
	dialog.SetBuffer(sOutputBuffer.c_str());
	dialog.DoModal();

}

//Not used currently
void CPacketSnifferDlg::OnExportSpreadsheet()
{
	CFileDialog dialog(FALSE,NULL,NULL,OFN_OVERWRITEPROMPT,"Text (*.txt)|*.txt||");
	int iRet = (int)dialog.DoModal();
	CString l_strFileName;
	l_strFileName = dialog.GetPathName();

	char szFilename[1024];

	if(iRet != IDOK)
	{
		return;
	}

	char* s = l_strFileName.GetBuffer();
	strncpy(szFilename, s, 1023);

	char* pStrstr = strstr(s, ".txt");
	if(!pStrstr)
	{
		strcat(szFilename, ".txt");
	}

	//do it!
	int nIndex = g_pListCtrl->GetNextItem(-1, LVIS_SELECTED);
	if((nIndex < (int)g_PacketInfo.size()) &&( nIndex > -1))
	{
		FILE* fp = fopen(szFilename, "w");
		if(!fp)
		{
			l_strFileName.ReleaseBuffer();
			return; 
		}

		char szIp[16] = "\0";
		char szTime[16] = "\0";
		int nByteCount = 0;

		//grab the source.. only parse packets with this src ip
		strncpy(szIp, g_PacketInfo[nIndex].szSource, 15);

		//now get the time of the first...
		char *pTime = g_PacketInfo[0].szTime;
		char *pTok = strtok(pTime, ".");
		strncpy(szTime, pTok, 15);

		BandwidthMap m_OutgoingMap;
		//now parse it from top to bottom
		for(int i  = 0; i < (int)g_PacketInfo.size(); ++i)
		{
#if 0
			if( strcmp(g_PacketInfo[i].szSource, szIp) == 0)
			{
				char *pTime = g_PacketInfo[i].szTime;
				char *pTok = strtok(pTime, ".");
				if(strcmp(pTok, szTime) != 0)
				{
					//write it...
					fprintf(fp, "%s,%d\n", szTime, nByteCount);

					//reset
					nByteCount = g_PacketData[i].size();
					strncpy(szTime, pTok, 15);
				}
				else
				{
					//strncpy(szTime, pTok, 15);
					nByteCount += g_PacketData[i].size();
				}
		
			}
#endif			
			if( strcmp(g_PacketInfo[i].szSource, szIp) == 0)
			{
				char *pTime = g_PacketInfo[i].szTime;
				char *pTok = strtok(pTime, ".");

				BandwidthMap::iterator iter = m_OutgoingMap.find(pTok);
				if(iter == m_OutgoingMap.end())
				{
					m_OutgoingMap[pTok] = (int)g_PacketData[i].size();
				}
				else
				{
					iter->second += (int)g_PacketData[i].size();
				}
			}

		}

		BandwidthMap::iterator iter = m_OutgoingMap.begin();
		while(iter != m_OutgoingMap.end())
		{
			fprintf(fp, "%s,%d\n", iter->first.c_str(), iter->second);

			++iter;
		}

		/*
		if(nByteCount != 0)
		{
			fprintf(fp, "%s,%d\n", szTime, nByteCount);
		}
		*/

		fclose(fp);
	}

	l_strFileName.ReleaseBuffer();
};

//Not used currently
int CPacketSnifferDlg::CalcOutgoingBandwidth(const char* szIpAddress, int nStartIdx, int nEndIndex)
{
	if((nEndIndex <= nStartIdx) || (nStartIdx < 0) || (nEndIndex >= (int)g_PacketData.size()) || (!szIpAddress))
	{
		return -1;
	}

	int nSize = 0;
	for(int i = 0; i < (nEndIndex-nStartIdx); ++i)
	{
		if( strcmp(g_PacketInfo[i].szSource, szIpAddress) == 0)
		{
			nSize += (int)g_PacketData[i].size();
		}
	}


	return nSize;
}

void CPacketSnifferDlg::OnFileCloseApp()
{
	SendMessage(WM_CLOSE);
}

void CPacketSnifferDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(g_pListCtrl && g_pList)
	{
		CRect windowrect; 
		GetWindowRect(&windowrect);

#if 0
		char szBuf[256];
		sprintf(szBuf, "W (%d,%d) (%d,%d) Size: (%d,%d)\n", 
			windowrect.left, windowrect.top, windowrect.right, windowrect.bottom,
			windowrect.right-windowrect.left, windowrect.bottom-windowrect.top);
		OutputDebugString(szBuf);
#endif

		CRect clientrect; 
		GetClientRect(&clientrect);

#if 0
		sprintf(szBuf, "C (%d,%d) (%d,%d) Size: (%d,%d)\n", 
			clientrect.left, clientrect.top, clientrect.right, clientrect.bottom,
			clientrect.right-clientrect.left, clientrect.bottom-clientrect.top);
		OutputDebugString(szBuf);
#endif

		int xWindowSize = windowrect.right - windowrect.left;
		int yWindowSize = windowrect.bottom - windowrect.top;
		int xClientSize = clientrect.right - clientrect.left;
		int yClientSize = clientrect.bottom - clientrect.top;

		CRect rect, rect2;
		g_pListCtrl->GetWindowRect(&rect);
		g_pList->GetWindowRect(&rect2);

		int nYDiff = (int)abs(yWindowSize - yClientSize);
		int nYList = yWindowSize - (rect2.bottom-rect2.top)-nYDiff - 7;

		// Note: these values are semi-hacky...ok full-on hacky...
		g_pList->SetWindowPos(g_pList, 14, nYList, xWindowSize-(36), 100, SWP_NOZORDER | SWP_SHOWWINDOW);
		g_pListCtrl->SetWindowPos(g_pListCtrl, 14, 75, xWindowSize-(36), nYList - 80, SWP_NOZORDER | SWP_SHOWWINDOW);
	}

}

void CPacketSnifferDlg::OnBnClickedAutoscroll()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_AUTOSCROLL);
	g_bAutoScroll = !!pCheck->GetCheck();
}

void CPacketSnifferDlg::OnClose()
{
	// Free all of our plug-ins.
	ModuleDescriptorList::iterator iter = m_ModuleDescriptors.begin();
	while(iter != m_ModuleDescriptors.end())
	{
		//Free it.
		FreeLibrary((*iter)->hModule);
		delete(*iter);

		++iter;
	}

	CDialog::OnClose();
}
