//-----------------------------------------------------------------------------
// File: NetConnect.h
//
// Desc:
//
// Copyright (C) Microsoft Corporation. All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef NETCONNECT_H
#define NETCONNECT_H


#include <windows.h>
#include <dplay8.h>
#include <dpaddr.h>
#include <tchar.h>


//-----------------------------------------------------------------------------
// Defines, structures, and error codes
//-----------------------------------------------------------------------------
#define DISPLAY_REFRESH_RATE        250
#define TIMERID_DISPLAY_HOSTS       1
#define TIMERID_CONNECT_COMPLETE    2
#define MAX_PLAYER_NAME             14

#define NCW_S_FORWARD      ( (HRESULT) 0x01000001 ) // Dialog success, so go forward
#define NCW_S_BACKUP       ( (HRESULT) 0x01000002 ) // Dialog canceled, show previous dialog
#define NCW_S_QUIT         ( (HRESULT) 0x01000003 ) // Dialog quit, close app
#define NCW_S_LOBBYCONNECT ( (HRESULT) 0x01000004 ) // Dialog connected from lobby, connect success

enum SigningType { SIGN_NONE, SIGN_FAST, SIGN_FULL };


class CNetConnectWizard
{
public:
	CNetConnectWizard( HINSTANCE hInst, HWND hWndParent, LPCTSTR strAppName, GUID* pGuidApp );
	virtual ~CNetConnectWizard();

    HRESULT WINAPI MessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );
    HRESULT WINAPI LobbyMessageHandler( PVOID pvUserContext, DWORD dwMessageId, PVOID pMsgBuffer );

    HRESULT Init( IDirectPlay8Peer* pDP, IDirectPlay8LobbiedApplication* pLobbiedApp );
    VOID    Shutdown();
    HRESULT DoConnectWizard( BOOL bBackTrack );
    HRESULT ConnectUsingLobbySettings();

    void   SetMaxPlayers( DWORD dwMaxPlayers )                  { m_dwMaxPlayers = dwMaxPlayers; }
    void   SetPlayerName( LPCTSTR strPlayerName )               { _tcsncpy( m_strLocalPlayerName, strPlayerName, MAX_PLAYER_NAME-1 ); m_strLocalPlayerName[ MAX_PLAYER_NAME-1 ] = 0; }
    void   SetSessionName( LPCTSTR strSessionName )             { _tcsncpy( m_strSessionName, strSessionName, MAX_PATH-1 ); m_strSessionName[ MAX_PATH-1 ] = 0; }
    void   SetPreferredProvider( LPCTSTR strPreferredProvider ) { _tcsncpy( m_strPreferredProvider, strPreferredProvider, MAX_PATH-1 ); m_strPreferredProvider[ MAX_PATH-1 ] = 0; }
    void   SetDefaultPort( DWORD dwDefaultPort )                { m_dwPort = dwDefaultPort; }

    TCHAR* GetPlayerName()                                      { return m_strLocalPlayerName; }
    TCHAR* GetSessionName()                                     { return m_strSessionName; }
    TCHAR* GetPreferredProvider()                               { return m_strPreferredProvider; }
    BOOL   IsHostPlayer()                                       { return m_bHostPlayer; }
    BOOL   IsMigrateHost()                                      { return m_bMigrateHost; }
    BOOL   HaveConnectionSettingsFromLobby()                    { return m_bHaveConnectionSettingsFromLobby; }
    static BOOL SPRequiresPort( GUID* pSP )                     { return (*pSP != CLSID_DP8SP_MODEM && *pSP != CLSID_DP8SP_SERIAL && *pSP != CLSID_DP8SP_BLUETOOTH); } 

protected:
    struct DPHostEnumInfo
    {
        DWORD                 dwRef;
        DPN_APPLICATION_DESC* pAppDesc;
        IDirectPlay8Address* pHostAddr;
        IDirectPlay8Address* pDeviceAddr;
		TCHAR                szSession[MAX_PATH];
        DWORD                dwLastPollTime;
        BOOL                 bValid;
        DPHostEnumInfo*      pNext;
    };

    static INT_PTR CALLBACK StaticConnectionsDlgProc( HWND, UINT, WPARAM, LPARAM );
    static INT_PTR CALLBACK StaticSessionsDlgProc( HWND, UINT, WPARAM, LPARAM );
    static INT_PTR CALLBACK StaticCreateSessionDlgProc( HWND, UINT, WPARAM, LPARAM );
    static INT_PTR CALLBACK StaticLobbyWaitDlgProc( HWND, UINT, WPARAM, LPARAM );
    static INT_PTR CALLBACK StaticAddressDlgProc( HWND, UINT, WPARAM, LPARAM );
    INT_PTR CALLBACK ConnectionsDlgProc( HWND, UINT, WPARAM, LPARAM );
    INT_PTR CALLBACK SessionsDlgProc( HWND, UINT, WPARAM, LPARAM );
    INT_PTR CALLBACK CreateSessionDlgProc( HWND, UINT, WPARAM, LPARAM );
    INT_PTR CALLBACK LobbyWaitDlgProc( HWND, UINT, WPARAM, LPARAM );
    INT_PTR CALLBACK AddressDlgProc( HWND, UINT, WPARAM, LPARAM );

    HRESULT ConnectionsDlgFillListBox( HWND hDlg );
    HRESULT ConnectionsDlgOnOK( HWND hDlg );
    VOID    ConnectionsDlgCleanup( HWND hDlg );

    VOID    SessionsDlgInitListbox( HWND hDlg );
    HRESULT SessionsDlgEnumHosts( HWND hDlg );
    VOID    SessionsDlgStopEnumHosts( HWND hDlg );
    VOID    SessionsDlgFinalizeEnumHosts( HWND hDlg );
    HRESULT SessionsDlgNoteEnumResponse( PDPNMSG_ENUM_HOSTS_RESPONSE pEnumHostsResponse );
    VOID    SessionsDlgExpireOldHostEnums();
    HRESULT SessionsDlgDisplayEnumList( HWND hDlg );
    HRESULT SessionsDlgJoinGame( HWND hDlg );
    HRESULT SessionsDlgCreateGame( HWND hDlg );
    VOID    SessionsDlgEnumListCleanup();

    IDirectPlay8Peer*       m_pDP;
    IDirectPlay8LobbiedApplication* m_pLobbiedApp;
    CRITICAL_SECTION        m_csHostEnum;
    GUID                    m_guidApp;
    HRESULT                 m_hrDialog;
    HWND                    m_hDlg;
    HINSTANCE               m_hInst;
    HWND                    m_hWndParent;
    DWORD                   m_dwMaxPlayers;
    DWORD                   m_dwPort;
    TCHAR                   m_strAppName[MAX_PATH];
    TCHAR                   m_strPreferredProvider[MAX_PATH];
    TCHAR                   m_strSessionName[MAX_PATH];
    TCHAR                   m_strLocalPlayerName[MAX_PLAYER_NAME];
    TCHAR                   m_strHostname[MAX_PATH];
    BOOL                    m_bSearchingForSessions;
    BOOL                    m_bMigrateHost;
    BOOL                    m_bUseDPNSVR;
    SigningType             m_eSigningType;
    DPHostEnumInfo          m_DPHostEnumHead;
    BOOL                    m_bEnumListChanged;
    DPNHANDLE               m_hEnumAsyncOp;
    BOOL                    m_bHostPlayer;
    DWORD                   m_dwEnumHostExpireInterval;
    BOOL                    m_bConnecting;
    DPNHANDLE               m_hConnectAsyncOp;
    HANDLE                  m_hConnectCompleteEvent;
    HANDLE                  m_hLobbyConnectionEvent;
    HRESULT                 m_hrConnectComplete;
    BOOL                    m_bHaveConnectionSettingsFromLobby;
    DPNHANDLE	            m_hLobbyClient;
    GUID                    m_guidSP;
};


#endif // NETCONNECT_H

