// HLADemoDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "HLADemo.h"
#include "HLADemoDlg.h"
#include ".\hlademodlg.h"
#include "client.h"
#include "server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float KnightWidth=10;
const float ViewportWidth=100;
double FrameMoveInterval=0.1;

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� ������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ����

// ����
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


// CHLADemoDlg ��ȭ ����



CHLADemoDlg::CHLADemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHLADemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHLADemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMAND, m_commandCtl);
	DDX_Control(pDX, IDC_CONNECT_OR_NOT, m_connectOrNotButton);
	DDX_Control(pDX, IDC_MESSAGE_LOG, m_messageLog);
}

BEGIN_MESSAGE_MAP(CHLADemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HELP_BTN, OnBnClickedHelp)
	ON_CBN_SELCHANGE(IDC_COMMAND, OnCbnSelchangeCommand)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CONNECT_OR_NOT, OnBnClickedConnectOrNot)
END_MESSAGE_MAP()

/*--------------------------------------------------------------------------
* Purpose:       Set the proper number of lines in a drop-down list or
*                combo box.
* Description:   Resizes the combo box window to fit the proper number
*                of lines. The window must exist before calling this function.
*  This function should be called when the combo box is created, and when
*  the font of the combo box changes. (e.g. WM_SETTINGCHANGE)
*  Testing needed:
*    Are there cases where SM_CYBORDER should be used instead of SM_CYEDGE?
*    owner-draw variable height combo box
*    Subclassed combo box with horizontal scroll-bar
* Returns:       nothing
* Author:        KTM
*--------------------------------------------------------------------------*/
void set_DropDownSize(CComboBox& box, UINT LinesToDisplay)
{
	ASSERT(IsWindow(box));	// Window must exist or SetWindowPos won't work

	CRect cbSize;			// current size of combo box
	int Height;            // new height for drop-down portion of combo box

	box.GetClientRect(cbSize);
	Height = box.GetItemHeight(-1);      // start with size of the edit-box portion
	Height += box.GetItemHeight(0) * LinesToDisplay;	// add height of lines of text

	// Note: The use of SM_CYEDGE assumes that we're using Windows '95
	// Now add on the height of the border of the edit box
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges

	// The height of the border of the drop-down box
	Height += GetSystemMetrics(SM_CYEDGE) * 2; // top & bottom edges

	// now set the size of the window
	box.SetWindowPos(NULL,            // not relative to any other windows
		0, 0,                         // TopLeft corner doesn't change
		cbSize.right, Height,         // existing width, new height
		SWP_NOMOVE | SWP_NOZORDER     // don't move box or change z-ordering.
		);
}

BOOL CHLADemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	// �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// create canvas objects 
	m_client=RefCount<CClient>(new CClient(this));
	m_server=RefCount<CServer>(new CServer(this));
	m_client->m_canvas.m_owner=this;
	m_server->m_canvas.m_owner=this;

	CWnd* clientCanvasWnd=GetDlgItem(IDC_CLIENT_CANVAS);
	CRect clientCanvasRc;
	clientCanvasWnd->GetWindowRect(clientCanvasRc);
	ScreenToClient(clientCanvasRc);
	m_client->m_canvas.Create(NULL,"Client Canvas",WS_CHILD,clientCanvasRc,this,30000);
	m_client->m_canvas.ShowWindow(SW_SHOW);

	CWnd* serverCanvasWnd=GetDlgItem(IDC_SERVER_CANVAS);
	CRect serverCanvasRc;
	serverCanvasWnd->GetWindowRect(serverCanvasRc);
	ScreenToClient(serverCanvasRc);
	m_server->m_canvas.Create(NULL,"Server Canvas",WS_CHILD,serverCanvasRc,this,30001);
	m_server->m_canvas.ShowWindow(SW_SHOW);

	int index;
	index=m_commandCtl.AddString("Create a Knight"); 
	m_commandCtl.SetItemData(index,Command_CreateKnight);
	index=m_commandCtl.AddString("Destroy a Knight");
	m_commandCtl.SetItemData(index,Command_DestroyKnight);
	index=m_commandCtl.AddString("Move a Knight");
	m_commandCtl.SetItemData(index,Command_MoveKnight);
	index=m_commandCtl.AddString("Change a Knight��s State");
	m_commandCtl.SetItemData(index,Command_ChangeKnightState);
	index=m_commandCtl.AddString("Move the Viewport");
	m_commandCtl.SetItemData(index,Command_MoveViewport);
	set_DropDownSize(m_commandCtl,5);

	m_commandCtl.SetCurSel(0);

	SetTimer(1,(UINT)(FrameMoveInterval*1000),0);

	SetConnectOrDisconnectButtonCaption();

	return TRUE;  // ��Ʈ�ѿ� ���� ��Ŀ���� �������� ���� ��� TRUE�� ��ȯ�մϴ�.
}

void CHLADemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸����� 
// �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
// �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CHLADemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�. 
HCURSOR CHLADemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHLADemoDlg::OnBnClickedHelp()
{
	ShellExecute(NULL,"open","ReadMe.RTF","","",SW_SHOW);
}

void CHLADemoDlg::OnCbnSelchangeCommand()
{
	int index=m_commandCtl.GetCurSel();
	if(index<0)
		m_command=Command_None;
	else
		m_command=(CommandType)m_commandCtl.GetItemData(index);
}

void CHLADemoDlg::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);

	m_client=RefCount<CClient>(); // delete this object first
	m_server=RefCount<CServer>();
}

void CHLADemoDlg::OnTimer(UINT nIDEvent)
{
	m_server->FrameMove(FrameMoveInterval);
	m_client->FrameMove(FrameMoveInterval);

	m_server->m_canvas.Invalidate();
	m_client->m_canvas.Invalidate();

	CDialog::OnTimer(nIDEvent);
}

void CHLADemoDlg::OnBnClickedConnectOrNot()
{
	// connect to server or disconnect from server
	if(m_client->IsConnectedToServer()==false)
	{
		m_client->Connect(m_server);
	}
	else
		m_client->Disconnect();

	SetConnectOrDisconnectButtonCaption();
}

void CHLADemoDlg::SetConnectOrDisconnectButtonCaption()
{
	if(m_client->IsConnectedToServer()==false)
	{
		m_connectOrNotButton.SetWindowText("&Connect");
	}
	else
		m_connectOrNotButton.SetWindowText("&Disconnect");
}

void CHLADemoDlg::AddMessageLog( CString& txt )
{
	int idx=m_messageLog.AddString(txt);
	m_messageLog.SetCurSel(idx);
}