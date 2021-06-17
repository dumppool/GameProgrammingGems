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
    DWORD dwType;               // type of the opera0`?``àüşüflnlÿlÿlflnlÌÌœ  ``ş`ÿüüşlşlÚlşll<l~ÌúÌÙœ  ``~`~üfü~lfl~l~l`lll~ÌvÌaœ  ?ø?ø`?ü1Œ?ü1Œ?ü1Œ0€?ü><x8   `ş`6`üşüşlÖlşlÖlÖlşlÖLÖÌ×œÁ  00>0~0Ì~ş¶m¶m¶¶¶nÌ3cşÁü  ``ş`şüüşlşlÆlşÌşÌm¼m¸lÏşÇü  >0>020>ş ş66c6k6k6kff6Æá¼  0Û0Û0ÿ~ÿ~6ÿ¶ÿ¶c6ÿ¶6f>f0Æ?Şyœ  ``ÿ`ÿü ü~l~lfl~l~ll^ÌÚÌ¹œ;  ``~`~üüÿlÿlflÛl~l~lÌÌœq  ``ÿ`ÿü6ü6lÿlÿl l~l~lfÌ~Ìœ   000~ÿ~66 666U6W¶ÿæğŞŒ  ``ÿ`ÿüZüZlÿl½ll<l>l{ÌØÌÙœ  6`6``6ü>üllmlmllllÌÜ˜  l`l``lümülçll~lfl~lfÌ~Ì~œg   0ÿ0ÿ0şÿşÿ6Õ6İ6Õ6Õ6İfÕfÿFÿŞÀœ   000ÿşş~6Z6~6Z6~6ffFŞğœ   0ÿ0Á0İ~Á~İ6 66c66cffcÆa  øøøø `~`fş~şff~fgfÿÎñŒ  f0f0ÿ0ÿ~f~~6f6~6f6ÿ6ÿ6jfqf~Ş~Ü   `ş~şşmì9¸ş0şşÎ:`9üyüØl˜Ì9œ3  j0ÿ0ÿ0jşnş`66 666kfkffvŞbœ   0?030?ş ş6c66c6&cff6FsŞ`œ  60v0ã0Ûş6ş>6 6÷6÷636wf3fÿF3Şvœ   `}hül`şşlØfmümülllÌİœ›  0Z´Z´<x~üfÌÿşÿşÃ?ø?øxpp  `ş`øøø ÿş3oö008à   0÷°÷°µşsşı¶>6ó¶É¶66fsffxŞ`Ü   0ÿ040ÿ~µ~ÿ6 6~6 6ÿ¶ÿ¶.fmfÜŞœ  `şøøÿhfèÛh~úÎÿ† øø8xàp  000~~ÿ¶c66c66cff6ÆcA  60ÿ°60>ş ş6m6666fffÎpÌ  |ü|Ìü|0aş}¶ş<4;ş ?ø?øxpp  j0Ú0Ô0jş~şZ6~6Z6~6fÿfÿF>Æ[ŞØœ  ``~`ş`Øüşü4l~lşlìl~lll~llÌ~Ü`˜  00>0~0Ì0ÿ~[~606Û6>&ïf=fìFÜŞœ  j0ÿ0j0nş`ş6 66c6&cff6FæŞÃœ  0~00ÿ°ÿşf~ÿ666ÿ6ÿ6Ã&ûfëFÿŞÆœ  0ÿ0ø0Zşÿşÿ6z6Ù6ÿ66kffkFŞcœ  0Z00~~ÿ¶ÿ¶k6Õ¶<6fæFmŞÜœ  0006~ÿşÿ¶m66m6fffF~Şqœ  000~m~~6m66h6}6nfwfoÆöŞÎœ  00ÿ°ÿşc~{6k66 6~6ff~fgFÿŞøœ  0 0üşü6´vüî´Œ´Düÿ UdUjUjUä«d<  60ÿ0ÿ06şş[66[666ÿ&ÛfÿFÃŞÇœ  ~00ÿşÿşZ6~6Z6~ffÆŞÿœ  ffff   0ÿ00ÿ0mşşm6666fÿfÛFÛŞ œ  000şmş~6m66m66m66Ê¦ÙnïL   0°j0~j~6m66m66l6¶íæ¿Î¡Œ  3ïŞÄ¾?ôğçÌfÜgöfæ ?ü?ü<<08  0ÿ°ÿ°U~w~66¶66¶66ÿ¶ÿ¦6fûN1Œ  60ÿ°ÿ°6~w~U6w6l6ÿ6ÿ6l6flFŞ`œ    üü0`À       | 8    üü0p`Œ   < 8    üü0`Ïìì  ìì  < 8    üü0lßŒŒŒŒ¬ì0ì`< 8    üü0cÃŒì8l0 < 8    üü2fÌÌ?ì?ì  < 8     ü?üclFlÌÌ1Œc8x 0    üü4gŒ  ÌÌ>8 | x    øø3cËO˜Ø~>1ğ!à    üü0lÌÌÌlìÌ < 8    üü0aŒÙŒÌìlìì < 8    üü3cßìììì < 8    üü0Ì_ÌÌÌü8 şü     ü?ücl[lÌÌŒ3ìl8x p    üü0fÇììl6ìÌŒ< 8   ü?üpàÍŒ7,3,7¬=¬0,?ì?ì      üü0ìßìLL?ì?ì<8    üü3{lÛl?ü?üÌl3,#<8    üü<oìßì?ü?ülìì < 8    üü3gÍŒ8ì7l ÌÌÌü 8   ş?ş0cfßö?æ3f?æ3f?æ3f3n3l    üü3ìÌÌ;l?üLÌÌlü 8  `ş~şşÆ6âö<Ö ÖşÖ0ö>ÌÀÂ<ş|   ş?ş0æÀÆÆÆ ?æ2f?æ2n?ì   üü4~ìÖì,ìÌì>,l>lv<8   üü6lüÆlŒì?ü0<ìlì < 8   şş0~öÚ¶¾öö¶fö    şş4~öÖ–ööö>–>ööv    8à€     üü    ÀÀÀÌ8ÌxØØğàÀÀÀÆÆş|    ÀÀÀÄ~Ü~ğÀÀÀÀÆşÆfş|  ¶a¶clcla¶y¶xÀcşcşcNc6{N{şcş  ?`?`3`?d3n?x `ààfo~o<| ßşÇş    üü` ` ` ` ` ` ` ` ` ` üü    üüc cğgğn0lp`àa°gn` üü    üüa€m€oğğ}°mğm˜oøgğ` üü    üüa€oøoøm˜m˜m˜m¸m°a€` üü    üü` oğoğa€a€a€a€øø` üü    üüa€oøoø` gàgàf`fln|l8üü  üü` `xgàf gügüf`f`l`l`` şş    üü` oøoø`Àgøgø`Àoüoü` üü    üüf gøoøyxcXfØm˜cf8`0üü  üü` oøoøm˜oøm˜oøoøa€a€a€şş    üü` oøoøa€oøoøa°oøoø` üü    üüc f0oøoøc`oøoøf`n`l`üü  üü` d``høvhhfhgh~jfÎ` üü    üüa€øøm°¸{ìcàgpn8lüü    üük0k0oüoüc000k|{|` üü    üüa€m˜oøa€oøa€oğa€ø` üü    üüc`||c`oxoxc`||c`şş  üüf oø{XfØm˜c8` oğl0oğl0üü    üücÀnp}ÜxoXmXoXmXo8m0üü  ÿüÿüÄ`ß`ÄüßüÕlßlÕlßlÄîßÎÄ€ÿşÿş  üü`ÀgøfØgø`Àü` gøfØfØc0nş    üüi°mücü{°møa°mø}°yüa€şş    üümXox` oğm°oğm°øøa€üü  üüa€oøm˜üül0oğl0oğf`l0şş  üüa€üc0~üf0gğ` oxoxmXüü  üüa€gà~|{Ü` oxmXoxf0o8ihüü  üül`|rĞoğl0oğl0oğf`ül`üü  üüa€üa€oømXoøf0gğf0gğlşş  üüf0üfÀoü|ÀølÀoüloxlşş  üükXoxkXoxfÀoø|ÀølÀoølÀşş    üü0 0 0 0 0 0 0 0 0 ?ş?ş        üüf`f`f`f`fll|x8` ` üü    ?ü?ü0 6301à0À1à30640 ?ü?ü  üü` f gøoøhÀüüaÀc`f8lşş    üüaÀc`n8}Üxgğf0gğgğ` üü    üül0oğl0oğc üüf`cÀnpüü  üü`Àoüloül oün¬oüv¬f¬` üü  üü` c0oüc0a€üc oøcø` şş    üü` gğf0gğ` o|mlmlo|` üü  €€€€€€şş€€€€€€€    ğğ000ÿ°ÿ°00066   ğ?ø?    ÿüÿü         00000ÿşÿş00000000  ˜˜˜˜˜ÿşÿş˜˜˜1˜1˜a˜@   ğğ006>qœ€üü€€€€   ``?`<```ÿşÿş````0```@`    øø1€a€A€€şş€€€€€  €€üü€°00ÿşÿş00000`0  €ˆˆ	?ü?ü€€şş€€€€  €€şş€°°0ÿşÿş00ğğ0  06Ø6Ø6Ø6Øÿşÿş6Ø6Ø6Ø6Ø6Ø6Ø6Ø0    ŒŒŒŒŒŒüüa€a€a€a€aşaş  ÀÈÜ8ø{à[ÀÌüx€ÿşÿş€€€  0À0À0À3øûøøØ0Ø6Ş6Ş51˜336x40  ` aşaşa€a€a€a€üüŒŒŒÿŒÿŒ    ?ø?ø33?ø3?ø?øÀÿşÿş À À À  €üü  00x2Ì  €ÿşÿş€€€   üü ?ø?ø0?ø0?ø ÿşÿş    0`3ü3ü0ÌùÌû¼386?ş?ş6Ö6Ö6Ö=ş=¬  0`?ü?ü1Œ?ü?ü1Œ?ü?ü€şş€€  €üü€üü€1€€ÿüÿü`8p  €€şş€?ü?ü2L7ì1Œ7ì7ì1Œ1¼1˜  3;¸°?ü?ü1Œ?ü1Œ?ü?ü€ÿşÿş€€  33˜˜3?~3~?3ÿ˜ÿ˜jq˜˜  0Ø7ü7ü0ÀÿüşÌ7ü6Ì7ü6Ì?ş?ş31808   cØÛôğ8kÔø>ÀÚÖÛÖ€ÿşÿş€€€       `0       ``````~~~~```````     ÿşÿş  @`0       À ÀüÀüÀÌàÌğÌØÌÌÌÄüÀüÀÌÀÀÀ À À     üü  ?ø?ø000?ø?ø0    øø  ÿşÿş À`0     üü ?ø?ø00?ø?ø0 0 ` ` À    üü øø`˜a˜a˜˜``øø   üü øø`dXfØc˜fØlXøø`  €€şş€şşm¶ofgÆ}ö{¶cşş  00000ÿ¸ÿ¼ 4620000p0   000l0l08<c462l0l000 0  €ü€ğ0°°üü`gìflgì``     ü ü Ì Ì Ì Ì Ì Ì Ì Ü Ø À À À    ğğ000°°000ğà      øøxp  ÿşÿş     ~üxü`Ì`Ì`Ì`Ì`ÌfÌnÌ|ÜpØ À À À    ~ü~üÌÌÌÌÌÌÌÜşØğÀ À À   x?ø? 0 ?ş?ş0 7ø666p& fGüÃø   ~x~ffffffffffff~fv~l`0```   ~üxü`Ì`Ì~Ì~Ì`Ì`Ì`ÌfÌ|üpØ À À   ğğ0``Àÿü?ü0 7ğ606p6`6güÃü    ~ü~ü6Ì6ÌvÌîÌÌÌ~Ì~ÌfÜfØ~À~ÀfÀ    ||clcllclll`lfln|{xq` `   ~~fffÿfÿff2f3fnl` `   ~üpüfÌvÌ~ìnüfÜfÌ~ÌvÜØÀpÀ À  0 0~~ÿfÛff|fxfffs~|`|`p`   30?ü şş0øîlfàøğ  0 ?~~lfffffoflflfonÿlà` `   ~~fUfUfUfUfUfUfUnWìÿàø` `  ?ø?ø À}œà¸3ïğ  ğ0ğüø    6~w~ãfÌff;faæÿæÿf3n3l?`?`3`   À øÿøÿmşm mxmHmHmXoDn|lÏşÇü   ||cllclll lfl~|x|c``>`    w~U~wf ff fÿæÿæ0f?n?l```  7¼÷¼Æ´Ö´×´Ö´×´×´ö4Ö´´¼6°`0À0   ~~<fïfÍæf3æíæÍfon>lï`İ``  7Xã8Ş˜^ÿşnøİ  øø80     ?ş?ş0 0 0 0 0 0 0 0 ` ` À €     à?àğÀÿş?ş0 0 0 0 p ` à À     ?ş?ş0 7ø7ø6666ø6p6fgşÃü    üü` ` oüoü`À`À`À`À`À`ÀÁÀ€    ?ü?ü0 1€1€?ø?ø1˜1˜3#fLxÈp    ?ş?ş0 7ø7ø0À0ü?ü?À0À`Æ`ÆàşÀ|    ?ş?ş0 7ø7ø0À0À?ü?ü0À À`À@ÀÀÀ    ?ş?ş0 7ü7ü1€1€1ø1ø3#flxHp  ?ş?ş0 7ü7ü0060?ş?ş0ğ1°#0n0LpÀ`    ?ü?ü0 0À0À7ü7ü0À0Ğ0Ø0ÈoşoşÀ     ?ş?ş0Ğ0Ø0È7ü7ü0À0À1à30flÈ  şşa€oüoüc fÀoøoø`Àoüoü`ÀÀÀ€À    şş` oøoølÀoüoül`l`ovo>ÌŞÀÌ    şşc güoüm€aøaøa€aøaøa€Á€Á€    ?ş?ş303?ü?ü3`3l3x6pfàmæß~Ø<    şş``o`o`mlm|mxm`o`of`fÀ~À<    ?ş?ş6 7ø?ø<À0À?ü?ü0ÀlÌlÌÏüÏü    ?ş?ş0À7ø7ø0À?ü?ü0À7øgø`ÀÏüÏü    ?ş?ş0 ?ü?ü307ø7ø0À7øgø`Àßşßş    şş` o¬h¬j¬j¬j¬j¬j¬gmŒØœÀ    ?ş?ş60?ü?ü6Ø6à6Ø6ğfÜlğoÆØşØ|    ?ş?ş0À7ü7ü30?ş?ş0À?üoü`ÀÀÀÀÀ    ?ş?ş0 7ø6Ø7ø6Ø7ø7ø0Àgø`ÀOüÏü  ?ş?ş0À?ü?ü0À7ø6Ø7ø6Øgø`ÀoüÏüÀÀ  üü` oøloøloø` oø``şßşÀÀAÀ  şş` `ününÀjşjşjØnØnØ`Ø`ØÁ˜Á˜  ?ş?ş0 7ü0À?ü?ü6Ø?ş?ş6Øoü`ÀÇüÇü  ?ş?ş0 1°7ü1°1°?ş0 7ü67üfGü†    şşc`o|o|c`o|o|c`||c`Ã`Ã`  ?ş?ş0À7ü7ü67ü67ü7ü0À&ØlÌKÄÁ€  şş` o¬o¬m¬o¬m¬o¬m¬o¬`mŒİÜØØ  şşa€oøoøloøloø`ÀnìnüfØıÎÙ†  şş` f|f|oLoLf|oLoÌvüvLf|Æ|†L  şş`Ğ`Èoüc0f|oülÌoülÌoüÀÀ€À  şş` o|o|f0o|o|f0oxo|~öw¶Æ0Æ0  şş` h°e0o¾bfj”joob8flÌÆ˜   üü` üa€oøloøloøf oø[0ÁàŸ<  ?ş?ş0À?ü0À7ø7ø67ø67øfşÆÌ    şş`oŒ`>o¾iŒo¬o¬`,mŒÇÌŞ@    üü` o|koXk~o~hk8oxlÚÉA  şşf0üüf0oøm˜oøa€oøa€oøÁ€ßü    şşf0o¾f~oìfl¬f<g¼o˜m¼ÛîÛf  ?ş?ş67ø67ø307¸>¼3°6şl gøÆÇø  şşk0o4k4o0`şoşo0k8o8klolËÆË†  şş` i>¾i0o>i>o4i4ô`4J4Ñd€D    şşf~~f8o|~Şva€mŒl>ÏöÇà    şşf0v¾o>flìq¬w¬u¼w˜q¼ÑîÓÆ    şşaÀgp~<{ì` o|mlo|f0f8Ï|Íl    şşc0üc0gøfØgøfØoülÌlüÏŒÌ  3;˜şş` o0c>şoìm¬o¼mØß¾Áæ  şşk0o4`~o~k0oxkLoülÌoülÌÏüÀÀ  ?ü1€7ø6gøæØİÌşdÈnÜjÔnÜtÈ×îÍZ           `000øü        üü ÀÀÀ°°3wüoü    0000000`0`6lflfÌşÌşş ş     àà``fs~c< 00øøp    üüÀà<pû<ã 00ğ?ø  €€€?ø?ø€€ÿşÿş `0ø?ü?    00?ø?ø €€°˜1Œa„G„     00ğø  00006lfl~|şşÆÆ    ğğ00?°?°0ğğ0¶1¶ŞÌ    øøøøÿşÿş 0?ø  €şş€?ø1˜?ø1˜?ø?ø 0ø?ü?   @àğ şş`07Øàà  ?ø?ø   `0ø şş0ónÌ	0à€>   `ğğ`6Ì~ü€àü~ãÀ˜pÀ>   `ğğ`6Ì~ü€ğü~ïî  à  ?ø?ø  `ğğ0`6l~ü€àü~ól;ls6g6    ş{ìy¬şÿşşÆ1ü1ülÀıüıÜxüÌ   0 üv4vş şø4øüg0g´ÛxÛ¼ûtÿ€ş~    ?ø?ø00``ÀÀ€àxx`    øø00`@À€Àpø>`    ?ğ?ğ0`xx0à0Àcğ_>    üü ğğ00 1``ÀÃğ<    }ü}üÌlÌlÌ8ØØĞp6`"ğa˜Ç   p?ø?€0 0 ?ğ?ğ60603`3`aÀcğŞ>L    oüoümŒmŒmœmØlØ|ğüpÌøÜ    ?ø?øÿşÿş ?ğ?ğ`àÀü||   üü  şş øø°8àqğo¾   3033 üü øø03`aÀCà>  ğğ0`à?ø>8  ~ü~ü6Øx><vî`Æ  €üü€?ø1˜?ø€?ø?øpàÀü||   üüÌÌÌÿlÿll~x{0ÛpØØ9Œ3    ~ü~ü6l8~üvì  ~ü~ü6Ìx>|vî`Æ  ş şüfüflfl~lflfl~lf8g0ÿ8öîÄ   øø~ 3°üü`àà`ÀÀ>pp   üüÀĞ6ØfÌ  ?ğ?ğ`ÀÀüx<   ü6ücLÿL>LLXXp.0mpÌØ    ~ü~üf~ü~ü` ~ü~ü`Ì~x~0`xaşaÎ  ZşZğ<ÀÀ~ü~üÌ~Ì~ì¬˜38fìDF   üüc07àğx<ìà`àÀü||    üü|Ì|ÜÀü~ü~Ì0ì6øfØ~ü~î Æ    cüül`l~l`lx8l0xmüÍÎİ„  €=¼=¼1Œ=¼1Œ?ü€?ø?ø0Àğ|  ?ğ`À`~ø4Øp2ÈÿşÀğ0ğ0ü    üÿüÃl~lklÕl>x÷8>06x>ü7Î?†  6Ø6Øü`ü€?ø€ÿş6 >|6l?8ÿşæî      ?ü?ü0000000?ü?ü0    €€€şş€€øøøø    üü0`_ÌÌÌÌÌÌÌ < 8    øøøø øøxx0p    øøøø  üüp<`8    }ü}üdÌdÌdÌdÌdÌ|Ì|ÌdÌaŒ<8    ~ü~üfÌfÌfÌfÌfÌ~Ì~ÌfÜ`Ø À À À    øøøø  00`@   ~~ÌfÌfÌfÌfÌfÌ~ü~üfÌf     ?ø?øx8pp øøøø    }°}°e°e°e°e°e°}°}°ecf    ışışÌ0Ì0Ì0Ì0Ì0Ì0ü0ü0Ì0À0 p `    ÿşÿş  ?˜?˜1˜1˜?˜?˜1˜  x p     0?ü?ü  øøøø   ` `~`~dflflfxfpfàà`ff`f ~ <  €€?ü?ü1Œ1Œ?ü?ü€€€ €}şp|     şş  øø<lÌøø    ğğ06>xp øøøø    üü` ` gøgøffgøgø` ` üü   0|0|0l0l0mşmşl0l0|0|0l0`0 0 0    øøøø  ÿşÿş øø ğ à    ?ü?ü ìì ?Ì?Ì0Ì0Ì?Ì?Ì < 8    ûüûüÉŒÉŒÉ˜É˜É˜ÈĞøğø`ÈğÁ˜     À~`~`f f0fpfpfX~Ø~ØaŒ   ` `~`ügüflflflflfl~Ì~ÌaŒ<8    ü}ü|ll,lllLlÌ|Œ}Œmc 8 0    ûğûğË0Ë0Ë0Ë0Ë0Ë0û0û0Ë06>    øø à€€€€  ?ø?ø00?ø?ø    üü~`f`f`f`f`f`~`~`f` `şş   üğ|0|0l0mşmşl0|0|0l0 0 0 0    ùüùüØ`Ø`Ø`ÛşÛşø`ø`Ø`À` ` à À    üü0 0 ?ø?ø  ?ø?ø00?ø?ø   À À|ü}üo€o løløl0|`|Àm†a†ş ü    ğğ4`fÀ€øğğğ00ğ0     ü~ü~ÌfÌfÌfÌfÌfÌfÌfÌ~Ì~üfü Ì   0 0|`|`dÌeŒeøeød0d`|@|ÌeŒcş†    ü}ü|8lpl`oşoşl`|`|`l` ` à À    €Àpşïì@ ?ğ?ğ0000?ğ?ğ00  €€şş€€?ü?ü  øøøø    øøø€?ü?ü1Œ1Œ1Œ1¼1¸€     |}şmşlm˜mØ|Ø|l  8 0    üü`oìoì`gìgìflgìgì``<`    øø80v`#À€üü|üü   8?ø?À0 ?ş?ş0 0 ?ø?ø|lïøÏø  €€üü€?ø?ø1˜?ø?ø€ Àüşp<   ` `ü`ü`Ì`ÏüÏüÌ`Ì`ü`ü`Ì`À`Çşş    ?ü?ü007ì7ì6l6l7ì7ì00<08   üûàø`È`È`È~ÏşÏàÈ`ø`ø`Èf ~ <    üûüÈÀÈÀÈÀÈĞÈØÈÌøÄøÀÈÀÀÀ À À   0 0°}°}¾mşoöo¶m¶}¾}¼m°†ş ü    øøøø  ?ü?ü00?ü?ü0  Œ}Œ|ØdØd0d`d`d`|`|`d``` ` `    }ø}ølm˜m˜m˜mşmş|}ömö` <    À À ü}ü°o0l0oşoş|0|0l0 0 0 0   ` `|`|`mdmlmhl`lp|ğ|Øm˜    ü}ü|ÌoşoşlÌmümü|Ì|ÀmÀ€      ?ø?øÿşÿş?ø ø?ølOøø    üü|ÌlÌlÌoüoülÌ|Ì|ÌlÌ Ìşş   üü`À€àø>üğ00ğğ0  ?ø?ø  üü`0şö00ğğ0   ` `|p|Ğl˜mŒofl2mü}ü|l`0 0     ` x|l|ll`oşoşl`l`|ğ|ğm˜cŒ  0û0û0Û2Û6ÛüÛøÛ0Û0û0û0Ûpö¾   €mŒlìÌøğ  øøøø     `ğ0ğĞÀĞÀÒÌÒÆÖÆÖÂôÀğÌÀÌ ü x   ` `|ğ}øoœollØlØ|Ø|Øm˜˜    şş|`l`l`lìmìoæf|`l` ` ` `    üü€à¸yá†øøøø    ü{ü{lklklklkükü{ { kcşü   ` `ûşûşØ`Ø`ÛlÛlÛlûüûüØ`Àf ~ <    ù°ù°Ù˜ÛÛßşŞøøØøØØØÁ˜˜xp  °°}°}¶m¾o¼o¸mğmğ}°}°m´¶¾œ   €À`;8á_äà Àøøøø   ıüıàÍ€Í€ÍşÍşÍ˜Í˜ı˜ı˜Í˜Ã   À `şşl mømøm˜m˜}˜}˜m˜c   À Àù€ùÛÛüÛôÙ°Ù°ù°ù°Û0Æ6>   €?ü?ü0?ü?ü0 7ü7ü6fgüÇü†   À À|ÀşoşlÀlÀl°m°}°}lclüş Æ   `ø`ûşËşÈ`È`ËüËüÉŒù˜øğÈ`Àğ   ûìûìÈlÈlËìËËËìølølÈlÀlìÌ  ààl`oàoà` ğğ0ÿşÿş`0p`  ?ø?ø0?ø?ø  ?ø?ø üüÀ`x>`   ` `|`}hmlmfobn`ll|l|l0``À    ` `|`}şmşm¶m¶m¶mş}Î}†m††Œ   ` `|`}ümülllllloşş|plØŒ    {ü{üi˜i˜i¾i¾iÌiÌ{l{8c0fxÎ†  €€}€}şoşonll`l`|`|àm°c     ø|ø|ØmŞoo mümü|Ì|Ølp ø  €€ù€ùşÛşŞ¶Ø¶Ù¶Ù¶ûfúfØÆÁ†<    ØØhØi˜i˜i˜iØoØ˜y˜i˜aš   0 0}°}°müoüo0l0oşş|0l0 0 0 0  ?ü?ü øøşş  øøøø  €€ø?ø1€üü€øøøø    ü{ühi˜i˜kşkşhØxØi˜i˜cx 0   À À|ş}şo†olÆlælf|6|öoæ† > <    øøøø ?ü?ü00?ü?ü0    ûşûşË Ë|Ë|ËlËlûlûxË`Ã`f~>    ?ğ?ğ<8<1€ÿşÿş€øøøø   À à|`şoşm˜m˜m¸mğ|ğ|`lğø    øøøø€ÿşÿşà°qaŒ€  €˜}˜}°à9ğw¼ç@ ?ø?ø00?ø?ø  øøøø  ?ü?ü€øø€şş  ğğl0oğoğ` øø ÿşÿşp88p`  €€ø?øa€€şş  øøøø   ` `ø`ûüËüÈ`È`ËşËşø`øğÉ˜Ã    }˜}ØlØlm˜mØlØ|şcø       ü}ü| l oşoşl`l`|à|ÌmŒşş    üûüûÛÛüÛüÛ`Û`û0û0Æ     ØûØû˜Ù˜Ù˜ÛşÛşÙ˜ù˜ù˜Ù˜   ` `{ş{şkfkfkfkfkÖ{–{kc   ø?ø?  üü`88ÿşßşøø    ûøûøÈÀÈÀÏüÏüÈÀÈğù°ù°Ë06>  ˜˜ÿşÿşÉ˜È ËøËøÈ`øÀù€Ë üø   À ÀøÀøÀÏüÏüÈÀÈÀÉàù°û0ËÆŒÎD    ü{ü{ k˜kØkpk0k0kx{Ì{Œk cşş    ûşûşÛ`Û`ÛüÛüÛlûlûlÛlÆlÌœ  üûüûËlËlËlËlËlËlûløÀÉ°   ğğ00ğ  ?ø?ø0333px  øøø€üücfÌllx,``   À À|À|şmşo˜o˜m˜l˜|Ø|plp Ø   ` `üğüØÍŒÏÍøÍøÍ˜ı¸ı°Í€Í†ş ü   `ü}ü}ŒmŒm¼m¸m€}ş}şlcææ     `şûşûÛşÛşÛ Ûüûøû0Æ`Æ†şü    üûüøÛüÛüØÛìÛìûlûìÛì      ` `  ûşûşØÀÙ˜ÛøÛğø øhØÌüş    ` `ø`ûşÛşØ`ØğÙøÙøûlÿüÎöf ` `    üûüûÛüÛüÛ`Û`Ûşûşû0Û2ÖŞ    ü{ü{kükük kf{l{xk`f`f~<    üûüûÛlÛlßşßşÛûlûşÛş  < 8   `x`xği˜knÆh`køkøhx0y hà p 0   ` `ûüûüÛlÛüÛüÛlÛlÿşÿşÛ   00ó0ó~Ö~Ö¶×¶Ñ6Ó6Ö¶÷öğfÀŞœ     üûğû0Û0Û0ÛşÛşÛ0û0û0Ûºš~ n    ?ü?ü1Œ7ì1Œ7ì7ì07ì6lgìgìÀ€    üùüø Ø ÛşÛşØ`ÛlûlûlÆff à À    üûüûÛÛÛüÛüÙ°ù°ù°Ù¶6>   ` `ø`ø~Ø~Ø`Ø`ÛüÛüûûÛüü    ş÷şö¶Ö¶Ö¶Ö¶Ö¶×¾÷öÖşş   llûlûlÛlßşßşÛlÛlû|û|Û  şş    üûüûÛÛÛÛüÛüø ù˜Ùˆ    üûüø`ÛlÛlØ`ßşßşø`ø`Ø` ` ` `  ``o|o|l`ofÿ~ø<  ?ø?ø00?ø?ø   ÷ü÷ğÖĞÖĞÖĞÖĞÖØÖØöÈöÌÆÔöÒ    ØøØøØÈØÊÚÊşÊüÊØÊØúØúØÂØú   ` `ûøûøØ`Ø`ÛüÛüØàùğûxÃlÆfb `   À À|ş}şo†omömöm¶}¶}ömö       {ş{şhkìkìklklkl{ì{ìkl` < 8   ~÷ş÷¶Ò¶Ò¶Ö¶Ö´Ö”óœñÓ¸lÆ ‚  ş{ş{fkfkşkşkfkfkş{ş{fh` ` ` `    şş|0lpl`lèmìof|b|`l```şş   ``û`ûüßüŞ`Ø`ÛşÛşø`øğÙ˜    ü}ü}Œmül oşoşlÀ}ü}ül  < 8   ` `{ü{üklklkükü{l{ükücl ` ` `   <{ü{àh`klklklh`{ş{şh``` `àÀ   €À`øwŞ`>ø>ø6Ø6Ø>Ø>ø0À À    üûüûÛÛüÛüÛÛûüûüØ   şş   ` `|`}ümül`l`oşoş|À|ÌmŒœşæ    ü}ü}ŒmŒmümümŒmŒ}ü}ümŒaŒşş  ŒŒ}ŒşoşmŒmŒmŒmü}ü}ŒmŒüüŒ  ş÷şö Ö0ÖüÖüÖ´Ö´Ö´ö¼ö0Ö0Æ şş  ûPûPÛ~ßşßÖÛVÛVû^û\ÛPF~<   ` `ûfûfËfËfËşËşÈ`ûfûfËfÃşş   €?ü?ü0`şş  øøøø   À À}şşnmömöm¶m¶}ş}Œm€a†aş ü   @ `ø,øÌØÌØÈÚØÚÔÚöúâøÀÁÌÌü x   ° °{ş{şh¶kşkşj°kş{şx¶h¾¼00   ` `ø`ûüÛüÛlÛlÛlßşÿşøğÙØœ   <üûàø`Ø`ÛüÛüØ`ØğùøûlÇnf` `   À ÀyşyşkÀnÀhühühÀxÀxşhş`À À À   ~ xüüÌÿÌÿÌÌ>Ì{ÌØü˜üÌ     À ÀøÀùŒÙŒÛüÛşØûüûüÛÃüü  ```?xsl[d`şñş` øøø   À ğx0{àkàhfoloxip{p{xfnf	à À   Ì Ì|Ì}Œm¾o¾oŒm¬m¼}œ}ŒmŒaŒœ˜   ~û~ÿØßØÛÛÛØßØÿûÛ80    ~ü~üfÌ~ü~ü  àà``ff0~`<    üÿüù€ÙøÙøÙÛXÛXûXûÛÚÚ     À|À|üdüdÀdÀgşgşdÀ|Ğ|ØdÌ`Ì À À   ` `x`şoşh`h`kükü{{kcüü    {ŞoÖkVkVkVkVkV{V~VfŞÖ    À ÀøÀûşÛşÙ€Ù€ÙüÛüû0ÿ0Æ00şş   ` `ûşûşØÀÙ°Û°ßşßşù¶ù¶Ù¶¾¼ 0  €y˜yˆküküi°i´i¼y¸{0cpò¶   ÀøÀøøÉøË˜ÎğÈ`ÉØÏøàø0È À p    ` `ûşûşËÈ È ËşËşø0ø0È0 0 ğ à   ` `ûşûşÛÛØÀØØØüøğøÀØÆ Æ ş |  óò0Ö¤Õì×xÓÖ0÷¼÷¼Ğ   şş    şÿşù˜Ù˜Ù˜ßşßşÙ˜ù˜ù˜Ù˜    şÿşøÀÛüÛüÛÛÛüûüûÃüü  ğğ0ğ  üü0ğ0ğ>ÿşÿğ 0    şÿşù˜ÙøÙøÙ˜ÙøÙøù˜ùşÇş      >ø6Ø>ø  ?ø?ø  şş ø  ø ğ   `ğ`ölÓlÓxĞ`×ş×şÑ°ñ°ñ°Ó°6>   À À÷ü÷üÖĞ0ÓğÓÀĞü×ü÷ÀğÀĞÌ ü x    şÿşøÀÙ˜ÛüÛşØfØ`ûüûüØ` `şş   ` `ûşûşÈ`ËüÈlËüË`ûşøfÀì°   ÷Æ÷ÆÓÓÖ×ÖÖÖÜÖÚÖñ–ñ–Ó   €aü3üÌ Àà1°gDøøø  ˜øûüËüÈ`ËüËüÈ`ø`ÿşÏşÀ` ` `   ` `údûlÉhÈ`ËşËşÈğùøûlÆfb ` `   ||lclcl||l llfÆfÆc AÀÀş€>   Àø`ûşËşÈ É˜ËÊÉ˜ù˜ù°ÈàÀğ   ` `ûşûşÈ`È`ËüËüÈ ËüûûÃüü   ``û`ûüßüŞ`Ø`ÛşÛşøğùøÃlnf `   À ø}øo°làlğmo}ü}ŒmŒaüüŒ  €€}ş}şooömöm¶mö}ö}¶möö     ` ÀûüûüËËËüËËûüûËÃüü  ˜ø°ûüÛüØ`Ø`ÛüÛüø`øàÙ°   À `şşi€i˜køkğhlxÜ{¸kp`ØŒ  €ñ€÷ş×şÑ€Ñ°Ó6Ó´Ò¼ö°üpĞØŒ   ` `ûşûşÙ˜ßşŞöØ Ûüûüù€Ùø  x 0   <üûàø`ÛşÛşØ`Ø`ÛüûüûÛüü  °°ù¶ı¶ß¼Û¼Ù°Û¸ß¼ı¶ù²Ù°6>   Ø Èüü`ÀoÀoÌ`ÌoXkPk0or`ÚÁ   ` `ûşûşÛÛfØ`ÛşÛşù˜ù˜Øğ xÌ    şşø ûüÛüÛÛüÛüûûüÛü  şş  00ó0öüÖüŞ0Ş0ÖpÖxÖüö¶÷2Ö000    ğğ0°°üü`oìlloìoì``  ş{ş{kfkfkşkşkf{v{Şkcşş   À ÀûìûìØØßşßşØ`ØÌûüûğØÀ Æ ş |   óüóüÖlÖşÖşŞlÖlÖüöüölÖ`@À€  €şş  øøøø€Ø|pm8    øøøø  ~ş~şfÆfÆ~ş~şfÆ    ş÷şğĞ×ş×şÖ–Ö–×öö×şş    şûşûÛvÛÛvÛvÛVûVûvÛv     Ø|Ø}ümülØlØlØoşş| m˜c  €€ùşûşßŞ¦ØæÛVÛöû¶ûÛöö     À ÀùüûüË˜ÈğÈÀÏàË~øşûÌËxÀ0à€   ` `ûüûüØ`ßşßşØ Ø`ûüûüØ`À`şş   ` `|`|ğm˜oünöl mü}ü}ŒmŒaüüŒ  À~Ø~ÌÀÿşÿş À~Ì~ÌfØfx~p~v`Ş   À ÀûşûşØÀÙüÛüßŒİüùŒùüÙüŒœ˜   öØöØÖŞßşßúÖÚÖŞÖŞöØ×ØÏÆş |   ü÷àöÖğÖĞÖÖÖÜÖØöĞöØÜÈìÆ €   ` À{ş{şkkvkvkVkV{V{vkvc  ˜˜üüm˜l m˜m˜m˜||ğl``ğ  øûøûËËøËËøËøËdûlû8Ë0Ø  şşö öüÖüÖ0ÖüÖüÖ0ö0öüÖü şş    ü÷üñ°Ñ°Õ´Õ´Õ´×¼Ó¸ñ°ñ°Ñ°şş   00ó0ñşĞş×0×0Ó0ÓxólóÆÓ†Ç ş~  €ùüÿüÏ€ÈØÈpËäË¼È ÿüù°É°Ã6>  0û6û¼Ë¸Ë0Ë6Ë¾ËÈ`ø`ÿşÇş ` ` `   0û6û6Ë6ËşËşÈÀÉüËüúø˜ÈØÀpÀ€  €€ùşûşß0ßüÙüÙ°Ù°ûşûşØ0 0 0 0  °ù°û6Û<ß8ßpÛvÛ>Ûø`ÿşÇş ` ` `  €¼ÿ<ş Ù€ÙşÛ~ßßûûÛ80   ` `øàù°ËÎÌöÈ ËüûüøÀÉÃü  øñøñ˜Ñ˜Ñ˜ÓÖnĞ`÷ş÷şÑğÃxnf `   À~Ì~üfğfÆ~ş~|À<ğø>ïşğ0<ğ8à   `ûşûşÉˆÈØÈpÈØËÊù˜ù˜É˜˜   ÀğÀ÷ü×üÖÌĞÀÑ¤Ñ¬ó¸÷°ÍÉ˜ìÆ€   ÀğÀñ˜ÓüÓüĞ Ó`Óü×üö`÷şÇş ` ` `  00ó0ò>Ö¾×æ×FÒ&×¶÷–ğÑ†<    0°ùüûüÛ0Ø0ÛşÛşØ ùüùüÙŒüüŒ    ğ0ğ  ?ø0?ø0?ø0?ø?ø0p  üüø`ùüÙüØ`ßşßşÙ˜ÿşÿşÙ˜˜˜    üûüø ÛüÛüÛÛÛüûüù˜Ù¸ 0şş  08ûÿüßüÛhÛxÛpÛlûxûlŞxvş¼  üü ?˜1˜?˜  ÿşÿş ?˜1˜?˜ x p   0yô´i´oşoşi°i²iö{ü{˜i˜aºî   ` `óüóüĞ`×ş×şÑ°Ó¼÷¶õ¶Ñ²Ã0p`   `lûlùøÈ`ËüËüËËûüûËüÃ    üûüûlËüËlËlËüËüø`ûüËüÀ`şş  ŞŞzÚjÚjÚoÚjÜjÚoÚÚzÚjŞfØØ	˜    üûüûÛÛüÛüØ`Û|û|û`Û`àş>   şóşöŞìŞìÖ¬Ö¬ÖìöìöÖ    >ü>ü6Ì>ü>ü üüÀ`08p   ÀìùìøØÛşÛşØ`ÙøÛ˜ş`ûüÛüÀ` à À    üûüûÛüÛÛüÛÛüûüù°Ù¶¶>   0 0ûşûşÛÙ°Ù°ÙüÛüû0ûşÛş 0 0 0   `üûüø`ÛşÛşÛÙøØ0ø`ûüÛü ` à À  }ü}ğ€}ş}ş˜;˜3?ğ?ğ0000?ğ?ğ  ~óx÷à×àÓ~Ó~Óì×ì×lólólÃlÌŒ   ` `üûüûÛüÛÛüÛüû`ûlÛ<ØÎ    Æ÷ÆöÖ×Ö×ÖÓ×Ö×ÖóVóVÆVFÎ	Œ   Ø ØûŞûŞØØØØØ ÙüÙüø`ûşÛş ` ` `   şûàøÛfÛfØÛşÛşøøØØØ  8 0   şûğøÛlÛlØ ØÀßşÿşù˜Ùğ øŞ†   şûàø6Ù¶Ù€ÛüÛüØ0ø`ûşÛş ` à À   l l{ş{şh`küküklkü{l{üküclll  üüûûüÛÛüØ ÛüÛüø`ÿşÇş ` ` `   `ûşûşØ ÙüÙŒÙüØ ÛüûüøØ0 0 ğ à  şşø`ûüËlËlËüËlËüø`û`ÁÀ à8  ˜ù°ûüÛüÛÛÛüÛüù°ù°Ù°¶>   ÿ|ÿ|L3Lf|  ÿşÿş ?˜1˜?˜1¸ 0   üø`ÿşßşÙøßnŞfÛğûğù¼Ù¼Œ   ` `ûşûşÈ ËüÈ ËüÈ ûüûüËÃüü    ˜ûœûlØğÙøÛœßßşùøù˜Ù˜øø˜   Àøûøÿ0ßüÛüÛlÛüÛüûlûüÛül|  üüûûüÛüÛÛüÛüÛûüûüÙ˜œ   ` `øğù˜ÛnŞfÙøÙøØ0ûüûüÛüü   ÀøĞù˜ÛüÛÄÙ˜ÛŞÄÙøûøÚ˜ÀĞ pŞ  üü` oø` şşlØop`oøÌÌOø    øûøûÛøØ ÛüÛüÛlûüûüÛ şü   À Øy˜{üküi€iüküj`{ş{şhğa˜   ` `ûşûşØ`ÛlÛlßşŞöø`øğÙøœ   ` àùğû¸ßßşÙøØ`Ûüûüø`Ûlnæ À   ~÷xóÓXÖ^×^×XÑXõ~÷~Ó€Àş>   ` `÷ş÷şÖÑøÑøĞ Óüóüñ°Ñ°¶>   ÆÆ†ù–ù–ßÖßÖÙ–ÛÖÿÖı–Ù–†Œ  ñ˜ğğÑ˜Ól×ş×şĞÀñ°óüÏ´Í´´¼ 0  €üüa€oøa˜üa˜oøa€oølLÏøŒ   ` `ólólÓlÓlÕvÔâĞ`