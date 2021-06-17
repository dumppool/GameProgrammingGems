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
	D3DSURFACE_DESC   P    v    [���         P    �    W���         P    �    Z���         P    �    T���         P    v     V���         P    �     O���         P    �     Y���         P    �     U���         P    v .   R���         P    � .   N���         P    � .   M���         P    � .   Q���       D  @         � >        M S   S h e l l   D l g     P      d  ����� yr'`T( & N )     !P      d  ����         P     # 6  ���� Omȉ( & B ) . . .       P    = # 6  ���� nd�( & C )        P    k  *  ����� ���:       P    �  ] 6 ����       �   Bold Italic     d4   V S _ V E R S I O N _ I N F O     ���   
    
    ?                         �   S t r i n g F i l e I n f o   �   0 8 0 4 0 4 b 0   L   C o m p a n y N a m e     M i c r o s o f t   C o r p o r a t i o n   h    F i l e D e s c r i p t i o n     M F C   L a n g u a g e   S p e c i f i c   R e s o u r c e s   8   F i l e V e r s i o n     7 . 1 0 . 3 0 7 7 . 0   :   I n t e r n a l N a m e   M F C 7 1 C H S . D L L     � /  L e g a l C o p y r i g h t   �   M i c r o s o f t   C o r p o r a t i o n .     A l l   r i g h t s   r e s e r v e d .     \   P r o d u c t N a m e     M i c r o s o f t �   V i s u a l   S t u d i o   . N E T   B   O r i g i n a l F i l e n a m e   M F C 7 1 C H S . D L L     <   P r o d u c t V e r s i o n   7 . 1 0 . 3 0 7 7 . 0   D    V a r F i l e I n f o     $    T r a n s l a t i o n     �     Sb _ �SX[:N
 @b	g�e�N  ( * . * )  �eh�� �OX[oR,g:N sQ�SbpS��ȉ!j_
 �S�m��ȉ  N*N*g}TT�v�e�N                     ��υ( & H )                                 	 �e�l���S���Oo`0 Ջ�VgbL��|�~N/ec�v�d\O0	 �e�l���S@b �D��n0 �QX[N��0 �Su*g�w�� G�0R�eHe�Spe0                           �eHe�v�e�NT0 Sb _�ech1Y%�0 �OX[�ech1Y%�0 /f&T\9e�R�OX[0R  % 1 � �^�zzz�ech1Y%�0 勇e�N*Y'Y��e�lSb _0	 �e�l/T�RSbpS\ON0 /T�R.^�R1Y%�0	 �Q萔^(uz�^�Q�0 }T�N1Y%�0 �l	g��Y�v�QX[gbL��d\O0 �|�~�l�Qy��]���yd�v^N�v�^�v  I N I   �e�N��YX[(W	�_N�� Rd�0 N/f@b	g�v�|�~�l�Qy��b  I N I   �e�N	������yd�0 (W�|�~-N�l	g~b0R�z�^@b ��v�e�N  % s 0. �z�^ޏ�c0R�e�N  % s   -N"N1Y�v���Q  % s   0勡��{:g
N�S��ň	g N*NN  % s   N|Q�[�vHr,g0   �e�l�o;mY�bk�v  A c i v e X   �[a�0 ޏ�c1Y%�0
 ���c�S���]�~�e�N0 �e�lYt}T�N�g�RhVz�^ck�_0 gbL�g�RhVz�^�d\O1Y%�0 �ech�[a�g�RhVN/ecُ*N�e�N0 % 1 
 �e�l�l�Q�ech0
 dk�ech�S���]�~��Sb _0 /T�Rg�RhV�^(uz�^1Y%�0 (W�~�~ЏL�MR�[  % 1   ۏL��f�e� N���f�e�[7b0 �l�Q1Y%�0A c t i v e X   �R���e�lcknxO(u0 �f�e�|�~�l�Q��U_1Y%�0
 ��Ջ(u  R E G E D I T 0 l�bc  A c t i v e X   �[a�1Y%�0 �e�l���S�Q^\'`0 �e�l�Q�S��^\'`02 S_ck(W ��Q  W i n d o w s   �e��e�l�OX[  A c t i v e X   �[a��
 >e_�[  % 1   @bZP�v@b	g�v�O9eT� R�^�[a�1Y%�0��nx���^(uz�^�](W�|�~�l�Qh�-N�l�Q0       aY�v�e�N<h_0 �e�l~b0R勇e�N0
 ������@b�~_�T�e�NT/f&Tcknx0
 �vh�x�vq��RhV�]�n0 �e�l�[  % 1   ۏL����d\O��[�]�~��vQ�N�NSb _0 �e�l�[  % 1   ۏL��Q�d\O��V:N�[/f�S���e�Nb�]�~��vQ�N�NSb _0 (W�[  % 1   ۏL����d\O�e�Su�NaY��0 (W�[  % 1   ۏL��Q�d\O�e�Su�NaY��0                         ����eQ N*Ntepe0 ����eQ N*Npe0 ����eQ N*N(W  % 1   �T  % 2   KN���vtepe0 ����eQ N*N(W  % 1   �T  % 2   KN���vpeW[0 ����eQNY�N  % 1   *N�vW[&{0 ��	��b N*N	c��0 ����eQ N*N(W  0   �T  2 5 5   KN���vtepe0	 ����eQ N*Ncktepe0 ����eQ N*N�eg�T/ b�e��<P0	 ����eQ N*N'�^<P0 ����eQ N*N  G U I D 0	 ����eQ N*N�e��<P0	 ����eQ N*N�eg<P0           MO�V
  NE^MO�V % s 
 % s  % s 
 % s    ���c�v  % s  *g�w{|�W Y�Oo`�e,g( R T F ) 
 W[SO�T�k=�<h_S�v�e,g *g<h_S�e,g
 *g�~Ǐ�NUO<h_S�v�e,g �eHe�v'�^0                     *g�Su��0 (W�[  % 1   ۏL�����e�Su�N*g�w��0 �l	g~b0R  % 1 0 % 1   -NS+T�eHe�v_0 �e�lSb _  % 1   �V:N*YY�e�N�]��Sb _0 �[  % 1   �v����b�~0  N*N�eHe�v�e�N�S�gN  % 1   �vsQT�0 �e�l Rd�  % 1   �V:N�[/fS_MR�vU_0 ��vU_�]�n��e�lR�^  % 1 0 �[  % 1   ۏL��g~b1Y%�0 (W���  % 1   �e N*Nlx�N��eQ/ ���Q���bJT0 (W���  % 1   �e�SuqQ�Nݏ�O0 (W���  % 1   �e�Su�ݏ�O0 (W���  % 1   �e�x�v�]�n0 Ջ�V��ǏvQ>\�z�[  % 1   ۏL����Q0     *g�Su��0 (W�[  % 1   ۏL�����e�Su�N*g�w��0 Ջ�V(W�[  % 1   ۏL����d\O�vT�e�[vQۏL��Q�d\O0 Ջ�V��ǏvQ>\�z�[  % 1   ۏL����Q0 Ջ�V(W�[  % 1   ۏL��Q�d\O�vT�e�[vQۏL����d\O0 % 1   <h_�0 % 1   +T	g^���g�v�[a�0 % 1   S+T�v!j_0                 �e�l�R}����N�|�~/e�c0 ���N�|�~  D L L   �eHe0  �S����N *g��\���N��Q0                                 a� }                                   % 1 :   % 2 
 /f&T�~�~ЏL��,g� R>m_8^�  % 1                                �S�m	�-N 	�-N �mT                         �N  % 1   
N  Nu�( & O )  $Nu�( & T )  ,{  % u   u� ,{  % u   u�  
 ,{  % u   u�- ,{  % u   u�  
  p r n 
 O u t p u t . p r n $ SbpS:g�e�N  ( * . p r n ) | * . p r n | @b	g�e�N  ( * . * ) | * . * | |  pS�Q0R�e�N 0R  % 1                                     	 �f�e  % 1 ( & U )       ��Qv^�V0R  % 1 ( & X )  �f�e  A c t i v e X   �[a� �VGr( CQ�e�N) 
  NE^�VGr ��Y�r�zMO�V
  NE^��Y�r�zMO�V            	 % 1   (W  % 2   -N   % 1   -   % 2                       	 Ջ�Vޏ�cpenc�n1Y%� ��U_Ɩ�N/ecTMR�y�R0 Ջ�VSb _h�<h1Y%� *gcf���h"}�vR0 �g�ԏ�V�vRpenc{|�WNggN&{0 �f�eb Rd��vO�V1Y%��N0 YL�penc���f�e0 �d\O1Y%���eS_MR��U_0 �f�eb Rd��d\O*g�m�S�NUOpencL�0 ��U_Ɩ/f�S��# O D B C   q��Rz�^N/ec  M i c r o s o f t   �W@x{|��Nۏ6Rpenc!j�W0 ňeQ@b ��v  O D B C 3 2 . D L L   ��N�vO�V1Y%�0 O D B C   q��Rz�^N/ec�R`��U_Ɩ0 ��/ec�_gq�R���O D B C   Y�`8nh/f�_���v0I O D B C   q��Rz�^N  M i c r o s o f t   �W@x{|penc�^{|N9SM�  ( A P I _ C O N F O R M A N C E   > =   S Q L _ O A C _ L E V E L 1   �_{�b�z) 0J O D B C   q��Rz�^N  M i c r o s o f t   �W@x{|penc�^{|N9SM�  ( A P I _ C O N F O R M A N C E   > =   S Q L _ O A C _ M I N I M U M   �_{�b�z) 0     Ջ�V��Ǐpenc�v	gHeX[.�:S��0 O D B C   q��Rz�^N/ec�R`��U_Ɩ0  ���&{T  O D B C   �v�N�~h�Q�vq��Rz�^0 O D B C   q��Rz�^N/ec�[MO�f�e0 N/ec@b��Bl�v�!j_0 penc��*b�e0 �h"}��U_�Q�0 �l	g~b0R@b ��v  O D B C   ۏeQ�p0nx�O  O D B C   �[ňcknx0
 �f�eb Rd��d\O1Y%�0  O D B C q��Rz�^N/ec�R`c��0  �eHe�vW[�kTbW[�k"}_0 fN~{N��  O D B C   q��Rz�^/ec0 fN~{(W��U_Ɩ-N*g�o;m0 < �] Rd�>             �e�l�[D A O / J e t   d b _�dۏL�R�YS0   ��vD F X <P�Spe0   D A O   T a b l e D e f bQ u e r y D e f �l	gSb _0   G e t R o w s   1Y%�0RM��~L��v�QX[NY0   G e t R o w s   K�<P��0�S��/fpenc{|�WN9SM� �b�v0   G e t R o w s   1Y%�0@b��Bl�vRN/f勰�U_<P�v N*NbR0                        ( *g�w)      �S_ �^(uz�^�]\O:S �z�S̀of �z�S�e,g ܃USh ܃US�eW[ ;m�R�vh��h N;m�R�vh��h ;m�R�vh��h�eW[	 N;m�R�vh��h�eW[ ;m�R��Fh N;m�R��Fh   �z�SFh�g wS�Rag 	c��h�b� 	c��4�q_ 	c���eW[ 	c���z�Q 1YHe�eW[ �z�Q �z�Q�v�eW[ ck8^SO ўSO �eSO ў�eSO A a B b Y y Z z  ( W[SO)  ( ��r�) 	 ( �VGr  -   % s ) � @b	g�VGr{|�W| * . b m p ; * . c u r ; * . d i b ; * . e m f ; * . i c o ; * . w m f | B i t m a p s   ( * . b m p ; * . d i b ) | * . b m p ; * . d i b | I c o n s / C u r s o r s   ( * . i c o ; * . c u r ) | * . i c o ; * . c u r | M e t a f i l e s   ( * . w m f ; * . e m f ) | * . w m f ; * . e m f | @b	g�e�N( * . * ) | * . * | |  *g�w �e MO�V CQ�e�N �Vh   ��r�yr'`u� ��r� W[SOyr'`u� W[SO �VGryr'`u� �VGr     Omȉ�VGr 0   -   �e 1   -   �V�[US*N                                 ��( & E ) 	 yr'`( & P ) . . .                                      �e�lSb _�VGr�e�N  % 1 0dk�e�N�S��NX[(WbN�S��0 �e�lňeQ�VGr�e�N  % 1 0勇e�N�S��<h_Ncknx0 �e�lňeQ�VGr�e�N  % 1 0勇e�N*Y'Y�QX[�e�l�[�~0 �e�lňeQ�VGr�e�N  % 1 0*g�NwQSO�f�v���d\O1Y%�0                         ^��l�Qpe�(u �n�Q �QX[N�� ����d� W[&{2Nzz��N�� Xhzz��N�� �e�NTb�e�N�S� �e�N*g~b0R �e�N!j_� �e�N�]�~Sb _	 ��Y��eQ/ ���Q�Q� �e�N�]�~X[(W ��U_��^� �x�v�]�n ��U_�Sx� �e�NT�       �e�N*YY ��YN�T *g���_���S �x�v*g�QY}Y	 _/ �e�N����� �l	g~b0R_ �eHe�vW[&{2N�W7h zz<PO(uNS_ �eHe�v�e�N<h_ �eHe�vyr'`<P �eHe�vpe�~Nh �yr'`(WЏL��e;RN����n �yr'`:N�S��yr'` O(uyr'`pe�~�e�_{�cfpe�~Nh N�S(Wُ*N�c6R
N��nyr'` �yr'`N�S(WЏL��e;R���S �yr'`:N�S�Qyr'` �l	g~b0Ryr'` �eHe�vjR4�g<h_ �eHe�VGr SbpS:g�Q� �e�lR�^X[.�Ǐz@b ����v4N�e�e�N
 �l	g~b0R���g~b�v�e,g ���fbc�v�e,g*Y�                        & < H E A D > < T I T L E > H T T P   ��  % d < / T I T L E > < / H E A D > ) < B O D Y > < H 1 > H T T P   ��  % d :   �e�S(u�mo`< / H 1 > < / B O D Y >  
 4 < B O D Y > < H 1 > �eHe��Bl< / H 1 > < P > �[7b�z�S�Q N*Ng�RhV�e�lYt�v��Bl. < b r > ��Bl:   % s  
 - < B O D Y > < H 1 >  ���t��[< / H 1 > < P > OmȉhV�et��[�R��bt��[1Y%�. < / B O D Y >  
 . < B O D Y > < H 1 > �ybk< / H 1 > < P > �[7b�l	gCgP��Ng�RhV���S�U R L . < / B O D Y >  
 1 < B O D Y > < H 1 > �l	g�S�s< / H 1 > < P > (W�g�RhV
N�l	g�S�s@b��Bl�vU R L . < / B O D Y >  
 3 < B O D Y > < H 1 > g�RhV��< / H 1 > g�RhVG�0R�Q���b��M�n�e�l�[b���Bl. < / B O D Y >  
 4 < B O D Y > < H 1 > �l	g�[�s< / H 1 > g�RhV�e�lgbL�勹e�l  < b > % s < / b >   dk�e. < / B O D Y >  
                    ibU\T 'Y�Q peW[ �n�R 9e�Q ��U_ GPT                    
 �^�z N*N�e�ech
 �e�^ Sb _ N*N�sX[�ech
 Sb _	 sQ�;m�R�ech
 sQ�	 �OX[;m�R�ech
 �OX[ (u N*N�e�e�NT�OX[;m�R�ech
 �SX[:N 9e�SSbpS	�y�
 u�b���n 9e�SSbpS:g�SSbpS	�y�
 SbpS��n	 SbpS;m�R�ech
 SbpS  	 >f:yteu�
 SbpS��ȉ     �Ǐ5u