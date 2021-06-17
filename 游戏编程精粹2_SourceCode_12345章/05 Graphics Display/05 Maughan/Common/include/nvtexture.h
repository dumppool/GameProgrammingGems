/******************************************************************************

  Copyright (C) 1999, 2000 NVIDIA Corporation
  This file is provided without support, instruction, or implied warranty of any
  kind.  NVIDIA makes no guarantee of its fitness for a particular purpose and is
  not liable under any circumstances for any damages or loss whatsoever arising
  from the use or inability to use this file or items derived from it.
  
    Comments:
		Texture routines to simplify reading/writing maps with vectors, colors, luminance, etc.

  - cmaughan@nvidia.com

  3/17/01 - Greg J.
     Changed name of this class to NVTexture2 to avoid name conflict with 
     the NVTexture of "nvtex.h"  

      
        
******************************************************************************/
#ifndef __NVTEXTURE_H
#define __NVTEXTURE_H

#include "singleton.h"
#include "nvdebug.h"

namespace nv_objects
{

class NVDataFormat
{
public:
	virtual void GetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD& dwValue) { assert(0); };
	virtual void SetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD dwValue) { assert(0); };

	virtual void GetColors(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float& fRed, float& fGreen, float& fBlue, float& fAlpha) { assert(0); };
	virtual void SetColors(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float fRed, float fGreen, float fBlue, float fAlpha)  { assert(0); };

	virtual void GetLuminance(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float& Luminance) { assert(0); };
	virtual void SetLuminance(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float Luminance) { assert(0); };
	
	virtual void GetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, D3DXVECTOR3& inVector) { assert(0); };
	virtual void SetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, const D3DXVECTOR3& inVector) { assert(0); };
};

class NVDataFormat_32Bit : public NVDataFormat
{
public:
	virtual void GetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD& dwValue)
	{
		dwValue = (*(DWORD*) (((BYTE*)LockData.pBits) + (j * LockData.Pitch) + (i * sizeof(DWORD))) );
	}

	virtual void SetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD dwValue)
	{
		*(DWORD*)(((BYTE*)LockData.pBits) + (j * LockData.Pitch) + (i * sizeof(DWORD))) = dwValue;
	}
};

class NVDataFormat_16Bit : public NVDataFormat
{
public:
	virtual void GetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD& dwValue)
	{
		dwValue = ((DWORD)*(WORD*)(((BYTE*)LockData.pBits) + (j * LockData.Pitch) + (i * sizeof(WORD))) );
	}

	virtual void SetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD dwValue)
	{
		*(WORD*)(((BYTE*)LockData.pBits) + (j * LockData.Pitch) + (i * sizeof(WORD))) = (WORD)dwValue;
	}
};

class NVDataFormat_8Bit : public NVDataFormat
{
public:
	virtual void GetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD& dwValue)
	{
		dwValue = ((DWORD)*(BYTE*)(((BYTE*)LockData.pBits) + (j * LockData.Pitch) + (i * sizeof(BYTE))) );
	}

	virtual void SetData(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, DWORD dwValue)
	{
		*(BYTE*)(((BYTE*)LockData.pBits) + (j * LockData.Pitch) + (i * sizeof(WORD))) = (BYTE)dwValue;
	}
};

class NVDataFormat_A8R8G8B8 : public NVDataFormat_32Bit
{ 
public:
	virtual void GetColors(const D3DLOCKED_RECT& LockData, float& fRed, float& fGreen, float& fBlue, float& fAlpha, DWORD i, DWORD j)
	{
		DWORD dwValue;
		GetData(LockData, i, j, dwValue);
		fBlue = ((float)(dwValue & 0xFF))/ 255.0f;
		fGreen = ((float)((dwValue >> 8) & 0xFF)) / 255.0f;
		fRed = ((float)((dwValue >> 16) & 0xFF)) / 255.0f;
		fAlpha = ((float)((dwValue >> 24) & 0xFF)) / 255.0f;
	}
	
	virtual void GetLuminance(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float& Luminance)
	{
		float fRed, fGreen, fBlue, fAlpha;
		GetColors(LockData, fRed, fGreen, fBlue, fAlpha, i, j);
		Luminance = ((fRed * 0.3f) + (fGreen * 0.59f) + (fBlue * 0.11f));
	}

	virtual void SetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, const D3DXVECTOR3& inVector)
	{
		D3DXVECTOR3 vecScaled = (inVector + D3DXVECTOR3(1.0f, 1.0f, 1.0f)) * 127.5f;
		BYTE red   = (BYTE)vecScaled.x;
		BYTE green = (BYTE)vecScaled.y;
		BYTE blue  = (BYTE)vecScaled.z;
		BYTE alpha = 0xFF;
		DWORD dwData = (DWORD)( ( (DWORD)alpha << 24 ) | ( (DWORD)red << 16 ) | ( (DWORD)green << 8 ) | ( (DWORD)blue << 0) );
		SetData(LockData, i, j, dwData);
	}

	virtual void GetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, D3DXVECTOR3& outVector)
	{
		DWORD dwData;
		GetData(LockData, i, j, dwData);

		outVector.x = ((dwData >> 16) & 0xFF);
		outVector.y = ((dwData >> 8) & 0xFF);
		outVector.z = ((dwData) & 0xFF);
		outVector /= 127.5f;
		outVector -= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

};

class NVDataFormat_X8R8G8B8 : public NVDataFormat_32Bit
{
public:
	virtual void GetColors(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float& fRed, float& fGreen, float& fBlue, float& fAlpha)
	{
		DWORD dwValue;
		GetData(LockData, i, j, dwValue);
		fBlue = ((float)(dwValue & 0xFF))/ 255.0f;
		fGreen = ((float)((dwValue >> 8) & 0xFF)) / 255.0f;
		fRed = ((float)((dwValue >> 16) & 0xFF)) / 255.0f;
		fAlpha = 1.0f;
	}
	
	virtual void GetLuminance(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, float& Luminance)
	{
		float fRed, fGreen, fBlue, fAlpha;
		GetColors(LockData, i, j, fRed, fGreen, fBlue, fAlpha);
		Luminance = ((fRed * 0.3f) + (fGreen * 0.59f) + (fBlue * 0.11f));
	}

	virtual void SetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, const D3DXVECTOR3& inVector)
	{
		D3DXVECTOR3 vecScaled = (inVector + D3DXVECTOR3(1.0f, 1.0f, 1.0f)) * 127.5f;
		BYTE red   = (BYTE)vecScaled.x;
		BYTE green = (BYTE)vecScaled.y;
		BYTE blue  = (BYTE)vecScaled.z;
		BYTE alpha = 0xFF;
		DWORD dwData = (DWORD)( ( (DWORD)alpha << 24 ) | ( (DWORD)red << 16 ) | ( (DWORD)green << 8 ) | ( (DWORD)blue << 0) );
		SetData(LockData, i, j, dwData);
	}

	virtual void GetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, D3DXVECTOR3& outVector)
	{
		DWORD dwData;
		GetData(LockData, i, j, dwData);

		outVector.x = ((dwData >> 16) & 0xFF);
		outVector.y = ((dwData >> 8) & 0xFF);
		outVector.z = ((dwData) & 0xFF);
		outVector /= 127.5f;
		outVector -= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	}

};

class NVDataFormat_Q8W8V8U8 : public NVDataFormat_32Bit
{
	virtual void SetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, const D3DXVECTOR3& inVector)
	{
		D3DXVECTOR3 vecScaled = inVector * 127.5f;
		signed char red   = (signed char)vecScaled.x;
		signed char green = (signed char)vecScaled.y;
		signed char blue  = (signed char)vecScaled.z;
		signed char alpha = 0.0f;
		DWORD dwData = (DWORD)( ( (DWORD)(unsigned char)alpha << 24 ) | ( (DWORD)(unsigned char)blue << 16 ) | ( (DWORD)(unsigned char)green << 8 ) | ( (DWORD)(unsigned char)red << 0) );
		SetData(LockData, i, j, dwData);
	}

	virtual void GetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, D3DXVECTOR3& outVector)
	{
		DWORD dwData;
		GetData(LockData, i, j, dwData);

		outVector.x = (float)(signed char)((dwData) & 0xFF);
		outVector.y = (float)(signed char)((dwData >> 8) & 0xFF);
		outVector.z = (float)(signed char)((dwData >> 16) & 0xFF);
		outVector /= 127.5f;
	}
};

class NVDataFormat_NVHS : public NVDataFormat_32Bit
{
	virtual void SetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, const D3DXVECTOR3& inVector)
	{
		D3DXVECTOR3 vecScaled = inVector * 32767.0f;
	    signed short h = (signed short)vecScaled.x;
		signed short l = (signed short)vecScaled.y;
		DWORD dwData = ( ( (DWORD)(unsigned short)h << 16 ) | ( (DWORD)(unsigned short)l << 0 ) );
		SetData(LockData, i, j, dwData);
	}

	virtual void GetVector(const D3DLOCKED_RECT& LockData, DWORD i, DWORD j, D3DXVECTOR3& outVector)
	{
		DWORD dwData;
		GetData(LockData, i, j, dwData);

		outVector.x = (float)(unsigned short)((dwData >> 16) & 0xFFFF);
		outVector.y = (float)(unsigned short)((dwData) & 0xFFFF);
		outVector.z = 1.0f;
		outVector /= 32767.0f;
	}

};
	
class NV2DTextureLocker
{
public:
	enum
	{
		MAX_LOCK_LEVELS = 12
	};

	NV2DTextureLocker(LPDIRECT3DTEXTURE8 pTexture)
		: m_pTexture(pTexture),
		m_pDataFormat(NULL)
	{
		m_pTexture->AddRef();
		for (DWORD i=0; i < MAX_LOCK_LEVELS; i++)
		{
			m_bLocked[i] = false;
		}
		m_dwLevels = m_pTexture->GetLevelCount();

		D3DSURFACE_DESC LevelDesc;
		CHECK_D3DAPI(m_pTexture->GetLevelDesc(0, &LevelDesc));

		switch(LevelDesc.Format)
		{
			case D3DFMT_UNKNOWN:
			case D3DFMT_VERTEXDATA:
			case D3DFMT_INDEX16:
			case D3DFMT_INDEX32:
			case D3DFMT_DXT1:
			case D3DFMT_DXT2:
			case D3DFMT_DXT3:
			case D3DFMT_DXT4:
			case D3DFMT_DXT5:
			default:
				assert(!"Don't understand surface format");
				break;

			case D3DFMT_R8G8B8:
				assert(!"Don't handle 24 bit surfaces");
				break;

			case D3DFMT_A8R8G8B8:
				m_pDataFormat = new NVDataFormat_A8R8G8B8;
				break;

			case D3DFMT_X8R8G8B8:
				m_pDataFormat = new NVDataFormat_X8R8G8B8;
				break;

			case D3DFMT_Q8W8V8U8:    
				m_pDataFormat = new NVDataFormat_Q8W8V8U8;
				break;

			case MAKEFOURCC('N', 'V', 'H', 'S'):
				m_pDataFormat = new NVDataFormat_NVHS;
				break;
		}

		assert(m_pDataFormat);
	}

	virtual ~NV2DTextureLocker()
	{
		for (DWORD i = 0; i < MAX_LOCK_LEVELS; i++)
		{
			if (m_bLocked[i])
			{
				Unlock(i);
			}
		}
		SAFE_RELEASE(m_pTexture);
		SAFE_DELETE(m_pDataFormat);
	}


	bool Lock(DWORD dwLevel)
	{
		HRESULT hr;
		assert(dwLevel < m_dwLevels);
		assert(!m_bLocked[dwLevel]);

		m_bLocked[dwLevel] = true;
		hr = m_pTexture->LockRect(dwLevel, &m_LockData[dwLevel], NULL, 0);
		CHECK_D3DAPI(m_pTexture->GetLevelDesc(dwLevel, &m_LevelDesc[dwLevel]));

		if (FAILED(hr))
			return false;

		return true;
	}

	bool Unlock(DWORD dwLevel)
	{
		HRESULT hr;

		assert(dwLevel < m_dwLevels);
		assert(m_bLocked[dwLevel]);

		m_bLocked[dwLevel] = false;
		hr = m_pTexture->UnlockRect(dwLevel);
		if (FAILED(hr))
			return false;

		return true;
	}

	void WrapAddress(DWORD dwLevel, DWORD& i, DWORD& j)
	{
		if (i >= 0)
		{
			i = (i % m_LevelDesc[dwLevel].Width);
		}
		else
		{
			i = (m_LevelDesc[dwLevel].Width - 1) + (i % m_LevelDesc[dwLevel].Width);
		}
		
		if (j >= 0)
		{
			j = (j % m_LevelDesc[dwLevel].Height);
		}
		else
		{
			j = (m_LevelDesc[dwLevel].Height - 1) + (j % m_LevelDesc[dwLevel].Height);
		}
		assert(i >= 0);
		assert(j >= 0);
		assert(i < m_LevelDesc[dwLevel].Width);
		assert(j < m_LevelDesc[dwLevel].Height);
	}

	void GetMapData(DWORD dwLevel, DWORD i, DWORD j, DWORD& dwValue)
	{
		assert(m_bLocked[dwLevel]);
		WrapAddress(dwLevel, i, j);
		m_pDataFormat->GetData(m_LockData[dwLevel], i, j, dwValue);
	}

	void SetMapData(DWORD dwLevel, DWORD i, DWORD j, DWORD dwValue)
	{
		assert(m_bLocked[dwLevel]);
		WrapAddress(dwLevel, i, j);
		m_pDataFormat->SetData(m_LockData[dwLevel], i, j, dwValue);
	}


	void GetMapColors(DWORD dwLevel, DWORD i, DWORD j, float& fRed, float& fGreen, float& fBlue, float& fAlpha)
	{
		assert(m_bLocked[dwLevel]);
		WrapAddress(dwLevel, i, j);
		m_pDataFormat->GetColors(m_LockData[dwLevel], i, j, fRed, fGreen, fBlue, fAlpha);
	}

	void GetMapLuminance(DWORD dwLevel, DWORD i, DWORD j, float& Luminance)
	{
		assert(m_bLocked[dwLevel]);
		WrapAddress(dwLevel, i, j);
		m_pDataFormat->GetLuminance(m_LockData[dwLevel], i, j, Luminance);
	}
		
	void SetMapVector(DWORD dwLevel, DWORD i, DWORD j, const D3DXVECTOR3& inVector)
	{
		assert(m_bLocked[dwLevel]);
		WrapAddress(dwLevel, i, j);
		m_pDataFormat->SetVector(m_LockData[dwLevel], i, j, inVector);
	}

	void GetMapVector(DWORD dwLevel, DWORD i, DWORD j, D3DXVECTOR3& inVector)
	{
		assert(m_bLocked[dwLevel]);
		WrapAddress(dwLevel, i, j);
		m_pDataFormat->GetVector(m_LockData[dwLevel], i, j, inVector);
	}

private:
	NVDataFormat* m_pDataFormat;
	DWORD m_dwLevels;
	bool m_bLocked[MAX_LOCK_LEVELS];
	D3DLOCKED_RECT m_LockData[MAX_LOCK_LEVELS];
	D3DSURFACE_DESC m_LevelDesc[MAX_LOCK_LEVELS];

	
	LPDIRECT3DTEXTURE8 m_pTexture;

};

class NV2DSurfaceLocker
{
public:

	NV2DSurfaceLocker(LPDIRECT3DSURFACE8 pSurface)
		: m_pSurface(pSurface),
		m_pDataFormat(NULL)
	{
		m_pSurface->AddRef();
		m_bLocked = false;

		CHECK_D3DAPI(m_pSurface->GetDesc(&m_LevelDesc));

		switch(m_LevelDesc.Format)
		{
			case D3DFMT_UNKNOWN:
			case D3DFMT_VERTEXDATA:
			case D3DFMT_INDEX16:
			case D3DFMT_INDEX32:
			case D3DFMT_DXT1:
			case D3DFMT_DXT2:
			case D3DFMT_DXT3:
			case D3DFMT_DXT4:
			case D3DFMT_DXT5:
			default:
				assert(!"Don't understand surface format");
				break;

			case D3DFMT_R8G8B8:
				assert(!"Don't handle 24 bit surfaces");
				break;

			case D3DFMT_A8R8G8B8:
				m_pDataFormat = new NVDataFormat_A8R8G8B8;
				break;

			case D3DFMT_X8R8G8B8:
				m_pDataFormat = new NVDataFormat_X8R8G8B8;
				break;

			case D3DFMT_Q8W8V8U8:    
				m_pDataFormat = new NVDataFormat_Q8W8V8U8;
				break;

			case MAKEFOURCC('N', 'V', 'H', 'S'):
				m_pDataFormat = new NVDataFormat_NVHS;
				break;
		}

		assert(m_pDataFormat);
	}

	virtual ~NV2DSurfaceLocker()
	{
		if (m_bLocked)
		{
			Unlock();
		}
		SAFE_RELEASE(m_pSurface);
		SAFE_DELETE(m_pDataFormat);
	}


	bool Lock()
	{
		HRESULT hr;
		assert(!m_bLocked);

		m_bLocked = true;
		hr = m_pSurface->LockRect(&m_LockData, NULL, 0);
		if (FAILED(hr))
			return false;

		return true;
	}

	bool Unlock()
	{
		HRESULT hr;
		assert(m_bLocked);

		m_bLocked = false;
		hr = m_pSurface->UnlockRect();
		if (FAILED(hr))
			return false;

		return true;
	}

	void WrapAddress(DWORD& i, DWORD& j)
	{
		if (i >= 0)
		{
			i = (i % m_LevelDesc.Width);
		}
		else
		{
			i = (m_LevelDesc.Width - 1) + (i % m_LevelDesc.Width);
		}
		
		if (j >= 0)
		{
			j = (j % m_LevelDesc.Height);
		}
		else
		{
			j = (m_LevelDesc.Height - 1) + (j % m_LevelDesc.Height);
		}
		assert(i >= 0);
		assert(j >= 0);
		assert(i < m_LevelDesc.Width);
		assert(j < m_LevelDesc.Height);
	}

	void GetMapData(DWORD i, DWORD j, DWORD& dwValue)
	{
		assert(m_bLocked);
		WrapAddress(i, j);
		m_pDataFormat->GetData(m_LockData, i, j, dwValue);
	}

	void SetMapData(DWORD i, DWORD j, DWORD dwValue)
	{
		assert(m_bLocked);
		WrapAddress(i, j);
		m_pDataFormat->SetData(m_LockData, i, j, dwValue);
	}


	void GetMapColors(DWORD i, DWORD j, float& fRed, float& fGreen, float& fBlue, float& fAlpha)
	{
		assert(m_bLocked);
		WrapAddress(i, j);
		m_pDataFormat->GetColors(m_LockData, i, j, fRed, fGreen, fBlue, fAlpha);
	}

	void GetMapLuminance(DWORD i, DWORD j, float& Luminance)
	{
		assert(m_bLocked);
		WrapAddress(i, j);
		m_pDataFormat->GetLuminance(m_LockData, i, j, Luminance);
	}
		
	void SetMapVector(DWORD i, DWORD j, const D3DXVECTOR3& inVector)
	{
		assert(m_bLocked);
		WrapAddress(i, j);
		m_pDataFormat->SetVector(m_LockData, i, j, inVector);
	}

	void GetMapVector(DWORD i, DWORD j, D3DXVECTOR3& inVector)
	{
		assert(m_bLocked);
		WrapAddress(i, j);
		m_pDataFormat->GetVector(m_LockData, i, j, inVector);
	}

private:
	NVDataFormat* m_pDataFormat;
	bool m_bLocked;
	D3DSURFACE_DESC m_LevelDesc;
	D3DLOCKED_RECT m_LockData;
	
	LPDIRECT3DSURFACE8 m_pSurface;

};

class NVTexture2
{
public:

	// Gets height from luminance value
	static LPDIRECT3DTEXTURE8 CreateNormalMap(LPDIRECT3DDEVICE8 pD3DDev, LPDIRECT3DTEXTURE8 pSource, D3DFORMAT Format = D3DFMT_Q8W8V8U8, D3DPOOL Pool = D3DPOOL_MANAGED)
	{
		LPDIRECT3DTEXTURE8 pNormalMap = NULL;
		D3DSURFACE_DESC ddsdDescDest;
		D3DSURFACE_DESC ddsdDescSource;
		D3DXVECTOR3 Normal;
		HRESULT hr;
		DWORD i, j;

		assert(pSource && pSource->GetType() == D3DRTYPE_TEXTURE);
		if (!pSource)
			return NULL;

		(pSource)->GetLevelDesc(0, &ddsdDescSource);

		hr = pD3DDev->CreateTexture(ddsdDescSource.Width, ddsdDescSource.Height, pSource->GetLevelCount(), 0, Format, Pool, &pNormalMap);	
		if (FAILED(hr))
		{
			return NULL;
		}

		NV2DTextureLocker SourceLocker(pSource);
		� �֋5�  h`� h�& ��h � h�& ���5�� h�& ���%����� 8u+�   Vh�� �5\� �<  ;ÉE�����;�rF�����\� ��� ��+Њ�@:�u���� �Њ@:�u�+�O�OG:�u��������ȃ��jY3���x����@�E�f�E��E�P��x���P�5İǅx���D   Sj SSSh�� S�@  �������9�� uS�5� �P! �E�j�P�,� ��  �E�P�u��D  �u���  �����}�� ��u�]��F�����?9��u7S�5� �P! h  Wh   �5\� �h! h0  WW�5� �`! ��� ��xL�   @��u9�5�� ������h�� Q�����Q��P�:�������� uƣ�� ��t
h   �H���9`� th@� ��  �`� �u���  ��
���U��E���;u��t@���
u�8 	@���u�3�]� U��M���tA�<
u�AQ����]� 3���U��S�]V�uW� �V���������t,�>[u�F�
��t�� ~
@���]u���p�+�H��t�9}w3��W�FPS��! S� ��! ����_^[]� U��E�8[u@�u�uP�x���]� U���djd�E�P�u�`�����t?S�U�M��:u��t�Z:YuBBAA��u�3��Ƀ����tjd�M�QP������u�[�� U���djd�E�P�u�u�����t?S�U�M��:u��t�Z:YuBBAA��u�3��Ƀ����tjd�M�QP�7�����u�[�� U��EP��������t�8[u�uR������u�]� U��SW3�3��}s3��   �EV�u�����   ��
��   ��;��   ��"t��=t��,t�� �GF���u��3�CF���t\;}wW3҅�t��"t��
u3����
t$��,t��;t��=t�� ~�B;Us!F���uŅ�uG9}�j���u
��v� �3�^_[]� U��3҃}s3��(�EV�u���
t��t	�B;UsF���u�� ^]� 3���U���u�u������t	�uP����]� U���4V�u�u������Sj2�E�Pj V������t8�E�P��! Y�M�E̊:u��t�P:Qu@@AA��u�3��������t�uV�K�������u���^�� �%x! �%t! �%|! U��3�9Eu�W ��%�UV�u���t	�
BF�Mu�9E^uJ�z �� ]� U��V�u3�;���t�U8tBNu�;�u�W ���U;�t+Ή
^]� U��V�uh  ��Y3�;�uQj�u(�{  3��i�U���   ����   ��P�U�P�U�P�U�P�U�P�U �P�U$�P �U(�HH�HD�HL���  �p�fǀ�    ���   ���   ���   ^]�U���$V�uj$�E�P�u�V����$u�}�MSCFuf�}�t�E�Pj�6��  3��H�E�M�f�M�f�Hf�M�f�Hf�M�f�Hf�M�f�H
�M��������P�у����P�H3�@^��U��SV�u���  ���  �H���  �H���  �H�N8�Hf�NpWf�Hf�NrP3�S���  f�H �V$���YYt79^(t@��F8�G�FL�Gf���   f�Gf�Fpf�G�FrW�G�V(���YuSj�6��  3��3�@_^[]� U��SV�uW���   ���  �vH���   �V��9��   ��   �FH�@�]�;��   ��   �FH�@P�F<�P���   �V�NH�I��;���   �FH3�9t1�@QP�F<�P�m  P���   ��P�FH��P�V  �NH;u|3ɋFHfX;�w�FHf9Ht3��3�B9N(tU�   �F8�Gf���   f- f�Gt�FH���G��O�F<ÉG�FHf�@Wf�G�Wf�_�V(���Yuj j�	3�@�j j�6�  3�_^[]� U��SV�uWjj ���   �V�]�}SW���   �E�V����~=�L����D� �x�@��u�+ǍP;�|��u�Mj �DP���   �V�����uj j�6�*  3��3�@_^[]� U��SV�ujj ���   �V�؃����uj j�6��  3��xf���   Wf���  f�Fp���  f���  �F8Wj����  �V$���YYuj j�6�  3��0���  f��f���   t�?;�tj W���   �V�����t�3�@_^[]� U��V�u���   W��3�+�tBHt-Ht Ht��tBWj�6�@  3��7�v4��  ��v4������v4������YtWj���v<�V�v@�VYY3�@_^]� U���SV�u���   ��W��3�+Ǎ��   � �  ��   HtiHt<Ht���v  j�oWWWW3�W��@W�������   SW�E�W�E�PR�  ��,�=�F WWWWWW���   SW�E�W�E���P��R�M��J�����W���   SWWR�8�������tjXWP��   ���   � �  �3�V;�Y�F<uWj��   ���   �V;�Y�F@u	�v<�VY�����   ��HteHt3H��   �v�F4�v�v�v�vPS�v�E��vP���   P�H  �*�v�F4�v�v�v�vPS�v�E��vP���   P������,��F4PS�v���   �vP�c�����;�t0�v<3ۃ��Í\�V�v@�VYYWS�6�V  fǆ�    3��3�@_^[�� U��V�u���   W��3�+�tBHt-Ht Ht��t4Wj�6�  3��)�v4�  ��v4�������v4�������YtWj��3�@_^]� U��SVW�}���   ��3�+���   HtmHtEHtSj��   �u��E�EP�GH�w@�@P�w<�w4��  ������   f�Ef��   �u��E�EP�GH�w@�@P�w<�w4�7����ċ��   �E�EP�GH�w@�@P�w<�w4��������u@�Ef�Mf��/�GH�@��;��   w"�Mf��w<�@�ȋ����ȃ��3�@�Sj�7��	  3�_^[]� U��V�uW�}�EPVW�������|�u�E+�V�P����_^]� U��V�uj�FtP���   �V����uVh   ���   P�������t3�@�j j�6�]	  3�^]� U��V�uWf�}f;��   u3�@�+V�������uPj�6�(	  3��Vf���   ���������_^]� U���$S�]VW���  P�  V���  W������uVW�
�����  W� �  ���  VP�S��������   �9  W���  VP�S��������   �  j$�E�P���   �S����$tj ��   �}�MSCFu�f�}�t�E�Pj��  f�Ef=��tf�Mf;M�uf;E�t	j j
��  j	�{PY�u��3��Cnf�}�E
 �E tj�EP���   �S����uW�E9��   t)�CL;�tP�SY�{L�E;ǉ��   vP�S;�Y�CLtp���   ;�v P�sL���   �S��9��   tWj�,  �E
��9{DuP���   �S;�Y�CDu
�&;��   u.�E��9{HuP���   �S;�Y�CHuWj��   ;��   tWj	��   �Cn�   t-SV���  P��������   SV���  P�s�����u�   ƃ�   ƃ�   �Cnt&SV���  P�H�����twSV���  P�6�����u�cƃ�   ƃ�   jW���   �S������C,Wt�s`���   �S�����uWj�f�ClSf���   ����������j j�3�p  3�_^[�� U��S�]V�u���   ��F,Wj P���   ���  ���   �V�������   ���   �vD���   �V��9��   ��   �FDj �0���   �V�����th�FDf�