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
	D3DSURFACE_DESC   P    v    [ÿÿ€         P    Š    Wÿÿ€         P        Zÿÿ€         P    ²    Tÿÿ€         P    v     Vÿÿ€         P    Š     Oÿÿ€         P         Yÿÿ€         P    ²     Uÿÿ€         P    v .   Rÿÿ€         P    Š .   Nÿÿ€         P     .   Mÿÿ€         P    ² .   Qÿÿ€       D  @         ú >        M S   S h e l l   D l g     P      d  ÿÿÿÿ‚ yr'`T( & N )     !P      d  ±ÿÿ…         P     # 6  ³ÿÿ€ OmÈ‰( & B ) . . .       P    = # 6  ´ÿÿ€ nd–( & C )        P    k  *  ÿÿÿÿ‚ „˜–:       P    —  ] 6 ²ÿÿ‚       ê   Bold Italic     d4   V S _ V E R S I O N _ I N F O     ½ïş   
    
    ?                         Ä   S t r i n g F i l e I n f o       0 8 0 4 0 4 b 0   L   C o m p a n y N a m e     M i c r o s o f t   C o r p o r a t i o n   h    F i l e D e s c r i p t i o n     M F C   L a n g u a g e   S p e c i f i c   R e s o u r c e s   8   F i l e V e r s i o n     7 . 1 0 . 3 0 7 7 . 0   :   I n t e r n a l N a m e   M F C 7 1 C H S . D L L     ‚ /  L e g a l C o p y r i g h t   ©   M i c r o s o f t   C o r p o r a t i o n .     A l l   r i g h t s   r e s e r v e d .     \   P r o d u c t N a m e     M i c r o s o f t ®   V i s u a l   S t u d i o   . N E T   B   O r i g i n a l F i l e n a m e   M F C 7 1 C H S . D L L     <   P r o d u c t V e r s i o n   7 . 1 0 . 3 0 7 7 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     °     Sb _ æSX[:N
 @b	g‡eöN  ( * . * )  àeh˜˜ İOX[oR,g:N sQí•SbpS„˜È‰!j_
 ÖSˆm„˜È‰  N*N*g}TT„v‡eöN                     –Ï…( & H )                                 	 àeÕl·ƒÖS•ï‹áOo`0 Õ‹şVgbLˆû|ß~N/ec„vÍd\O0	 àeÕl·ƒÖS@b —Dn0 …QX[N³0 ÑSu*gåw•ï‹ G0RàeHeÂSpe0                           àeHe„v‡eöNT0 Sb _‡ech1Y%0 İOX[‡ech1Y%0 /f&T\9e¨RİOX[0R  % 1 ÿ ú^Ëzzz‡ech1Y%0 å‹‡eöN*Y'YÿàeÕlSb _0	 àeÕl/T¨RSbpS\ON0 /T¨R.^©R1Y%0	 …Qè”^(uz^úQ•0 }TäN1Y%0 ¡l	g³Y„v…QX[gbLˆÍd\O0 û|ß~èlŒQy˜ò]«ˆûyd–v^Nøv”^„v  I N I   ‡eöNÿ‚YX[(W	ÿ_N«ˆ Rd–0 N/f@b	g„vû|ß~èlŒQy˜ÿb  I N I   ‡eöN	ÿı«ˆûyd–0 (Wû|ß~-N¡l	g~b0Rå‹z^@b —„v‡eöN  % s 0. å‹z^Ş¥c0R‡eöN  % s   -N"N1Y„v“úQ  % s   0å‹¡‹—{:g
NïSı€Åˆ	g N*NN  % s   N|Q¹[„vHr,g0   àeÕlÀo;mY—bk„v  A c i v e X   ù[aŒ0 Ş¥c1Y%0
 ş”¥cïSı€ò]Ï~­e†N0 àeÕlYt}TäNÿg¡RhVz^ckÙ_0 gbLˆg¡RhVz^Íd\O1Y%0 ‡echù[aŒg¡RhVN/ecÙ*N‡eöN0 % 1 
 àeÕlèlŒQ‡ech0
 dk‡echïSı€ò]Ï~«ˆSb _0 /T¨Rg¡RhV”^(uz^1Y%0 (Wç~í~ĞLˆMRù[  % 1   ÛLˆôf°eÿ Nı€ôf°e¢[7b0 èlŒQ1Y%0A c t i v e X   ŸRı€àeÕlcknxO(u0 ôf°eû|ß~èlŒQ°‹U_1Y%0
 ÷‹Õ‹(u  R E G E D I T 0 lbc  A c t i v e X   ù[aŒ1Y%0 àeÕlû‹êS™Q^\'`0 àeÕl™QêSû‹^\'`02 S_ck(W úQ  W i n d o w s   öeÿàeÕlİOX[  A c t i v e X   ù[aŒÿ
 >e_ù[  % 1   @bZP„v@b	g„vîO9eTÿ Rú^ù[aŒ1Y%0÷‹nx¤‹”^(uz^ò](Wû|ß~èlŒQhˆ-NèlŒQ0       aY„v‡eöN<h_0 àeÕl~b0Rå‹‡eöN0
 ÷‹ŒšÁ‹@bÙ~ï„_ŒT‡eöNT/f&Tcknx0
 îvhÁxØvqš¨RhVò]án0 àeÕlù[  % 1   ÛLˆû‹Íd\Oÿƒ[ò]Ï~«ˆvQÖNºNSb _0 àeÕlù[  % 1   ÛLˆ™QÍd\OÿàV:Nƒ[/fêSû‹‡eöNbò]Ï~«ˆvQÖNºNSb _0 (Wù[  % 1   ÛLˆû‹Íd\OöeÑSu†NaY•ï‹0 (Wù[  % 1   ÛLˆ™QÍd\OöeÑSu†NaY•ï‹0                         ÷‹“eQ N*Ntepe0 ÷‹“eQ N*Npe0 ÷‹“eQ N*N(W  % 1   ŒT  % 2   KNô•„vtepe0 ÷‹“eQ N*N(W  % 1   ŒT  % 2   KNô•„vpeW[0 ÷‹“eQNYN  % 1   *N„vW[&{0 ÷‹	éb N*N	c®”0 ÷‹“eQ N*N(W  0   ŒT  2 5 5   KNô•„vtepe0	 ÷‹“eQ N*Ncktepe0 ÷‹“eQ N*NåegŒT/ böeô•<P0	 ÷‹“eQ N*N'^<P0 ÷‹“eQ N*N  G U I D 0	 ÷‹“eQ N*Nöeô•<P0	 ÷‹“eQ N*Nåeg<P0           MOşV
  NE^MOşV % s 
 % s  % s 
 % s    ş”¥c„v  % s  *gåw{|‹W YáOo`‡e,g( R T F ) 
 W[SOŒTµk=„<h_S„v‡e,g *g<h_S‡e,g
 *gÏ~ÇûNUO<h_S„v‡e,g àeHe„v'^0                     *gÑSu•ï‹0 (Wù[  % 1   ÛLˆ¿‹î•öeÑSu†N*gåw•ï‹0 ¡l	g~b0R  % 1 0 % 1   -NS+TàeHe„vï„_0 àeÕlSb _  % 1   àV:N*YY‡eöNò]«ˆSb _0 ù[  % 1   „v¿‹î•«ˆÒbİ~0  N*NàeHe„v‡eöNåSÄgN  % 1   øvsQT€0 àeÕl Rd–  % 1   àV:Nƒ[/fS_MRîvU_0 å‹îvU_ò]ánÿàeÕlRú^  % 1 0 ù[  % 1   ÛLˆåg~b1Y%0 (W¿‹î•  % 1   öe N*NlxöN“eQ/ “úQ•ï‹«ˆ¥bJT0 (W¿‹î•  % 1   öeÑSuqQ«Nİ‹O0 (W¿‹î•  % 1   öeÑSu•İ‹O0 (W¿‹î•  % 1   öeÁxØvò]án0 Õ‹şVŠÇvQ>\ïzù[  % 1   ÛLˆû‹™Q0     *gÑSu•ï‹0 (Wù[  % 1   ÛLˆ¿‹î•öeÑSu†N*gåw•ï‹0 Õ‹şV(Wù[  % 1   ÛLˆû‹Íd\O„vTöeù[vQÛLˆ™QÍd\O0 Õ‹şVŠÇvQ>\ïzù[  % 1   ÛLˆû‹™Q0 Õ‹şV(Wù[  % 1   ÛLˆ™QÍd\O„vTöeù[vQÛLˆû‹Íd\O0 % 1   <h_•0 % 1   +T	g^—„˜g„vù[aŒ0 % 1   S+T•ï‹„v!j_0                 àeÕl R}®öNû|ß~/eôc0 ®öNû|ß~  D L L   àeHe0  ÑS®öN *gı€\®öNúQ0                                 aŒ }                                   % 1 :   % 2 
 /f&Tç~í~ĞLˆ,gÿ R>m_8^ÿ  % 1                                ÖSˆm	-N 	-N ÷mT                         N  % 1   
N  Nu˜( & O )  $Nu˜( & T )  ,{  % u   u˜ ,{  % u   u˜  
 ,{  % u   u˜- ,{  % u   u˜  
  p r n 
 O u t p u t . p r n $ SbpS:g‡eöN  ( * . p r n ) | * . p r n | @b	g‡eöN  ( * . * ) | * . * | |  pSúQ0R‡eöN 0R  % 1                                     	 ôf°e  % 1 ( & U )       úQv^ŞV0R  % 1 ( & X )  ôf°e  A c t i v e X   ù[aŒ şVGr( CQ‡eöN) 
  NE^şVGr ¾‹YìrËzMOşV
  NE^¾‹YìrËzMOşV            	 % 1   (W  % 2   -N   % 1   -   % 2                       	 Õ‹şVŞ¥cpencn1Y% °‹U_Æ–ÅN/ecTMRûy¨R0 Õ‹şVSb _hˆ<h1Y% *gcf‰Àh"}„vR0 ågâ‹ÔŞV„vRpenc{|‹WNggN&{0 ôf°eb Rd–„vOşV1Y%†N0 YLˆpenc«ˆôf°e0 Íd\O1Y%ÿàeS_MR°‹U_0 ôf°eb Rd–Íd\O*g‰mÊSûNUOpencLˆ0 °‹U_Æ–/fêSû‹# O D B C   qš¨Rz^N/ec  M i c r o s o f t   úW@x{|•ŒNÛ6Rpenc!j‹W0 ÅˆeQ@b —„v  O D B C 3 2 . D L L   èöN„vOşV1Y%0 O D B C   qš¨Rz^N/ec¨R`°‹U_Æ–0 ‰/ecë_gqŸRı€ÿO D B C   Y—`8nh/fÅ_‰„v0I O D B C   qš¨Rz^N  M i c r o s o f t   úW@x{|penc“^{|N9SM‘  ( A P I _ C O N F O R M A N C E   > =   S Q L _ O A C _ L E V E L 1   Å_{˜bËz) 0J O D B C   qš¨Rz^N  M i c r o s o f t   úW@x{|penc“^{|N9SM‘  ( A P I _ C O N F O R M A N C E   > =   S Q L _ O A C _ M I N I M U M   Å_{˜bËz) 0     Õ‹şVŠÇpenc„v	gHeX[.:Sô•0 O D B C   qš¨Rz^N/ec¨R`°‹U_Æ–0  —‰&{T  O D B C   „vŒN§~hÆQ„vqš¨Rz^0 O D B C   qš¨Rz^N/ecš[MOôf°e0 N/ec@b‰Bl„v•!j_0 penc«ˆ*b­e0 Àh"}°‹U_úQ•0 ¡l	g~b0R@b —„v  O D B C   ÛeQ¹p0nxİO  O D B C   ‰[Åˆcknx0
 ôf°eb Rd–Íd\O1Y%0  O D B C qš¨Rz^N/ec¨R`cˆ”0  àeHe„vW[µkTbW[µk"}_0 fN~{N«ˆ  O D B C   qš¨Rz^/ec0 fN~{(W°‹U_Æ–-N*gÀo;m0 < ò] Rd–>             àeÕlù[D A O / J e t   d b _ÎdÛLˆRËYS0   •„vD F X <PÂSpe0   D A O   T a b l e D e f bQ u e r y D e f ¡l	gSb _0   G e t R o w s   1Y%0RM‘Ù~Lˆ„v…QX[NY0   G e t R o w s   K<P•ï‹0ïSı€/fpenc{|‹WN9SM‘ b„v0   G e t R o w s   1Y%0@b÷‹Bl„vRN/få‹°‹U_<P„v N*NbR0                        ( *gåw)      ğS_ ”^(uz^å]\O:S —zãSÌ€of —zãS‡e,g ÜƒUSh ÜƒUS‡eW[ ;m¨R„vh˜˜h N;m¨R„vh˜˜h ;m¨R„vh˜˜h‡eW[	 N;m¨R„vh˜˜h‡eW[ ;m¨R¹Fh N;m¨R¹Fh   —zãSFh¶g wS¨Rag 	c®”hˆb— 	c®”4–q_ 	c®”‡eW[ 	c®”zúQ 1YHe‡eW[ zúQ zúQ„v‡eW[ ck8^SO ÑSO œeSO ÑœeSO A a B b Y y Z z  ( W[SO)  ( œ˜r‚) 	 ( şVGr  -   % s ) ¨ @b	gşVGr{|‹W| * . b m p ; * . c u r ; * . d i b ; * . e m f ; * . i c o ; * . w m f | B i t m a p s   ( * . b m p ; * . d i b ) | * . b m p ; * . d i b | I c o n s / C u r s o r s   ( * . i c o ; * . c u r ) | * . i c o ; * . c u r | M e t a f i l e s   ( * . w m f ; * . e m f ) | * . w m f ; * . e m f | @b	g‡eöN( * . * ) | * . * | |  *gåw àe MOşV CQ‡eöN şVh   œ˜r‚yr'`u˜ œ˜r‚ W[SOyr'`u˜ W[SO şVGryr'`u˜ şVGr     OmÈ‰şVGr 0   -   àe 1   -   úVš[US*N                                 ‘( & E ) 	 yr'`( & P ) . . .                                      àeÕlSb _şVGr‡eöN  % 1 0dk‡eöNïSı€NX[(WbNïSû‹0 àeÕlÅˆeQşVGr‡eöN  % 1 0å‹‡eöNïSı€<h_Ncknx0 àeÕlÅˆeQşVGr‡eöN  % 1 0å‹‡eöN*Y'Y…QX[àeÕl¹[³~0 àeÕlÅˆeQşVGr‡eöN  % 1 0*gˆNwQSOô‹f„vû‹Íd\O1Y%0                         ^—ÕlıQpeŒ(u ¢núQ …QX[N³ «ˆö–d– W[&{2Nzzô•N³ Xhzzô•N³ ‡eöNTb‡eöN÷S• ‡eöN*g~b0R ‡eöN!j_• ‡eöNò]Ï~Sb _	 ¾‹Y“eQ/ “úQúQ• ‡eöNò]Ï~X[(W °‹U_•¦^• ÁxØvò]án °‹U_÷Sx• ‡eöNT•       ‡eöN*YY ¾‹YNT *g·ƒ—_¸‹ïS ÁxØv*gÆQY}Y	 ï„_/ ‡eöN¿‹î••ï‹ ¡l	g~b0Rï„_ àeHe„vW[&{2N‹W7h zz<PO(uNS_ àeHe„v‡eöN<h_ àeHe„vyr'`<P àeHe„vpeÄ~Nh å‹yr'`(WĞLˆöe;RNı€¾‹n å‹yr'`:NêSû‹yr'` O(uyr'`peÄ~öeÅ_{˜cfpeÄ~Nh NïS(WÙ*N§c6R
N¾‹nyr'` å‹yr'`NïS(WĞLˆöe;Rû‹ÖS å‹yr'`:NêS™Qyr'` ¡l	g~b0Ryr'` àeHe„vjR4g<h_ àeHeşVGr SbpS:gúQ• àeÕlRú^X[.Çz@b —‰„v4Nöe‡eöN
 ¡l	g~b0R‰åg~b„v‡e,g ‰ÿfbc„v‡e,g*Y•                        & < H E A D > < T I T L E > H T T P   •ï‹  % d < / T I T L E > < / H E A D > ) < B O D Y > < H 1 > H T T P   •ï‹  % d :   àeïS(uˆmo`< / H 1 > < / B O D Y >  
 4 < B O D Y > < H 1 > àeHe÷‹Bl< / H 1 > < P > ¢[7bïzÑSúQ N*Ng¡RhVàeÕlYt„v÷‹Bl. < b r > ÷‹Bl:   % s  
 - < B O D Y > < H 1 >  —‰t’š[< / H 1 > < P > OmÈ‰hVàet’š[ŸRı€bt’š[1Y%. < / B O D Y >  
 . < B O D Y > < H 1 > ybk< / H 1 > < P > ¢[7b¡l	gCgP–ÎNg¡RhV·ƒÖSå‹U R L . < / B O D Y >  
 1 < B O D Y > < H 1 > ¡l	gÑS°s< / H 1 > < P > (Wå‹g¡RhV
N¡l	gÑS°s@b÷‹Bl„vU R L . < / B O D Y >  
 3 < B O D Y > < H 1 > g¡RhV•ï‹< / H 1 > g¡RhVG0R…Qè•ï‹b•ï‹M‘nàeÕlŒ[bå‹÷‹Bl. < / B O D Y >  
 4 < B O D Y > < H 1 > ¡l	g[°s< / H 1 > g¡RhVàeÕlgbLˆå‹¹eÕl  < b > % s < / b >   dköe. < / B O D Y >  
                    ibU\T 'Y™Q peW[ Ún¨R 9e™Q °‹U_ GPT                    
 ú^Ëz N*N°e‡ech
 °eú^ Sb _ N*N°sX[‡ech
 Sb _	 sQí•;m¨R‡ech
 sQí•	 İOX[;m¨R‡ech
 İOX[ (u N*N°e‡eöNTİOX[;m¨R‡ech
 æSX[:N 9eØSSbpS	y˜
 u˜b—¾‹n 9eØSSbpS:gÊSSbpS	y˜
 SbpS¾‹n	 SbpS;m¨R‡ech
 SbpS  	 >f:yteu˜
 SbpS„˜È‰     Ç5u