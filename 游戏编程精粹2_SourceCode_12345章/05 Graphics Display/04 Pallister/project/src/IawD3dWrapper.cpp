// IawD3dWrapper.cpp App Wizard Version 2.0 Beta 1
// ----------------------------------------------------------------------
// 
// Copyright � 2001 Intel Corporation
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
    Que �������������̃�SW���GH3�;�t[��P�(p �GH�� P��p �   9GDtG�OH�� Q��p �H;�t&�WR�,p ��u��P����W����W�|	  ��_�   [��� G<�GH�� P��p Vj �	  ����;���   �F��^�^�^�^�F� �O@U�l$$�^�\$�]!S�L$��  �������   �X�F��tP��p �F    ���tP��  ���    ��]u�GHVP������؋F��tP��p ���t	P�  ��V�  �������^_��[��� �   �����^_��[��� �T$ UR�� P�p  ��D(  ��i��Z��T$(�@   ��Q��L$��3��B���hܓ �A�(p ��\$�B�B������������U����j�h�` d�    P��0SVW�� 3�P�D$@d�    ��3��D$ �ȉt$�t$(�t$,�t$0�t$4�D$$�L$ �t$H�GH;�t[��P�(p �GH�� P��p �   9GDtD�GH�� P��p �H;�t&�WR�,p ;�u��P����W�����W�_  ���   �   G<�GH�� P��p �G@�t$8�t$�u�^!S�D$�   �����D$��   �L$(�ɍXtQ��p �D$�D$(    ��tP��  ��3��D$��u�M�T$�H�GHRP��������$���3��D$H�����D$(;�tP��p �D$;�t	P�  ���ËL$@d�    Y_^[��]� �MVQ�� P�y  �T$(�D2  �D$(�p�L$(�Y�T$(3��B�L$(�U�Q�L$(�T$ ��L$(�U�Q�L$(��hܓ �A�(p �T$�\$�B�D$�+�����������������U����j�hx` d�    P��(SVW�� 3�P�D$8d�    ��3ۍD$�ȉ\$�\$ �\$$�\$(�\$,�D$�L$�\$@�GH;�t[��P�(p �GH�� P��p �   9GDtA�GH�� P��p �H;�t&�WR�,p ;�u��P����W�����W�O  ���   �WG<�GH�� P��p �E�M�UPQRS�]��KPQ�t$,�  ����u�S�D$�L$�P�WHQR��������L���3��D$@�����D$ ;�tP��p �D$;�t	P��  ���ƋL$8d�    Y_^[��]� �����������U����j�hx` d�    P��(SVW�� 3�P�D$8d�    ��3ۍD$�ȉ\$�\$ �\$$�\$(�\$,�D$�L$�\$@�GH;�t[��P�(p �GH�� P��p �   9GDtD�GH�� P��p �H;�t&�WR�,p ;�u��P����W����W��  ���   �   G<�GH�� P��p �w@j!�\$�\$4�  ��;ÉD$��   �L$ ;˾   tQ��p �D$�\$ ;�tP�  ��3��D$;�u �U�P�E�L$�T$�A�GHRP�\����������D$@�����D$ ;�tP��p �D$;�t	P�:  ���ƋL$8d�    Y_^[��]� S�� SP�  �D$ �X �L$ �Y�T$ �B!   �D$ �X�L$ �Y�T$ �M�2�D$ �H�T$ ��hܓ �Z�(p �L$�t$�A�D$�-��������������V�t$��t�FP�  V�  ��^� ̋D$��tP�|  Y� ��������������W�����6�����u	�   _� V�wV��p � Vt��p ����^��  _� �D$�G�G    ��p �i���^3�_� ��W�����������u	�   _� V�wV��p � Vt��p �,���^��  _� �D$�L$�G    �G�O��p ����^3�_� ����������̋F��tP��p ���tP�  Y�����Q�L$SU3�3�;�W~�D$$��(����u��}!W�^�|$�  ��;Éu�   �$�D$tHSSSS�0p ;ÉFu7�4p ��;�t"�F;�t
P��p �^�;�tP��   �����_][Y� 9\$(~7�|$$�D$(���D$(��W��QR�L Q�	  �����l$(uދ|$3ۋ�D*  ��h��D$�y��B��T$ �Q��L$���D$�B�hܓ �Y�(p �_]�B3�[Y� ̸   � VW�|$�W�"   ��YtW�   ����Yt�_��^�������%Lq �%Hq ;� u���  ��%Dq �%@q �%<q S�\$��V��t$