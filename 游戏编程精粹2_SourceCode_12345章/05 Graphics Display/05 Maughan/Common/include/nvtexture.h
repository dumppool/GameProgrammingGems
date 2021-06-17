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
		äª ÿÖ‹5È  h`­ hÄ& ÿÖh ¬ h¬& ÿÖÿ5ˆ¡ h”& ÿÖè%Ûÿÿ¡ìª 8u+¾   VhÀ° ÿ5\­ ÿ<  ;Ã‰Eü„üÿÿ;ÆrFéüÿÿ¡\­ ¿À° ‹×+ĞŠˆ@:Ëuö¡ˆ¡ ‹ĞŠ@:Ëuù+ÂOŠOG:Ëuø‹ÈÁé‹òó¥‹Èƒáó¤jY3À½xÿÿÿó«@‰E¤f‰E¨EàP…xÿÿÿPÿ5Ä°Ç…xÿÿÿD   Sj SSShÀ° Sÿ@  …À„†ûÿÿ9”¡ uSÿ5äª ÿP! ‹EàjÿP£,­ ÿô  EüPÿuàÿD  ÿuäÿ  èËÿÿ}üÑ ¿Êu‰]üëFè®Ïÿÿë?9À°u7Sÿ5äª ÿP! h  Wh   ÿ5\  ÿh! h0  WWÿ5äª ÿ`! ¡¨¥ …ÀxL¾   @…Æu9ÿ5¬¥ ‹ÈÁéƒáh°¥ Q‹ÈÑéƒáQƒàPè:Ûÿÿ…À¡¨¥ uÆ£¨¥ …Æt
h   èHÚÿÿ9`  th@­ ÿè  ‰`  ÿuüÿä  Ìé
úÿÿU‹ì‹Eë€ù;u„Ét@Š€ù
uô€8 	@Š„Éuã3À]Â U‹ì‹Më„ÀtAŠ<
uõAQè»ÿÿÿ]Â 3ÀëøU‹ìS‹]V‹uWÆ ëVèÇÿÿÿ‹ğ…öt,€>[uïFë
„Ét€ù ~
@Š€ù]uïëpÿ+ÆH‹øtÙ9}w3ÀëWFPSÿ„! SÆ ÿ¨! ƒÄ‹Æ_^[]Â U‹ì‹E€8[u@ÿuÿuPèxÿÿÿ]Â U‹ìƒìdjdEœPÿuè`ÿÿÿ…Àt?S‹UMœŠ:u„ÛtŠZ:YuBBAA„Ûuæ3ÉëÉƒÙÿ…ÉtjdMœQPè’ÿÿÿ…ÀuÃ[ÉÂ U‹ìƒìdjdEœPÿuèuÿÿÿ…Àt?S‹UMœŠ:u„ÛtŠZ:YuBBAA„Ûuæ3ÉëÉƒÙÿ…ÉtjdMœQPè7ÿÿÿ…ÀuÃ[ÉÂ U‹ì‹EP‹Ğèşÿÿ…Àt€8[uÿuRè…ÿÿÿ…Àuâ]Â U‹ìSW3Û3ÿƒ}s3Àé¬   ‹EV‹uŠ„É„˜   €ù
„   €ù;„†   €ù"t€ù=t€ù,t€ù ëGFŠ„ÉuĞë3ÛCFŠ„Ét\;}wW3Ò…Ût€ù"t€ù
u3Ûë€ù
t$€ù,t€ù;t€ù=t€ù ~ˆB;Us!FŠ„ÉuÅ…ÿuG9}‡jÿÿÿu
…ÒvÆ ë3À^_[]Â U‹ì3Òƒ}s3Àë(‹EV‹uë€ù
t€ùt	ˆB;UsFŠ„ÉuæÆ ^]Â 3Àë÷U‹ìÿuÿuèşÿÿ…Àt	ÿuPè¶şÿÿ]Â U‹ìƒì4VÿuÿuèÏÿÿÿëSj2EÌPj Vè¸şÿÿ…Àt8EÌPÿ¨! Y‹MEÌŠ:u„ÒtŠP:Qu@@AA„Òuæ3ÀëÀƒØÿ…ÀtÿuVèKşÿÿ‹ğ…öu§‹Æ^ÉÂ ÿ%x! ÿ%t! ÿ%|! U‹ì3À9Eu¸W €ë%‹UV‹uŠ„Ét	ˆ
BFÿMuñ9E^uJ¸z €Æ ]Â U‹ìV‹u3À;ğ‹Ît‹U8tBNuø;ğu¸W €ë‹U;Ğt+Î‰
^]Â U‹ìV‹uh  ÿÖY3É;ÁuQjÿu(è{  3Àëi‹Uƒˆˆ   ÿƒˆ„   ÿ‰P‹U‰P‹U‰P‹U‰P‹U‰P‹U ‰P‹U$‰P ‹U(‰HH‰HD‰HL¹ÿÿ  ‰p‰fÇ€²    ‰ˆ    ‰ˆ¨   ‰ˆ¤   ^]ÃU‹ìƒì$V‹uj$EÜPÿuÿVƒÄƒø$u}ÜMSCFuf}ôt·EôPjÿ6èÎ  3ÀëH‹E‹Mä‰f‹Möf‰Hf‹Møf‰Hf‹Müf‰Hf‹Mşf‰H
·Mú‹ÑÁêƒâ‰P‹Ñƒâƒá‰P‰H3À@^ÉÃU‹ìSV‹u†¼  ·  ‰H¸  ‰H¹  ‰H‹N8‰Hf‹NpWf‰Hf‹NrP3ÛS¾ä  f‰H ÿV$ƒøÿYYt79^(t@‰‹F8‰G‹FL‰Gf‹†    f‰Gf‹Fpf‰G·FrW‰GÿV(ƒøÿYuSjÿ6èê  3Àë3À@_^[]Â U‹ìSV‹uWÿ¶¨   ¾ä  ÿvHÿ¶„   ÿVƒÄ9†¨   …õ   ‹FH·@‹]Ã;†˜   ‡İ   ‹FH·@P‹F<ÃPÿ¶„   ÿV‹NH·IƒÄ;È…´   ‹FH3É9t1·@QP‹F<ÃPèm  P‹†¨   ƒèP‹FHƒÀPèV  ‹NH;u|3É‹FHfX;Ùw‹FHf9Ht3Òë3ÒB9N(tUÇ   ‹F8‰Gf‹†¨   f- f‰Gt‹FHƒÀ‰Gë‰O‹F<Ã‰G‹FHf‹@Wf‰G‰Wf‰_ÿV(ƒøÿYuj jë	3À@ëj jÿ6è¬  3À_^[]Â U‹ìSV‹uWjj ÿ¶ˆ   ÿV‹]‹}SWÿ¶ˆ   ‰EÿVƒÄ…À~=ŠLÿ‹ÇÆDÿ xŠ@„Òuù+ÇP;Ó|„Éu‹Mj DPÿ¶ˆ   ÿVƒÄƒøÿuj jÿ6è*  3Àë3À@_^[]Â U‹ìSV‹ujj ÿ¶ˆ   ÿV‹ØƒÄƒûÿuj jÿ6èò  3Àëxf‹†¬   Wf‰†Ş  f‹Fp¾¼  f‰†Ú  ‹F8Wj‰‰†Ì  ÿV$ƒøÿYYuj jÿ6è«  3Àë0·†Ş  f…Àf‰†¬   t‹?;ûtj Wÿ¶ˆ   ÿVƒÄƒøÿtÄ3À@_^[]Â U‹ìV‹u¶†²   Wƒà3ÿ+ÇtBHt-Ht HtƒètBWjÿ6è@  3Àë7ÿv4èÜ  ëÿv4èÑÿÿëÿv4èÑÿÿ…ÀYtWjëÏÿv<ÿVÿv@ÿVYY3À@_^]Â U‹ìƒìSV‹u·²   ‹ÁWƒà3ÿ+Ç–”   Ç €  „   HtiHt<Htƒè„v  jëoWWWW3ÀWÁé@WƒáÓà˜   SW‰EğWEğPRè  ƒÄ,ë=‹F WWWWWW˜   SW‰EüWEøÁéPƒáR‰MøèJĞÿÿëÑW˜   SWWRè8ĞÿÿƒÄ…ÀtjXWPéç   ˜   Ç €  ÿ3ÿV;ÇY‰F<uWjéÆ   ÿ¶”   ÿV;ÇY‰F@u	ÿv<ÿVYëŞ¶†²   ƒàHteHt3H…©   ÿvF4ÿvÿvÿvÿvPSÿvEğÿvP†”   PèH  ë*ÿvF4ÿvÿvÿvÿvPSÿvEøÿvP†”   PèÏÿÿƒÄ,ëF4PSÿv†”   ÿvPècÏÿÿƒÄ;Çt0ÿv<3Ûƒø•Ã\ÿVÿv@ÿVYYWSÿ6èV  fÇ†²    3Àë3À@_^[ÉÂ U‹ìV‹u¶†²   Wƒà3ÿ+ÇtBHt-Ht Htƒèt4Wjÿ6è  3Àë)ÿv4èŒ  ëÿv4èßÎÿÿëÿv4èÕÎÿÿ…ÀYtWjëÏ3À@_^]Â U‹ìSVW‹}¶‡²   ƒà3Û+Ã„¦   HtmHtEHtSjéÌ   ‹u·‰EEP‹GHÿw@·@Pÿw<ÿw4èÆ  ƒÄ…À…›   f‹Ef‰éŠ   ‹u·‰EEP‹GHÿw@·@Pÿw<ÿw4è7ÎÿÿëÄ‹‡”   ‰EEP‹GHÿw@·@Pÿw<ÿw4èÎÿÿƒÄ…Àu@‹Ef‹Mf‰ë/‹GH·@·È;”   w"‹Mf‰‹w<‹@·È‹ÁÁéó¥‹Èƒáó¤3À@ëSjÿ7èÛ	  3À_^[]Â U‹ìV‹uW‹}EPVWèì÷ÿÿ…À|ÿu‹E+ğVÇPè÷ÿÿ_^]Â U‹ìV‹ujFtPÿ¶ˆ   ÿVƒÄƒøuVh   †´   PèÎúÿÿ…Àt3À@ëj jÿ6è]	  3À^]Â U‹ìV‹uWf‹}f;¾²   u3À@ë+VèÆûÿÿ…ÀuPjÿ6è(	  3ÀëVf‰¾²   èüÿÿ÷ØÀ÷Ø_^]Â U‹ìƒì$S‹]VWƒ¹  P¾  V»º  WèÜöÿÿÿuVWè
ÿÿÿ¿€  W¾ €  ƒº  VPÿSƒÄƒøÿ‰ƒˆ   „9  Wƒº  VPÿSƒÄƒøÿ‰ƒ„   „  j$EÜPÿ³ˆ   ÿSƒÄƒø$tj éÎ   }ÜMSCFuğf}ôt·EôPjéä  f‹Ef=ÿÿtf‹Mf;Müuf;Eşt	j j
éÁ  j	{PYuÜó¥3ÿöCnf‰}ÆE
 ÆE tjEPÿ³ˆ   ÿSƒÄƒøuW·E9ƒ    t)‹CL;ÇtPÿSY‰{L·E;Ç‰ƒ    vPÿS;ÇY‰CLtp‹ƒ    ;Çv PÿsLÿ³ˆ   ÿSƒÄ9ƒ    tWjé,  ¶E
ƒÀ9{DuP‰ƒ¤   ÿS;ÇY‰CDu
ë&;ƒ¤   u.¶EƒÀ9{HuP‰ƒ¨   ÿS;ÇY‰CHuWjéŞ   ;ƒ¨   tWj	éÎ   öCn¾   t-SVƒµ  Pè‰øÿÿ…À„´   SVƒ¶  Pèsøÿÿ…Àué   Æƒµ   Æƒ¶   öCnt&SVƒ·  PèHøÿÿ…ÀtwSVƒ¸  Pè6øÿÿ…ÀuëcÆƒ·   Æƒ¸   jWÿ³ˆ   ÿSƒÄƒøÿ‰C,Wtÿs`ÿ³ˆ   ÿSƒÄƒøÿuWjëf‹ClSf‰ƒ¬   èöÿÿ÷ØÀ÷Øëj jÿ3èp  3À_^[ÉÂ U‹ìS‹]V‹u‹†¤   ¯ÃF,Wj Pÿ¶„   ¾ä  ‰   ÿVƒÄƒøÿ„¡   ÿ¶¤   ÿvDÿ¶„   ÿVƒÄ9†¤   …€   ‹FDj ÿ0ÿ¶„   ÿVƒÄƒøÿth‹FDf‹