// IawD3dWrapper.cpp App Wizard Version 2.0 Beta 1
// ----------------------------------------------------------------------
// 
// Copyright © 2001 Intel Corporation
// All Rights Reserved
// 
// Permission is granted to use, copy, distribute and prepare derivative works of this 
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  This software is provided "AS IS." 
//
// Intel specifically disclaims all warranties, express or implied, and all liability,
// including consequential and other indirect damages, for the use of this software, 
// including liability for infringement of any proprietary rights, and including the 
// warranties of merchantability and fitness for a particular purpose.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

// ----------------------------------------------------------------------
//
// PURPOSE:
//    
// IawD3dWrapper.cpp: implementation of the CIawD3dWrapper class
//
// ----------------------------------------------------------------------
// Authors: Kim Pallister,Dean Macri - Intel Technology Diffusion Team
// ----------------------------------------------------------------------

#include "stdafx.h"

const int IawD3dWrapper::MAX_TEXTURE_FORMAT_TRIES = 10;

// Constructor
IawD3dWrapper::IawD3dWrapper(HWND hWnd) :
  mpDevice(NULL),
  mFrameTime(0.0f),
  mInitialized(false)
{
  mRenderWindow = hWnd;

  mColBackground = RGB_MAKE(0x00, 0x00, 0x00);
  mpFirstDisplayAdapter = NULL;

  mpDirectX8 = Direct3DCreate8(D3D_SDK_VERSION);

  //identify the list of adapters
  UINT adapter_count;
  adapter_count = mpDirectX8->GetAdapterCount();

  mpFirstDisplayAdapter = new IawDisplayAdapter;

  IawDisplayAdapter* temp_adapter = mpFirstDisplayAdapter;

  mCurrentMode = -1;

  for (UINT i=0;i<adapter_count;i++)
  {
    UINT  mode_count;
    D3DADAPTER_IDENTIFIER8 temp_iden;
    mpDirectX8->GetAdapterIdentifier(i, D3DENUM_NO_WHQL_LEVEL, &temp_iden);

    //next three lines for debugging only
    char  str[MAX_DEVICE_IDENTIFIER_STRING];
    sprintf(str, temp_iden.Description);
    OutputDebugString(str);
    OutputDebugString("Found adapter: \n");

    temp_adapter->mAdapterNum = i;
    strcpy(temp_adapter->mStrDesc, temp_iden.Description);
    memcpy(&temp_adapter->mD3dAdapterIdentifier8, &temp_iden, sizeof(D3DADAPTER_IDENTIFIER8));
    temp_adapter->mpNext = NULL;

    temp_adapter->mpFirstDisplayMode = new IawDisplayMode;
    IawDisplayMode* temp_mode = temp_adapter->mpFirstDisplayMode;

    mode_count = mpDirectX8->GetAdapterModeCount(i);
    for (UINT j=0;j<mode_count;j++)
    {
      mpDirectX8->EnumAdapterModes(i, j, &temp_mode->mD3dDisplayMode);
      temp_mode->mpNext = NULL;

      //just for debugging
      sprintf(str, temp_iden.Description);
      OutputDebugString("Found mode on ");
      OutputDebugString(str);
      if((i==0) && (mCurrentMode == -1) && 
         (temp_mode->mD3dDisplayMode.Width == 640) &&
         (temp_mode->mD3dDisplayMode.Height == 480))
        mCurrentMode = j;
      sprintf(str, " %d %d  %d  %d \n", temp_mode->mD3dDisplayMode.Width, temp_mode->mD3dDisplayMode.Height,
              temp_mode->mD3dDisplayMode.RefreshRate, temp_mode->mD3dDisplayMode.Format);
      OutputDebugString(str);

      if (j<(mode_count-1))
      {
        temp_mode->mpNext = new IawDisplayMode;
        temp_mode = temp_mode->mpNext;
      }
    }

    //now need to enum list of modes

    if (i<(adapter_count-1)) //then we have at least one more node to add to the list
    {
      temp_adapter->mpNext = new IawDisplayAdapter;
      temp_adapter = temp_adapter->mpNext;
    }
  }

  mCurrentAdapterNumber = 0;
  if(mCurrentMode == -1)
    mCurrentMode = 0;
  mD3dDeviceType = D3DDEVTYPE_HAL;
  mVertexProcessingType = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
}

IawD3dWrapper::~IawD3dWrapper()
{
  if(mpDevice)
    mpDevice->Reset(&mD3dPp);
  SAFE_RELEASE(mpDevice);
  SAFE_RELEASE(mpDirectX8);
}

HRESULT IawD3dWrapper::Initialize(int adapterNumber,
                                  int mode,
                                  bool fullScreen,
                                  UINT x,
                                  UINT y,
                                  UINT width,
                                  UINT height,
                                  DWORD bpp,
                                  DWORD backBufferCount,
                                  bool useZBuffer)
{
  //Need to add handling of failures
  HRESULT hr;
  IawDisplayMode* mode_ptr = NULL;
  
  int z_formats_cnt = 0;
  D3DFORMAT* z_formats = new D3DFORMAT[MAX_TEXTURE_FORMAT_TRIES];
  int idx = 0;
  
  mInitialized = false;
  if(mpDevice)
    mpDevice->Reset(&mD3dPp);
  SAFE_RELEASE(mpDevice);
  
  if(adapterNumber != (int)mCurrentAdapterNumber)
  {
    mCurrentAdapterNumber = adapterNumber;
    
    int num_modes = GetNumModes();
    mode = 0;
    for(int i=0; i<num_modes; i++)
    {
      mode_ptr = GetMode(i);
      if((mode_ptr->mD3dDisplayMode.Width == 640) &&
         (mode_ptr->mD3dDisplayMode.Height == 480))
      {
        mode = i;
        break;
      }
    }
    mCurrentMode = mode;
  }
  else
  {
    mCurrentAdapterNumber = adapterNumber;
    mCurrentMode = mode;
  }
  
  mFullScreen = fullScreen;
  mBpp = bpp;
  mBackBufferCount = backBufferCount;
  mUseZBuffer = useZBuffer;
  
  if(!mFullScreen)
  {
    RECT rc;
    mWindowWidth = width;
    mWindowHeight = height;
    mX = x;
    mY = y;
    if(IsZoomed(mRenderWindow))
      SetWindowLong(mRenderWindow, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE|
                    WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|
                    WS_MAXIMIZEBOX|WS_MAXIMIZE);
    else
      SetWindowLong(mRenderWindow, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE|
                    WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
    SetWindowPos(mRenderWindow, HWND_NOTOPMOST, mX, mY, mWindowWidth, mWindowHeight,
                 SWP_NOCOPYBITS | SWP_SHOWWINDOW);
    GetClientRect(mRenderWindow, &rc);
    mWidth = rc.right - rc.left;
    mHeight = rc.bottom - rc.top;
    HDC hDCScreen = GetDC(NULL);
    bpp = mBpp = GetDeviceCaps(hDCScreen, BITSPIXEL);
    ReleaseDC(NULL, hDCScreen);
  }
  else
  {
    // Get the width and height from the mode
    mode_ptr = GetMode(mode);

    mWidth = mode_ptr->mD3dDisplayMode.Width;
    mHeight = mode_ptr->mD3dDisplayMode.Height;

    char szTemp[50];
    sprintf(szTemp, "Switch to FS: %d x %d\r\n", mWidth, mHeight);
    OutputDebugString(szTemp);
  }
  
  D3DPRESENT_PARAMETERS d3dpp; 
  ZeroMemory(&d3dpp, sizeof(d3dpp));
  
  d3dpp.Windowed = fullScreen ? FALSE : TRUE;
  
  //figure out color format
  if (mFullScreen)
  {
    
    d3dpp.BackBufferFormat = mode_ptr->mD3dDisplayMode.Format;
  }
  else
  {
    D3DDISPLAYMODE d3ddm;
    hr = mpDirectX8->GetAdapterDisplayMode(mCurrentAdapterNumber, &d3ddm);
    d3dpp.BackBufferFormat = d3ddm.Format;
  }
  mD3dFormat = d3dpp.BackBufferFormat;
  
  //figure out z format
  d3dpp.EnableAutoDepthStencil = useZBuffer ? TRUE : FALSE;
  switch(bpp)
  {
  case 16:
      z_formats[z_formats_cnt++] = D3DFMT_D16 ;
      z_formats[z_formats_cnt++] = D3DFMT_D15S1;
      z_formats[z_formats_cnt++] = D3DFMT_D16_LOCKABLE;
      z_formats[z_formats_cnt++] = D3DFMT_D32;
      z_formats[z_formats_cnt++] = D3DFMT_D24S8;
      z_formats[z_formats_cnt++] = D3DFMT_D24X8;
      z_formats[z_formats_cnt++] = D3DFMT_D24X4S4;

      while ((idx < z_formats_cnt) && (FAILED(mpDirectX8->CheckDepthStencilMatch(mCurrentAdapterNumber,
                mD3dDeviceType,
                mD3dFormat,
                mD3dFormat,
                z_formats[idx]))))
      {
        idx++;
      }

      d3dpp.AutoDepthStencilFormat = z_formats[idx];
      break;
    case 32:
      z_formats[z_formats_cnt++] = D3DFMT_D32;
      z_formats[z_formats_cnt++] = D3DFMT_D24S8;
      z_formats[z_formats_cnt++] = D3DFMT_D24X4S4;
      z_formats[z_formats_cnt++] = D3DFMT_D24X8;
      z_formats[z_formats_cnt++] = D3DFMT_D16;
      z_formats[z_formats_cnt++] = D3DFMT_D15S1;
      z_formats[z_formats_cnt++] = D3DFMT_D16_LOCKABLE;
      
      while ((idx < z_formats_cnt) && (FAILED(mpDirectX8->CheckDepthStencilMatch(mCurrentAdapterNumber,
                mD3dDeviceType,
                mD3dFormat,
                mD3dFormat,
                z_formats[idx]))))
      {
        idx++;
      }
      d3dpp.AutoDepthStencilFormat = z_formats[idx];      
      break;
    default:
      d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  }
  
  delete [] z_formats;
  
  d3dpp.BackBufferCount = 1;
  d3dpp.SwapEffect = fullScreen ? D3DSWAPEFFECT_FLIP : D3DSWAPEFFECT_DISCARD; 
  
  d3dpp.BackBufferWidth = mWidth;
  d3dpp.BackBufferHeight = mHeight;
  d3dpp.hDeviceWindow = mRenderWindow;
  
  if (mFullScreen)
  {
    d3dpp.FullScreen_RefreshRateInHz = mode_ptr->mD3dDisplayMode.RefreshRate;
  }
  else
  {
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT ;
  }
  d3dpp.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT ;
  d3dpp.Flags = 0;
  
  if (mFullScreen)
  {
    SetWindowLong(mRenderWindow, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE);
    
    hr = mpDirectX8->CreateDevice(mCurrentAdapterNumber,
                                  mD3dDeviceType,
                                  mRenderWindow,
                                  mVertexProcessingType,
                                  &d3dpp,
                                  &mpDevice);

    mWidth = d3dpp.BackBufferWidth;
    mHeight = d3dpp.BackBufferHeight;
  }
  else
  {
    if(IsZoomed(mRenderWindow))
      SetWindowLong(mRenderWindow, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE|
                    WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|
                    WS_MAXIMIZEBOX | WS_MAXIMIZE);
    else
      SetWindowLong(mRenderWindow, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE|
                    WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
    
    hr = mpDirectX8->CreateDevice(mCurrentAdapterNumber,
                                  mD3dDeviceType,
                                  mRenderWindow,
                                  mVertexProcessingType,
                                  &d3dpp,
                                  &mpDevice);
    
    SetWindowPos(mRenderWindow, HWND_NOTOPMOST, mX, mY, mWindowWidth, mWindowHeight, SWP_NOCOPYBITS | SWP_SHOWWINDOW);
    
  }

  OutputDebugString("init'ing device\r\n");
  
  if (D3D_OK != hr)
  {
    char szTemp[50];
    sprintf(szTemp, "Problem: %8lx\r\n", hr);
    OutputDebugString(szTemp);
  }
  
  mD3dPp = d3dpp;
  
  //pEnum->Release();
  
  if (mpDevice)
    mpDevice->SetRenderState(D3DRS_ZENABLE, useZBuffer ? TRUE : FALSE);
  
  // Pick the appropriate timing device
  LARGE_INTEGER frequency;
  if(QueryPerformanceFrequency(&frequency))
  {
    mHighPerf = true;
    mFreq = (double)*(__int64 *)&frequency;
  }
  else
  {
    mHighPerf = false;
  }
  if(D3D_OK == hr)
    mInitialized = true;
  return hr;
}

//should probably change this to check validities
HRESULT IawD3dWrapper::ChangeDeviceOptions(D3DDEVTYPE deviceType, DWORD vertexProcessingType)
{
  if ((mD3dDeviceType != deviceType) || (mVertexProcessingType != vertexProcessingType))
  {
    mD3dDeviceType = deviceType;
    mVertexProcessingType = vertexProcessingType;
    UINT x, y;
    
    if(!mFullScreen)
    {
      RECT rc;
      GetWindowRect(mRenderWindow, &rc);
      x = rc.left;
      y = rc.top;
    }
    return Initialize(mCurrentAdapterNumber, mCurrentMode, mFullScreen, x, y,
                      mWindowWidth, mWindowHeight, mBpp, mBackBufferCount, mUseZBuffer);
  }
  
  return D3D_OK;
}

double IawD3dWrapper::GetTime()
{
  if(mHighPerf)
  {
    LARGE_INTEGER cnt;
    Que ÃÃÃÃÃÃÃÃÃÃÃÃÃÃÉÏSWã˘ãGH3€;√t[É¿Pˇ(p ãGHÉ¿ Pˇ†p ∏   9GDtGãOHÉ¡ Qˇòp ãH;˚t&çWRˇ,p Ö¿uããPãœˇ“WË›ˇˇWË|	  Éƒ_∏   [Éƒ¬ G<ãGHÉ¿ Pˇòp Vj Ë	  ãÉƒ;ÛÑ≥   çFââ^â^â^â^âFâ ãO@Uãl$$â^â\$ç]!SâL$ËÓ  ÉƒÖ¿âÖâ   çXãFÖ¿tPˇ®p «F    ãÖ¿tPËÍ  Éƒ«    Ö€]uãGHVPË»ËˇˇãÿãFÖ¿tPˇ®p ãÖ¿t	PË∂  ÉƒVËß  ÉƒË˚ˇˇ^_ã√[Éƒ¬ ª   ËÊˇˇ^_ã√[Éƒ¬ ãT$ URÉ¿ PËp  ã∆D(  ãâiãâZããT$(«@   ãâQããL$âã3¿âBãÉƒh‹ì âAˇ(p ãã\$âBÈBˇˇˇÃÃÃÃÃÃÃÃÃUãÏÉ‰¯jˇh®` d°    PÉÏ0SVW°ê 3ƒPçD$@d£    ã˘3ˆçD$ ã»ât$ât$(ât$,ât$0ât$4âD$$âL$ ât$HãGH;∆t[É¿Pˇ(p ãGHÉ¿ Pˇ†p ∏   9GDtDãGHÉ¿ Pˇòp ãH;˛t&çWRˇ,p ;∆uããPãœˇ“WËÒ⁄ˇˇWË_  Éƒª   Èå   G<ãGHÉ¿ Pˇòp ãG@ât$8ât$ãuç^!SâD$Ë   ÉƒÖ¿âD$Öë   ãL$(Ö…çXtQˇ®p ãD$«D$(    Ö¿tPËˆ  Éƒ3¿âD$Ö€uãMçT$âHãGHRPËÀÊˇˇãÿË$Ôˇˇ3ˆ«D$HˇˇˇˇãD$(;∆tPˇ®p ãD$;∆t	PËß  Éƒã√ãL$@dâ    Y_^[ãÂ]¬ ãMVQÉ¿ PËy  ãT$(∆D2  ãD$(âpãL$(âYãT$(3¿âBãL$(ãUâQãL$(ãT$ âãL$(ãUâQãL$(Éƒh‹ì âAˇ(p ãT$ã\$âBãD$È+ˇˇˇÃÃÃÃÃÃÃÃÃÃÃÃÃÃUãÏÉ‰¯jˇhx` d°    PÉÏ(SVW°ê 3ƒPçD$8d£    ã˘3€çD$ã»â\$â\$ â\$$â\$(â\$,âD$âL$â\$@ãGH;√t[É¿Pˇ(p ãGHÉ¿ Pˇ†p ∏   9GDtAãGHÉ¿ Pˇòp ãH;˚t&çWRˇ,p ;√uããPãœˇ“WË·ÿˇˇWËO  Éƒæ   ÎWG<ãGHÉ¿ Pˇòp ãEãMãUPQRSã]ããKPQçt$,Ë°  ãÖˆuãSãD$çL$âPãWHQRËÛ‰ˇˇãËLÌˇˇ3€«D$@ˇˇˇˇãD$ ;√tPˇ®p ãD$;√t	PËœ  Éƒã∆ãL$8dâ    Y_^[ãÂ]¬ ÃÃÃÃÃÃÃÃÃÃÃUãÏÉ‰¯jˇhx` d°    PÉÏ(SVW°ê 3ƒPçD$8d£    ã˘3€çD$ã»â\$â\$ â\$$â\$(â\$,âD$âL$â\$@ãGH;√t[É¿Pˇ(p ãGHÉ¿ Pˇ†p ∏   9GDtDãGHÉ¿ Pˇòp ãH;˚t&çWRˇ,p ;√uããPãœˇ“WËÅ◊ˇˇWËÔ  Éƒæ   Èâ   G<ãGHÉ¿ Pˇòp ãw@j!â\$â\$4Ëô  Éƒ;√âD$Öó   ãL$ ;Àæ   tQˇ®p ãD$â\$ ;√tPËë  Éƒ3¿âD$;Ûu ãUâPãEãL$çT$âAãGHRPË\„ˇˇãËµÎˇˇ«D$@ˇˇˇˇãD$ ;√tPˇ®p ãD$;√t	PË:  Éƒã∆ãL$8dâ    Y_^[ãÂ]¬ SÉ¿ SPË  ãD$ àX ãL$ âYãT$ «B!   ãD$ âXãL$ âYãT$ ãMâ2ãD$ âHãT$ Éƒh‹ì âZˇ(p ãL$ãt$âAãD$È-ˇˇˇÃÃÃÃÃÃÃÃÃÃÃVãt$ÖˆtãFPËò  VËå  Éƒ^¬ ÃãD$Ö¿tPË|  Y¬ ÃÃÃÃÃÃÃÃÃÃÃÃÃÃWã˘ã«Ë6ÍˇˇÖ¿u	∏   _¬ VçwVˇ†p É Vtˇòp ËåÍˇˇ^∏”  _¬ ãD$âG«G    ˇòp ËiÍˇˇ^3¿_¬ ÃÃWã˘ã«Ë÷ÈˇˇÖ¿u	∏   _¬ VçwVˇ†p É Vtˇòp Ë,Íˇˇ^∏”  _¬ ãD$ãL$«G    âGâOˇòp ËÍˇˇ^3¿_¬ ÃÃÃÃÃÃÃÃÃÃÃãFÖ¿tPˇ®p ãÖ¿tPËÄ  Y√ÃÃÃÃQãL$SU3€3Ì;ÀW~ãD$$É¿(É¿ÉÈuˆç}!Wâ^â|$Ë  Éƒ;√âuø   Î$ˆD$tHSSSSˇ0p ;√âFu7ˇ4p ã¯;˚t"ãF;√t
Pˇ®p â^ã;√tPË¯   Éƒâã«_][Y¬ 9\$(~7ã|$$ãD$(É«âD$(ããW¸ãQRçL QËº	  ÉƒÉ«Él$(uﬁã|$3€ã∆D*  ãâhããD$âyãâBããT$ âQããL$âããD$âBãh‹ì âYˇ(p ã_]âB3¿[Y¬ Ã∏   ¬ VWã|$ÎWË"   Ö¿YtWË   ãÖˆYtË_ã∆^√È÷ˇˇˇˇ%Lq ˇ%Hq ;ê uÛ√È£  Ãˇ%Dq ˇ%@q ˇ%<q Sä\$ˆ√VãÒt$