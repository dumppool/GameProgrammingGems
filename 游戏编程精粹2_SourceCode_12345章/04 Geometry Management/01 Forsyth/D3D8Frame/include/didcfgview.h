//-----------------------------------------------------------------------------
// File: didcfgview.h
//
// Desc: Header file for DIDCfgView, a class that encapsulates a view of a
//       DirectInput device.  The DIDCfgView class exists to make it easier 
//       to make custom interfaces to view or configure action mappings for 
//       input devices(instead of using IDirectInput8::ConfigureDevices).
//
//       To use the DIDCfgView class, you initialize it for a particular
//       DirectInput device.  You then set up state information for how the
//       image should be drawn: colors, fonts, and details for callouts(the
//       lines drawn from each axis/button to a label).  Finally, you can 
//       call RenderView, passing in a bitmap or HDC for DIDCfgView to draw
//       the image to.
//
//       DIDCfgView is the only class in this file that you need to understand 
//       or interface to.  The other classes shown here are only used to 
//       implement the DIDCfgView class.
//
//
// Copyright (c) 1999-2000 Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __DIDCV_H__
#define __DIDCV_H__




#include <windows.h>
#include <dinput.h>




// invalid id, used for unique, signed identifiers
#define DIDCV_INVALID_ID                            -1

// flags for callout states
#define DIDCV_DRAWCALLOUT                           0x1
#define DIDCV_DRAWOVERLAY                           0x2
#define DIDCV_DRAWHIGHLIGHT                         0x4
#define DIDCV_DRAWEMPTYCALLOUT                      0x8
#define DIDCV_DRAWFULLNAME                          0x10

// all flags set
#define DIDCV_ALL_CALLOUT_STATE_SET                 0xFFFFFFFF

// default dimensions
#define DIDCV_MAX_IMAGE_WIDTH                       512
#define DIDCV_MAX_IMAGE_HEIGHT                      512
#define DIDCV_CUSTOM_VIEW_WIDTH                     400
#define DIDCV_CUSTOM_VIEW_HEIGHT                    300

// DidcvCreateFont attribute flags
#define DIDCV_FONT_BOLD                             0x1
#define DIDCV_FONT_ITALIC                           0x2
#define DIDCV_FONT_UNDERLINE                        0x4
#define DIDCV_FONT_STRIKEOUT                        0x8

// default font point width
#define DIDCV_DEFAULT_FONT_SIZE                     90

// default size of callout array in DidcvCalloutManager
#define DIDCV_DEFAULT_CALLOUT_MANAGER_ARRAY_SIZE    0

// default size of callout array in DidcvCalloutSet
#define DIDCV_DEFAULT_CALLOUT_SET_ARRAY_SIZE        8




// forward declarations
class DidcvViewManager;
class DidcvCalloutManager;
class DidcvBitmap;
class DidcvCallout;
class DidcvCalloutSet;
struct DIDCfgViewInfo;
struct DidcvCalloutData;
struct DidcvCalloutState;
struct DidcvActionMap;
struct DidcvCustomViewInfo;




//-----------------------------------------------------------------------------
// Name: class DIDCfgView
// Desc: main interface for retrieving and rendering device configuration views
//
//       Init() - initializes the object for a particular DirectInputDevice
//       SetActionFormat() - sets the action mapping for the device
//       SetCalloutState() - specifies state for a given callout
//       SetAllCalloutState() - sets the state for all callouts
//       GetObjectIDByLocation() - gets object id at location of current view
//       SetCurrentView() - sets the current view to be rendered
//       RebuildViews() - collapses or expand the views for this device
//       RenderView() - renders device configuration view with image,
//                       callout information, action mapping.
//-----------------------------------------------------------------------------
class DIDCfgView
{
public:
    DIDCfgView();
    ~DIDCfgView();

public:
    // main device configuration view interface
    HRESULT Init( LPDIRECTINPUTDEVICE8 pDevice );
    HRESULT SetActionFormat( LPDIACTIONFORMAT pDiaf );
    HRESULT SetCalloutState( DWORD dwFlags, DWORD dwObjID );
    HRESULT SetAllCalloutState( DWORD dwFlags );
    HRESULT GetObjectIDByLocation( LPDWORD pdwObjID, LPPOINT pPt );
    HRESULT SetCurrentView( INT nView );
    HRESULT RebuildViews( BOOL bCompact );
    HRESULT RenderView( HBITMAP hBitmap, BOOL bIsDib );
    HRESULT RenderView( HDC hdc );

    // rendering options
    HRESULT SetViewOrigin( const POINT* pPtOrg, POINT* pPtOldOrg );
    HRESULT CalcCenterOrgForCurrentView( const SIZE* pImgSize, POINT* pPtOrg, BOOL bSetOrigin = FALSE );
    VOID    SetDefaultText( const TCHAR* pctszDefaultText, DWORD length );
    VOID    SetColors( COLORREF crFore, COLORREF crBack, COLORREF crHighlight, COLORREF crHighlightLine );
    VOID    GetColors( COLORREF* pCrFore, COLORREF* pCrBack, COLORREF* pCrHighlight, COLORREF* pCrHighlightLine );
    HRESULT SetFont( const TCHAR*  pctszFontName, DWORD dwAttributes );
    HFONT   GetFont();

    // information access functios
    HRESULT GetInfo( DIDCfgViewInfo* pCfgViewInfo );
    HRESULT GetCurrentView( LPINT lpnView );
    HRESULT GetCalloutState( LPDWORD lpdwFlags, DWORD dwObjID );

protected:
    // protected helper functions
    VOID    CleanUp();
    HRESULT InitAlloc();
    HRESULT InitImageInfoRetrieve( LPDIRECTINPUTDEVICE8 pDevice );
    HRESULT InitImageInfoProcess();
    VOID    CalcSizes( const DIDEVICEIMAGEINFO* prgImageInfoArray, DWORD dwNumElements, const LPDWORD pNumViews, LPDWORD pNumCallouts, LPDWORD pNumDistinctObjID );
    HRESULT RenderView( HDC hdc, VOID* pBits, INT width, INT height );

    HRESULT InitCustomViews( LPDIRECTINPUTDEVICE8 pDevice, BOOL bUseInternal = FALSE );
    HRESULT BuildCustomViews();
    VOID    RenderFullname( HDC hdc, const TCHAR* pctszFullname, const RECT* pRect );
    BOOL    CopyActionMap( LPDIACTIONFORMAT pDiaf );

protected:
    // data structures for managing views and callouts
    DidcvViewManager* m_lpViewManager;
    DidcvCalloutManager* m_lpCalloutManager;

    // keeps track of whether this has been initialized
    BOOL m_bIsInit;
    // the index of the next view to render
    INT m_nView;
    // pointer to the DIDEVICEIMAGEINFOHEADER
    LPDIDEVICEIMAGEINFOHEADER m_lpDidImgHeader;
    // custom device view data
    DidcvCustomViewInfo* m_lpCustomViewInfo;

    // device reference
    LPDIRECTINPUTDEVICE8 m_lpDIDevice;
    // action mapping copy
    LPDIACTIONFORMAT m_lpDiaf;
    // can this view be collapsed
    BOOL m_bCanBeCollapsed;
    // is this view compacted
    BOOL m_bIsCollapsed;

    // rendering options
    COLORREF m_crFore;
    COLORREF m_crBack;
    COLORREF m_crHighlight;
    COLORREF m_crHighlightLine;
    POINT    m_ptOrigin;
    HFONT    m_hFont;
    TCHAR m_tszDefaultText [MAX_PATH];
};




//-----------------------------------------------------------------------------
// Name: struct DIDCfgViewInfo
// Desc: struct containing current information about DIDCfgView states
//-----------------------------------------------------------------------------
struct DIDCfgViewInfo
{
    BOOL  bIsInit;              // is the CfgView object initialized
    INT   iCurrentViewID;       // the ID of the current view
    INT   iNumTotalViews;       // total number of views
    DWORD dwNumUniqueCallouts;  // number of unique
    BOOL  bCanBeCollapsed;      // can views for this CfgView object be compacted
    BOOL  bIsCollapsed;         // is the CfgView currently using collapsed views


    DIDCfgViewInfo()
        : bIsInit( FALSE ),
          iCurrentViewID( DIDCV_INVALID_ID ),
          iNumTotalViews( 0 ),
          dwNumUniqueCallouts( 0 ),
          bCanBeCollapsed( FALSE ),
          bIsCollapsed( FALSE )
    { }
};




//-----------------------------------------------------------------------------
// Name: struct DidcvCustomViewInfo
// Desc: object containing information about custom views
//-----------------------------------------------------------------------------
struct DidcvCustomViewInfo
{
    DWORD dwType;               // type of the opera0`?``����flnl�l�lflnl���  ``�`����l�l�l�ll<l~���ٜ  ``~`~�f�~lfl~l~l`lll~�v�a�  ?�?�`?�1�?�1�?�1�0�?�><x8   `�`6`����l�l�l�l�l�l�L��ל�  00>0~0�~��m�m���n�3c���  ``�`����l�l�l����m�m�l����  >0>020>� �66c6k6k6kff6�ἁ  0�0�0�~�~6����c6��6f>f0�?�y�  ``�`�� �~l~lfl~l~ll^��̹�;  ``~`~���l�lfl�l~l~l���q  ``�`��6�6l�l�l l~l~lf�~��   000~�~66 666U6W������  ``�`��Z�Zl�l�ll<l>l{���ٜ  6`6``6�>�llmlmllll���  l`l``l�m�l�ll~lfl~lf�~�~�g   0�0�0����6�6�6�6�6�f�f�F����   000���~6Z6~6Z6~6ffF��   0�0�0�~�~�6 66c66cffc��a  ���� `~`f�~�ff~fgf���  f0f0�0�~f~~6f6~6f6�6�6jfqf~�~�   `�~��m�9��0���:`9�y��l��9�3  j0�0�0j�n�`66 666kfkffv�b�   0?030?� �6c66c6&cff6Fs�`�  60v0�0��6�>6 6�6�636wf3f�F3�v�   `}h�l`��l��fm�m�lll�ݜ�  0Z�Z�<x~�f������?�?�xpp  `�`��� ��3o�008�   0������s���>6�ɶ66fsffx�`�   0�040�~�~�6 6~6 6����.fmf���  `����hf��h~���� ��8x�p  000~~��c66c66cff6�c�A  60��60>� �6m6666fff�p�  |�|��|0a�}��<4;� ?�?�xpp  j0�0�0j�~�Z6~6Z6~6f�f�F>�[�؜  ``~`�`����4l~l�l�l~lll~ll�~�`�  00>0~0�0�~[~606�6>&�f=f�F���  j0�0j0n�`�6 66c6&cff6F��Ü  0~00����f~�666�6�6�&�f�F��Ɯ  0�0�0Z����6z6�6�66kffkF�c�  0Z00~~����k6ն<6f�Fm�ܜ  0006~����m66m6fffF~�q�  000~m~~6m66h6}6nfwfo���Μ  00����c~{6k66 6~6ff~fgF����  0 0���6�v��D�� UdUjUjU�d<  60�0�06��[66[666�&�f�F��ǜ  ~00����Z6~6Z6~ff����  ffff   0�00�0m��m6666f�f�F�� �  000�m�~6m66m66m66ʦ�n�L   0�j0~j~6m66m66l6���Ρ�  3��ľ?����f�g�f� ?�?�<<08  0����U~w~66�66�66����6f�N1�  60����6~w~U6w6l6�6�6l6flF�`�    ��0`�       | 8    ��0p`�   < 8    ��0`���  ��  < 8    ��0lߌ�����0�`< 8    ��0c���8l0 < 8    ��2f��?�?�  < 8     �?�clFl��1�c8x 0    ��4g�  ��>8 | x    ��3c�O��~>1�!�    ��0l���l�� < 8    ��0a�ٌ��l�� < 8    ��3c����� < 8    ��0�_����8 ��     �?�cl[l���3�l8x p    ��0f���l6���< 8   �?�p�͌7,3,7�=�0,?�?�      ��0���LL?�?�<8    ��3{l�l?�?��l3,#<8    ��<o���?�?�l�� < 8    ��3g͌8�7l ���� 8   �?�0cf��?�3f?�3f?�3f3n3l    ��3���;l?�L��l� 8  `�~���6��<� ���0�>���<�|   �?�0����� ?�2f?�2n?�   ��4~���,���>,l>lv<8   ��6l��l��?�0<�l� < 8   ��0~�ڶ����f��    ��4~�֖����>�>��v    8��     ��    ����8�x����������|    ����~�~��������f�|  �a�clcla�y�x�c�c�cNc6{N{�c�  ?`?`3`?d3n?x `��fo~o<| ����    ��` ` ` ` ` ` ` ` ` ` ��    ��c c�g�n0lp`�a�gn` ��    ��a�m�o��}�m�m�o�g�` ��    ��a�o�o�m�m�m�m�m�a�` ��    ��` o�o�a�a�a�a���` ��    ��a�o�o�` g�g�f`fln|l8��  ��` `xg�f g�g�f`f`l`l`` ��    ��` o�o�`�g�g�`�o�o�` ��    ��f g�o�yxcXf�m�cf8`0��  ��` o�o�m�o�m�o�o�a�a�a���    ��` o�o�a�o�o�a�o�o�` ��    ��c f0o�o�c`o�o�f`n`l`��  ��` d``h�vhhfhgh~jf�` ��    ��a���m��{�c�gpn8l��    ��k0k0o�o�c000k|{|` ��    ��a�m�o�a�o�a�o�a��` ��    ��c`||c`oxoxc`||c`��  ��f o�{Xf�m�c8` o�l0o�l0��    ��c�np}�xoXmXoXmXo8m0��  �����`�`�����l�l�l�l����Ā����  ��`�g�f�g�`��` g�f�f�c0n�    ��i�m�c�{�m�a�m�}�y�a���    ��mXox` o�m�o�m���a���  ��a�o�m���l0o�l0o�f`l0��  ��a��c0~�f0g�` oxoxmX��  ��a�g�~|{�` oxmXoxf0o8ih��  ��l`|r�o�l0o�l0o�f`�l`��  ��a��a�o�mXo�f0g�f0g�l��  ��f0�f�o�|��l�o�loxl��  ��kXoxkXoxf�o�|��l�o�l���    ��0 0 0 0 0 0 0 0 0 ?�?�        ��f`f`f`f`fll|x8` ` ��    ?�?�0 6301�0�1�30640 ?�?�  ��` f g�o�h���a�c`f8l��    ��a�c`n8}�xg�f0g�g�` ��    ��l0o�l0o�c ��f`c�np��  ��`�o�lo�l o�n�o�v�f�` ��  ��` c0o�c0a��c o�c�` ��    ��` g�f0g�` o|mlmlo|` ��  ���������������    ��000����00066   �?�?    ����         00000����00000000  ������������1�1�a�@   ��006>q��������   ``?`<```����````0```@`    ��1�a�A���������  ������00����00000`0  ����	�?�?���������  �������0����00��0  06�6�6�6�����6�6�6�6�6�6�6�0    ��������a�a�a�a�a�a�  ���8�{�[���x��������  0�0�0�3�����0�6�6�5�1�336x40  ` a�a�a�a�a�a����������    ?�?�33?�3?�?������ � � �  ���  00x2�  ��������   �� ?�?�0?�0?� ����    0`3�3�0�����386?�?�6�6�6�=�=�  0`?�?�1�?�?�1�?�?������  �������1������`8p  �����?�?�2L7�1�7�7�1�1�1�  3;��?�?�1�?�1�?�?��������  33��3?~3~?3����jq��  0�7�7�0�����7�6�7�6�?�?�31808   c����8k��>�������������       `0       ``````~~~~```````     ����  @`0       � ����������������������� � �     ��  ?�?�000?�?�0    ��  ���� �`0     �� ?�?�00?�?�0 0 ` ` �    �� ��`�a�a��``��   �� ��`dXf�c�f�lX��`  �������m�ofg�}�{�c��  00000���� 4620000p0   000l0l08<c462l0l000 0  ����0����`g�flg�``     � � � � � � � � � � � � � �    ��000��000��      ��xp  ����     ~�x�`�`�`�`�`�f�n�|�p� � � �    ~�~������������� � �   x?�? 0 ?�?�0 7�666p& fG���   ~x~ffffffffffff~fv~l`0```   ~�x�`�`�~�~�`�`�`�f�|�p� � �   ��0``���?�0 7�606p6`6g���    ~�~�6�6�v�����~�~�f�f�~�~�f�    ||clcllclll`lfln|{xq` `   ~~fff�f�ff2f3fnl` `   ~�p�f�v�~�n�f�f�~�v���p� �  0 0~~�f�ff|fxfffs~|`|`p`   30?� ��0��lf���  0 ?~~lfffffoflflfon�l�` `   ~~fUfUfUfUfUfUfUnW����` `  ?�?� �}���3���  �0���    6~w~�f�ff;fa����f3n3l?`?`3`   � ����m�m mxmHmHmXoDn|l����   ||cllclll lfl~|x|c``>`    w~U~wf ff f����0f?n?l```  7���ƴִ״ִ״״�4ִ��6�`0�0   ~~<f�f��f3����fon>l�`�``  7X�8ޘ^��n��  ��80     ?�?�0 0 0 0 0 0 0 0 ` ` � �     �?�����?�0 0 0 0 p ` � �     ?�?�0 7�7�6666�6p6fg���    ��` ` o�o�`�`�`�`�`�`�����    ?�?�0 1�1�?�?�1�1�3#fLx�p    ?�?�0 7�7�0�0�?�?�0�`�`����|    ?�?�0 7�7�0�0�?�?�0� �`�@���    ?�?�0 7�7�1�1�1�1�3#flxHp  ?�?�0 7�7�0060?�?�0�1�#0n0Lp�`    ?�?�0 0�0�7�7�0�0�0�0�o�o��     ?�?�0�0�0�7�7�0�0�1�30fl�  ��a�o�o�c f�o�o�`�o�o�`�����    ��` o�o�l�o�o�l`l`ovo>����    ��c g�o�m�a�a�a�a�a�a�����    ?�?�303?�?�3`3l3x6pf�m��~�<    ��``o`o`mlm|mxm`o`of`f�~�<    ?�?�6 7�?�<�0�?�?�0�l�l�����    ?�?�0�7�7�0�?�?�0�7�g�`�����    ?�?�0 ?�?�307�7�0�7�g�`�����    ��` o�h�j�j�j�j�j�gm�؜�    ?�?�60?�?�6�6�6�6�f�l�o����|    ?�?�0�7�7�30?�?�0�?�o�`�����    ?�?�0 7�6�7�6�7�7�0�g�`�O���  ?�?�0�?�?�0�7�6�7�6�g�`�o�����  ��` o�lo�lo�` o�``�����A�  ��` `�n�n�j�j�j�n�n�`�`�����  ?�?�0 7�0�?�?�6�?�?�6�o�`�����  ?�?�0 1�7�1�1�?�0 7�67�fG��    ��c`o|o|c`o|o|c`||c`�`�`  ?�?�0�7�7�67�67�7�0�&�l�K���  ��` o�o�m�o�m�o�m�o�`m�����  ��a�o�o�lo�lo�`�n�n�f���ن  ��` f|f|oLoLf|oLo�v�vLf|�|�L  ��`�`�o�c0f|o�l�o�l�o�����  ��` o|o|f0o|o|f0oxo|~�w��0�0  ��` h�e0o�bfj�j�o�o�b8fl�Ƙ   ��` �a�o�lo�lo�f o�[0���<  ?�?�0�?�0�7�7�67�67�f���    ��`o�`>o�i�o�o�`,m����@    ��` o|koXk~o~hk8oxl�ɞA  ��f0��f0o�m�o�a�o�a�o�����    ��f0o�f~o�fl�f<g�o�m����f  ?�?�67�67�307�>�3�6�l g����  ��k0o4k4o0`�o�o0k8o8klol��ˆ  ��` i>�i0o>i>o4i4�`4J4�d�D    ��f~~f8o|~�va�m�l>����    ��f0v�o>fl�q�w�u�w�q�����    ��a�gp~<{�` o|mlo|f0f8�|�l    ��c0�c0g�f�g�f�o�l�l�ό�  3;���` o0c>�o�m�o�m�߾��  ��k0o4`~o~k0oxkLo�l�o�l�����  ?�1�7�6g������d�n�j�n�t����Z           `000��        �� �����3w�o�    0000000`0`6lflf����� �     ��``fs~c< 00��p    ����<p�<� 00�?�  ���?�?������� `0�?�?    00?�?� ����1�a�G�     00��  00006lfl~|����    ��00?�?�0��0�1���    �������� 0?�  ����?�1�?�1�?�?� 0�?�?   @�� ��`07���  ?�?�   `0� ��0�n�	�0��>   `��`6�~����~���p�>   `��`6�~����~��  �  ?�?�  `��0`6l~����~�l;ls6g6    �{�y������1�1�l�����x��   0 �v4v� ��4��g0g��xۼ�t���~    ?�?�00``����xx`    ��00`@���p�>`    ?�?�0`xx0�0�c�_>    �� ��00 1``���<    }�}��l�l�8���p6`"�a��   p?�?�0 0 ?�?�60603`3`a�c��>L    o�o�m�m�m�m�l�|��p����    ?�?����� ?�?�`���||   ��  �� ���8�q�o�   3033 �� ��03`a�C�>  ��0`�?�>8  ~�~�6�x><v�`�  ����?�1�?��?�?�p���||   ������l�ll~x{0�p��9�3    ~�~�6l8~�v�  ~�~�6�x>|v�`�  � ��f�flfl~lflfl~lf8g0�8���   ��~ 3���`��`��>pp   ����6�f�  ?�?�`���x<   �6�cL�L>LLXXp.0mp���    ~�~�f~�~�` ~�~�`�~x~0`xa�a�  Z�Z�<��~�~��~�~���38f�DF   ��c07��x<��`���||    ��|�|���~�~�0�6�f�~�~� �    c��l`l~l`lx8l0xm���݄  �=�=�1�=�1�?��?�?�0��|  ?�`�`~�4�p2�����0�0�    ����l~lkl�l>x�8>06x>�7�?�  6�6��`��?����6 >|6l?8����      ?�?�0000000?�?�0    �����������    ��0`_������� < 8    ���� ��xx0p    ����  ��p<`8    }�}�d�d�d�d�d�|�|�d�a�<8    ~�~�f�f�f�f�f�~�~�f�`� � � �    ����  00`@   ~~�f�f�f�f�f�~�~�f�f     ?�?�x8pp ����    }�}�e�e�e�e�e�}�}�ecf    �����0�0�0�0�0�0�0�0�0�0 p `    ����  ?�?�1�1�?�?�1�  x p     0?�?�  ����   ` `~`~dflflfxfpf��`ff`f ~ <  ��?�?�1�1�?�?���� �}�p|     ��  ��<l���    ��06>xp ����    ��` ` g�g�ffg�g�` ` ��   0|0|0l0l0m�m�l0l0|0|0l0`0 0 0    ����  ���� �� � �    ?�?� �� ?�?�0�0�?�?� < 8    ����ɌɌɘɘɘ�����`����     �~`~`f f0fpfpfX~�~�a�   ` `~`�g�flflflflfl~�~�a�<8    �}�|ll,lllLl�|�}�mc 8 0    �����0�0�0�0�0�0�0�0�06>    �� �����  ?�?�00?�?�    ��~`f`f`f`f`f`~`~`f` `��   ��|0|0l0m�m�l0|0|0l0 0 0 0    �����`�`�`�����`�`�`�` ` � �    ��0 0 ?�?�  ?�?�00?�?�   � �|�}�o�o l�l�l0|`|�m�a�� �    ��4`f������00�0     �~�~�f�f�f�f�f�f�f�~�~�f� �   0 0|`|`d�e�e�e�d0d`|@|�e�c��    �}�|8lpl`o�o�l`|`|`l` ` � �    ��p���@ ?�?�0000?�?�00  ������?�?�  ����    ����?�?�1�1�1�1�1��     |}�m�lm�m�|�|l  8 0    ��`o�o�`g�g�flg�g�``<`    ��80v`#����|��   8?�?�0 ?�?�0 0 ?�?�|l����  �����?�?�1�?�?�� ���p<   ` `�`�`�`�����`�`�`�`�`�`���    ?�?�007�7�6l6l7�7�00<08   ����`�`�`�~�����`�`�`�f ~ <    ����������������������� � �   0 0�}�}�m�o�o�m�}�}�m��� �    ����  ?�?�00?�?�0  �}�|�d�d0d`d`d`|`|`d``` ` `    }�}�lm�m�m�m�m�|}�m�` <    � � �}��o0l0o�o�|0|0l0 0 0 0   ` `|`|`mdmlmhl`lp|�|�m�    �}�|�o�o�l�m�m�|�|�m��      ?�?�����?� �?�lO��    ��|�l�l�o�o�l�|�|�l� ���   ��`����>��00��0  ?�?�  ��`0��00��0   ` `|p|�l�m�ofl2m�}�|l`0 0     ` x|l|ll`o�o�l`l`|�|�m�c�  0�0�0�2�6�����0�0�0�0�p��   �m�l����  ����     `�0������������������� � x   ` `|�}�o�oll�l�|�|�m��    ��|`l`l`l�m�o�f|`l` ` ` `    �����y������    �{�{lklklklk�k�{ { kc��   ` `�����`�`�l�l�l�����`�f ~ <    ����٘���������������xp  ��}�}�m�o�o�m�m�}�}�m����   ��`;8�_�� �����   ����̀̀����͘͘����͘�   � `��l m�m�m�m�}�}�m�c   � ����������ٰٰ�����0�6>   �?�?�0?�?�0 7�7�6fg����   � �|��o�l�l�l�m�}�}lcl�� �   `�`�����`�`����Ɍ�����`���   �����l�l�������l�l�l�l��  ��l`o�o�` ��0����`0p`  ?�?�0?�?�  ?�?� ���`x>`   ` `|`}hmlmfobn`ll|l|l0``�    ` `|`}�m�m�m�m�m�}�}�m����   ` `|`}�m�llllllo��|pl��    {�{�i�i�i�i�i�i�{l{8c0fx��  ��}�}�o�onll`l`|`|�m�c     �|�|�m�o�o m�m�|�|�lp ��  ��������޶ضٶٶ�f�f����<    ��h�i�i�i�i�o��y�i�a��   0 0}�}�m�o�o0l0o��|0l0 0 0 0  ?�?� ����  ����  ���?�1��������    �{�hi�i�k�k�h�x�i�i�cx 0   � �|�}�o�ol�l�lf|6|�o�� > <    ���� ?�?�00?�?�0    ����� �|�|�l�l�l�x�`�`f~>    ?�?�<8<1����������   � �|`�o�m�m�m�m�|�|`l���    �����������q�a��  ��}�}��9�w��@ ?�?�00?�?�  ����  ?�?�������  ��l0o�o�` �� ����p88p`  ���?�a����  ����   ` `�`�����`�`�����`��ɘ�    }�}�l�lm�m�l�|�c�       �}�| l o�o�l`l`|�|�m���    ����������`�`�0�0�     �����٘٘����٘����٘   ` `{�{�kfkfkfkfk�{�{kc   �?�?  ��`88������    ���������������������06>  ������ɘ� �����`����� ��   � ������������������0�ƌ�D    �{�{ k�k�kpk0k0kx{�{�k c��    �����`�`�����l�l�l�l�l��  �����l�l�l�l�l�l�l��ɰ   ��00�  ?�?�0333px  ������cf�llx,``   � �|�|�m�o�o�m�l�|�|plp ��   ` `����͌�����͘����̀͆� �   `�}�}�m�m�m�m�}�}�lc��     `��������� �����0�`����    ��������������l����      ` `  ������٘����� �h����    ` `�`�����`�������l����f ` `    ���������`�`�����0�2��    �{�{k�k�k kf{l{xk`f`f~<    �����l�l������l����  < 8   `x`x�i�kn�h`k�k�hx0y�h� p 0   ` `�����l�����l�l�����   00�0�~�~ֶ׶�6�6ֶ���f���     ����0�0�0�����0�0�0ۺ�~ n    ?�?�1�7�1�7�7�07�6lg�g���    ���� � �����`�l�l�l�ff � �    ����������ٰ����ٶ6>   ` `�`�~�~�`�`���������    �����ֶֶֶֶ׾�����   ll�l�l�l�����l�l�|�|�  ��    ������������ ��و    ����`�l�l�`�����`�`�` ` ` `  ``o|o|l`of�~�<  ?�?�00?�?�   ������������������������    �������������������������   ` `�����`�`���������x�l�fb `   � �|�}�o�om�m�m�}�}�m�       {�{�hk�k�klklkl{�{�kl` < 8   ~����ҶҶִֶ֔��Ӹl� �  �{�{fkfk�k�kfkfk�{�{fh` ` ` `    ��|0lpl`l�m�of|b|`l```��   ``�`�����`�`�����`��٘    �}�}�m�l o�o�l�}�}�l  < 8   ` `{�{�klklk�k�{l{�k�cl ` ` `   <{�{�h`klklklh`{�{�h``` `��   ��`�w�`>�>�6�6�>�>�0� �    ����������������   ��   ` `|`}�m�l`l`o�o�|�|�m����    �}�}�m�m�m�m�m�}�}�m�a���  ��}��o�m�m�m�m�}�}�m����  ���� �0����ִִִ���0�0� ��  ���P�P�~�����V�V�^�\�PF~<   ` `�f�f�f�f�����`�f�f�f���   �?�?�0`��  ����   � �}��nm�m�m�m�}�}�m�a�a� �   @ `�,�������������������� x   � �{�{�h�k�k�j�k�{�x�h��00   ` `�`�����l�l�l���������   <����`�`�����`�����l�nf` `   � �y�y�k�n�h�h�h�x�x�h�`� � �   ~ x��������>�{������     � �����ٌ�������������  ```?xsl[d`���` ���   � �x0{�k�hfoloxip{p{xfnf	� �   � �|�}�m�o�o�m�m�}�}�m�a���   ~�~�������������80    ~�~�f�~�~�  ��``ff0~`<    �����������X�X�X����     �|�|�d�d�d�g�g�d�|�|�d�`� � �   ` `x`�o�h`h`k�k�{{kc��    {�o�kVkVkVkVkV{V~Vf��    � �������ــ�����0�0�00��   ` `������ٰ۰��������ٶ�� 0  ��y�y�k�k�i�i�i�y�{0cp��   �������˘���`������0� � p    ` `������ � �����0�0�0 0 � �   ` `������������������ � � |  ��0֤���x��0�����   ��    �����٘٘����٘����٘    �������������������  ��0�  ��0�0�>���� 0    ���������٘����������      >�6�>�  ?�?�  �� �  � �   `�`�l�l�x�`����Ѱ��Ӱ6>   � �������0�������������� � x    �����٘�����f�`�����` `��   ` `�����`���l���`���f���   ������������������   �a�3�� ��1�gD���  ��������`�����`�`�����` ` `   ` `�d�l�h�`���������l�fb ` `   ||lclcl||l llf�f�c A����>   ��`����� ɘ��ɘ���������   ` `�����`�`����� �������   ``�`�����`�`���������lnf `   � �}�o�l�l�m�o}�}�m�a���  ��}�}�oo�m�m�m�}�}�m��     ` ������������������  ��������`�`�����`��ٰ   � `��i�i�k�k�hlx�{�kp`��  ������рѰ�6ӴҼ���p���   ` `����٘����� ��������  x 0   <����`�����`�`��������  ������߼ۼٰ۸߼����ٰ6>   � ���`�o�o�`�oXkPk0or`����   ` `������f�`���������� x�    ��� ��������������  ��  00�0�����0�0�p�x�����2�000    ��0����`o�llo�o�``  �{�{kfkfk�k�kf{v{�k�c��   � ������������`�������� � � |   �����l�����l�l�����l�`@��  ���  ������|pm8    ����  ~�~�f�f�~�~�f�    ����А����֖֖������    �����v��v�v�V�V�v�v     �|�}�m�l�l�l�o��| m�c  �������ަ���V��������     � �����˘�������~�����x�0��   ` `�����`����� �`�����`�`��   ` `|`|�m�o�n�l m�}�}�m�a���  �~�~������ �~�~�f�fx~p~v`��   � �����������ߌ�����������   ����������������������� |   ���������������������� �   ` �{�{�kkvkvkVkV{V{vkvc  ����m�l m�m�m�|�|�l``��  �������������d�l�8�0��  ��� �����0�����0�0���� ��    ����Ѱմմմ׼Ӹ��Ѱ��   00�0�����0�0�0�x�l��ӆ� �~  �����π���p��˼� ����ɰ�6>  0�6��˸�0�6˾��`�`���� ` ` `   0�6�6�6����������������p��  �������0����ٰٰ�����0 0 0 0  ����6�<�8�p�v�>��`���� ` ` `  ���<� ـ���~�����80   ` `��������� ������ɐ��  ����јј��n�`�������xnf `   �~�~�f�f�~�~|�<��>���0<�8�   `����Ɉ���p��ˎ�����ɘ�   �����������ѤѬ���͐ɘ���   ��������� �`�����`���� ` ` `  00�0�>־���F�&׶���ц�<    0������0�0����� ����ٌ���    �0�  ?�0?�0?�0?�?�0p  ���`�����`����٘����٘��    ���� ������������ٸ 0��  08������h�x�p�l�x�l�xv��  �� ?�1�?�  ���� ?�1�?� x p   0y��i�o�o�i�i�i�{�{�i�a��   ` `�����`����ѰӼ����Ѳ�0p`   `l�l���`������������    ����l���l�l�����`�����`��  ��z�j�j�o�j�j�o��z�j�f��	�    ����������`�|�|�`�`��>   ��������֬֬������    >�>�6�>�>������`08p   �����������`��ۘ�`�����` � �    ����������������ٶ�>   0 0�����ٰٰ�����0���� 0 0 0   `����`��������0�`���� ` � �  }�}��}�}��;�3?�?�0000?�?�  ~�x�����~�~�����l�l�l�l��   ` `������������`�l�<��    ���������������V�V�VF�	�   � ���������� �����`���� ` ` `   �����f�f����������  8 0   �����l�l� ���������� ���   ����6ٶـ�����0�`���� ` � �   l l{�{�h`k�k�klk�{l{�k�clll  ��������� �����`���� ` ` `   `����� ��ٌ��� ������0 0 � �  ���`���l�l���l���`�`�� �8  �����������������ٰ�>   �|�|L3Lf|  ���� ?�1�?�1� 0   ��`�������n�f������ټ�   ` `����� ��� ��� ��������    ����l����ۜ�������٘���   �����0�����l�����l����l|  �����������������٘�   ` `�����n�f�����0�������   ���������٘�������ژ�� p�  ��` o�` ��l�op`o���O�    ������� �����l����� ��   � �y�{�k�i�i�k�j`{�{�h�a�   ` `�����`�l�l�����`�����   ` �����������`�����`�ln� �   ~�x��X�^�^�X�X�~�~Ӏ��>   ` `���������� �����Ѱ�>   �����������ٖ������ٖ���  ���ј�l���������ϴʹ�� 0  ���a�o�a��a�o�a�o�lL���   ` `�l�l�l�l�v���`