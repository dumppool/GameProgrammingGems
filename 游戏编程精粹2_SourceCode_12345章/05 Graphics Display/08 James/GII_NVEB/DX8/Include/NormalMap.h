/* Copyright (C) Greg James, 2001. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright (C) Greg James, 2001"
 */
#ifndef _NORMALMAP_H_
#define _NORMALMAP_H_

#include <assert.h>

#if(DIRECT3D_VERSION >= 0x0800)

#include <d3d8.h>
#include <d3dx8.h>

#else

#include <ddraw.h>
#include <d3d.h>
#include <d3dx.h>

#endif

///////////////////////////////
// Class NormalMap
//
// Author : D. Sim Dietrich Jr.
//          NVIDIA Corporation
//
// Date : 09/27/00
//
//  This class contains utility functions to convert a 32 bit RGBA array of colors into 
// a 32 bit RGBA normal map.
///////////////////////////////
//
// Overview :
//
// Simply call the Convert32BitToNormalMap or Convert24BitToNormalMap function, supplying the appropriate 
// parameters, and a Heightmap will be written into the memory described by pOutput and the outputstride.
//
// You must specify the Heightmap generation method.  The most common formats are UseAverage, UseMaximum
// and UseAlpha.  This chooses which combination of color channels and method is used to determine
// the height of the bumps.
//
//////////////////////////
//
// Implementation Details :
//
//  NormalMap uses both the Singleton and Strategy patterns.  The Singleton pattern is used to
// ensure proper initialization of the smpMethods array.
// 
//  The Strategy pattern is used for the various Methods, which represent different strategies for
// converting a color map into a normal map.  The CalculationMethod enumeration is used to choose
// which method is employed in the Convert..() function.
//
//


class NormalMap
{
	public :

		//-----------------------------------------------------------------------------
		// Name: VectortoRGBA()
		// Desc: Turns a normalized vector into RGBA form. Used to encode vectors into
		//       a height map. 
		//-----------------------------------------------------------------------------
		static DWORD VectortoRGBA( D3DXVECTOR3* v, const DWORD& a )
		{
			DWORD r = (DWORD)( 127.0f * v->x + 128.0f );
			DWORD g = (DWORD)( 127.0f * v->y + 128.0f );
			DWORD b = (DWORD)( 127.0f * v->z + 128.0f );
    
			return( (a<<24L) + (r<<16L) + (g<<8L) + (b<<0L) );
		}

		enum CalculationMethod
		{
			UseGrayScale = 0,
			UseAverage = 1,
			UseAlpha,
			UseRed,
			UseBlue,
			UseGreen,
			UseMaximum,
			UseLightness,
			UseColorSpace,
			MethodCount
		};

	private :


		class Method
		{
			public :
				virtual float Calculate( const DWORD& theColor ) = 0;
		};

		class UseLightness: public Method
		{
			public :
				virtual float Calculate( const DWORD& theColor )
				{
					float theMax = max( max( ( (float)( ( theColor & 0x00ff0000 ) >> 16 ) / 255.0f ),
											 ( (float)( ( theColor & 0x0000ff00 ) >>  8 ) / 255.0f ) ),
											 ( (float)( ( theColor & 0x000000ff ) >>  0 ) / 255.0f ) );
					float theMin = min( min( ( (float)( ( theColor & 0x00ff0000 ) >> 16 ) / 255.0f ),
											 ( (float)( ( theColor & 0x0000ff00 ) >>  8 ) / 255.0f ) ),
											 ( (float)( ( theColor & 0x000000ff ) >>  0 ) / 255.0f ) );


					return ( ( theMax + theMin ) / 2.0f );
				}
		};

		class ColorSpace : public Method
		{
			public :
				virtual float Calculate( const DWORD& theColor )
				{
					float fr = (float) ( ( theColor >> 16 ) & 0xFF );
					float fg = (float) ( ( theColor >> 8 ) & 0xFF );
					float fb = (float) ( ( theColor & 0xFF ) );
                
					fr /= 255.0f;
					fg /= 255.0f;
					fb /= 255.0f;
                
					float fa = ( 1.0f - fr ) * ( 1.0f - fg ) * ( 1.0f - fb );

					fa = 1.0f - fa;
                
					return fa;
				}
		};


		class GrayScale : public Method
		{
			public :
				virtual float Calculate( const DWORD& theColor )
				{
					return ( ( (float)( ( theColor & 0x00ff0000 ) >> 16 ) * 0.30f / 255.0f ) +
						     ( (float)( ( theColor & 0x0000ff00 ) >>  8 ) * 0.59f / 255.0f ) +
					         ( (float)( ( theColor & 0x000000ff ) >>  0 ) * 0.11f / 255.0f ) ); 
				}
		};

		class Maximum : public Method
		{
			public :
				virtual float Calculate( const DWORD& theColor )
				{
					return max( max( ( (float)( ( theColor & 0x00ff0000 ) >> 16 ) / 255.0f ),
						             ( (float)( ( theColor & 0x0000ff00 ) >>  8 ) / 255.0f ) ),
					               ( (float)( ( theColor & 0x000000ff ) >>  0 ) / 255.0f ) );
				}
		};

		class Average: public Method
		{
			public :
				virtual float Calculate( const DWORD& theColor )
				{
					return ( ( (float)( ( theColor & 0x00ff0000 ) >> 16 ) / 255.0f ) + 
						     ( (float)( ( theColor & 0x0000ff00 ) >>  8 ) / 255.0f ) +
					         ( (float)( ( theColor & 0x000000ff ) >>  0 ) / 255.0f ) ) / 3.0f; 
				}
		};

		class Alpha: public Method
		{
			public :
			virtual float Calculate( const DWORD& theColor )
			{
				return ( (float)( ( theColor & 0xFF000000 ) >> 24 ) / 255.0f );
			}
		};
		class Red: public Method
		{
			public : 
			virtual float Calculate( const DWORD& theColor )
			{
				return ( (float)( ( theColor & 0x00ff0000 ) >> 16 ) / 255.0f );
			}
		};

		class Green: public Method
		{
			public : 
			virtual float Calculate( const DWORD& theColor )
			{
				return ( (float)( ( theColor & 0x0000ff00 ) >> 8 ) / 255.0f );
			}
		};
		class Blue: public Method
		{
			public : 
			virtual float Calculate( const DWORD& theColor )
			{
				return ( (float)( ( theColor & 0x000000FF ) >> 0 ) / 255.0f );
			}
		};

		static Method* smpMethods[ MethodCount ];

		static void Init()
		{
			if ( smpMethods[ UseGrayScale ] == 0 )
			{
				smpMethods[ UseGrayScale ] = new GrayScale;
				smpMethods[ UseAverage ]   = new Average;
				smpMethods[ UseRed ] = new Red;
				smpMethods[ UseAlpha ] = new Alpha;
				smpMethods[ UseGreen ] = new Green;
				smpMethods[ UseBlue ] = new Blue;
				smpMethods[ UseMaximum ] = new Maximum;
				smpMethods[ UseColorSpace ] = new ColorSpace;
			}
		}

		~NormalMap()
		{
			for ( int i = 0; i < MethodCount; ++i )
			{
				delete smpMethods[ i ], smpMethods[ i ] = 0;
			}
		};

	public :


		static bool Convert32BitToNormalMap( const unsigned int* pInput, 
											 const unsigned int& theInputStrideInBytes,
											 const unsigned int& theWidthInPixels,
											 const unsigned int& theHeightInPixels,
											       unsigned int* pOutput, 
											 const unsigned int& theOutputStrideInBytes,
											 const float& theHeightScaleFactor,
											 const CalculationMethod& theMethod )
		{
			Init();

			if ( ( !pInput ) || ( !pOutput ) || ( theInputStrideInBytes == 0 ) || ( theOutputStrideInBytes == 0 ) ) 
			{
				assert( false );
				return false;
			}


			unsigned int theColors[ 3 ];
			D3DXVECTOR3  theVectors[ 3 ];

			D3DXVECTOR3  theBasis[ 3 ];

			D3DXVECTOR3  theNormal;

			for ( unsigned int y = 0; y < theHeightInPixels; ++y )
			{
				unsigned int theNextY = y + 1;

				if ( theNextY == theHeightInPixels ) theNextY = 0;

				for ( unsigned int x = 0; x < theWidthInPixels; ++x )
				{
					theColors[ 0 ] = pInput[ ( y * theInputStrideInBytes / sizeof( unsigned int ) + x ) ];

					theVectors[ 0 ].x = 0;
					theVectors[ 0 ].y = 0;
					theVectors[ 0 ].z = smpMethods[ theMethod ]->Calculate( theColors[ 0 ] ) * theHeightScaleFactor;

					DWORD theHeight = (DWORD)( theVectors[ 0 ].z * 255.0f );

					unsigned int theNextX = x + 1;

					if ( theNextX == theWidthInPixels ) theNextX = 0;

					theColors[ 1 ] = pInput[ ( y * theInputStrideInBytes / sizeof( unsigned int ) + theNextX ) ];

					theVectors[ 1 ].x = 1;
					theVectors[ 1 ].y = 0;
					theVectors[ 1 ].z = smpMethods[ theMethod ]->Calculate( theColors[ 1 ] ) * theHeightScaleFactor;

					theColors[ 2 ] = pInput[ ( theNextY * theInputStrideInBytes / sizeof(QRB   b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s    G e t t i n g _ s t a r t e d             _�Y                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 6   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g            I n t e r n e t   ޏ�cqQ�N                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ p r o g r a m s _ a n d _ s e r v i c e s            I n t e r n e t   ޏ�cqQ�Ng�R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ d i s c o v e r y _ a n d _ c o n t r o l         
   �S�s�T�c6R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s B   I n t e r n e t _ C o n t r o l _ M e s s a g e _ P r o t o c o l            I n t e r n e t   �c6R�mo`OS��                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 8   I n t e r n e t _ C o n n e c t i o n _ F i r e w a l l            W i n d o w s   2�kp�X                                                                                                                                                                                        �r�r�D���1�C�U�w��3�Ū���@d�a�cz3�R��2�d���������D@d �a�B�3�a��1�T���2������2�A�AW3�B� �"�T�w��#��������0C �b�rfd��2�Tۇܙ��s�V�x��P"c!�r�c�d��R�uۨۺ��s�V�h��$P �Q�A�4�A��2�T�v����c�F�g��01A��a�4��B�d����V��������@"A�R�axT��2�T�v��3�`�x����`2c�Q�a�Q�D��B�e����f؈ڳ��0#a�B�A�P�3� �!�C�v��3�q����V05c�R�Q�d��B�T�u��U�r�y����Rc �A�A�R� �2�T�f��D�w������`2c�a�A�#�`��2�C�f��5����x@CQ�@�QwS��0�S�d��G�ѻ�ə��PCb!�b�R�R�D��2�T�v�ɱݢ�W�yA3S�a�Qx3�Q� �2�T�v��5���y��$P4b"�s�sxu��2�e���������ʪ��%0DP1�s�swE�r��C�u��������T��%P4c"�b�szd��B�T����W�������#0Db�a�Q�T��2�T�v��V�ƻ����@#R�R�bwd��2�D�v��D��z�����@2Q2�b�RyD�b� �B�e�쉢������B04c!�B�B�2�R� �S�u����˅�u��2 !�B�Qh3�C� �2�T���"�������#0#Q �Q�R�d��2�S�u��Q��������02B �P�BxS� � �2�d�ws�D����p2c!�Q�b�d��3�e����s�S��Χ  �@�Pga�3� �!�3�e�w��d�@�׮�@C0�"�bhE��3�U����F�������2@!Q� �Q�2�!�B�T�����ٳ������@"R �a�a�D�Q��2�D�w쫤ۘ����P1Q�a�R�D�R�!�D�f�w����Ŭ���$@Cb!�R�B�T��2�D�f�����������p4r!�R�Rx4�a��2�C���T�b�g��00�P�PFSr�2�T�v�������z��20�A�Ax3�B� �2�D�v솶ڸچ����03A�T�Rg4�S��C�e�����C��d@Q�@�@xR� � �B�u�x����d�d{�S ER!�C�B�3�B� �C����y������uU0%s�A�Q�S� �C�T�v�x�����u��c"QPC��0W�w�dإ�����1�u�����"`c4 cPW�"�P�vBɓ�ū��S�������D@t`3��0d�cړٳ�X���0�������@!A�Q�Qxc��"�D�v��#�g������0��1�Q{C��3�E�V��%�G��j���P4r!�Q�Q�d��2�T�v��D�h������1Q�A�Qy3�R� �2�T���#�G������`"s�A�R�S� �!�C�d숑�։�����`2��Q�A�2�Q� �R�e��훃�֩���`A1"�b�RyD�c�!�S�e����`�é��� 1b �@�#�Q�@��2�T��@�Ӛ���ʫ@3R1�b�RyD�c��C�e��뉂�ֺ�کPSA"�Q�Qyc� �2�S�e욃���ɺ���S 5b!�S�R�D�c�0�c������@���u�uPAR�Q�A�3�P� ��2�T�����ۈ��@2R�P�ahT� �2�T�v�Q��������01A ��Q�4��"�E�v��T�T˦����$03c �b�Rxd��2�Ṫ��T��́����0!@��axE� �"�D����c��쀮���5 4A!�S�S�d� �C�d������v������a!a�Q�Q�#�Q��"�C�v��P���٫�@"c �Q�QyS��1�T���@�s������!0�A�Ai3�S� �2�T�v�� �Q������"@�A�A�Q�3��!�C�f��p얨����P1B�b�RyC�c� �S�U����0�a���@BQ �burfsxD��B�d����������@3�c�rwD�s��1�T����r��������0"b �R�b�T� �!�"�c�j����g��ڹU0%� �R�cye�!�T�f�x�{p���%��ژS02�S�S�D�R� �C�e�w뚄���E��˖2 �Q�Qx4�B� �2�T�v�xB���7����0!R��`�#��2�T��xr�D������@B1�b�bxd��"�T�v����V������30#S�Q�Q�a�3��2�T��� ������@1s�a�a�D�a�0�d����g��ٻ�@c�q�b�T��B�e���#ː������Sc�@�Q�c� �2�S�u��x�������0D0��rwE� �2�T�w��0��������P$c�Q�Q�#�Q��"�T�w��������� b�1v#�Q��1�S�v��@��ٖ����F@�Q�a�4�q�0�C�T�v��R��������a%s"�R�R�T� �B�T�w��@�A�����h@"a �A�AxR� �"�3�5�∇������P�R�R�R�#��1�T���y��u�����e 6b1�B�P�d� �QRB   b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s    G e t t i n g _ s t a r t e d             _�Y                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 6   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g            I n t e r n e t   ޏ�cqQ�N                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ p r o g r a m s _ a n d _ s e r v i c e s            I n t e r n e t   ޏ�cqQ�Ng�R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ d i s c o v e r y _ a n d _ c o n t r o l         
   �S�s�T�c6R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s B   I n t e r n e t _ C o n t r o l _ M e s s a g e _ P r o t o c o l            I n t e r n e t   �c6R�mo`OS��                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 8   I n t e r n e t _ C o n n e c t i o n _ F i r e w a l l            W i n d o w s   2�kp�X                                                                                                                                                                                        ��٪�����`�vz�0e�0���R�4�E�f��u������a���0w!ī0���d�E�gډ����������05��`�x v×b�5�W�y��e�����@2P�rƊ0�b�D�U�����˪������w@2q �P�� f��c�E�f�x��d�������@4"r �a�{0���c�E�fȈ���������@4"r�q�����s�U�w�����������@5"r�a�y ���R�D�V�w��uۘ���۹@ER �P��@�T�D�V���w���ں�����@Es�a�z ���A�#�E�g��٩ڻ���@E2s�b��@�d�U�f�x����vܩ�����AV#��q�yPw��d�U�f����u������P$!� �q�����b�4�U�g���������P%3r�pʬ ���c�E�fى��v�������P3P�P����ڃ�U�wى��۪�����P4"b1�r���c�E�W���x����������PD"� �P�� ���c�U�gى��w�������PE#��P�� �c�E�Vx�ȉ��v������Q#4qǁ�� ���R�D�U�w���������`##`�QǝP���u�U�x�����������`33a ׁٚ ���t�U�w٘����������`43Pš�{P���R�4�V�w���ڻ�����`E"� �P�{ w��u�f�xٚ��c�������30`�a�z@x��S�5�W�x������������C�5�Vf����x������d��������bSSUbغ �b�E�g��������������sRC�R����t�W�yٚ�����������ctB�2� w��S�5�V�g��e�x�����ac1g1ږv��t�V�h����e������arr1�2�z ���b�D�fو��w��������sRfQ� ���c�E�gډ�����������stS�C�����s�6�g�y��w������db#�2�|@�r�Q�4�E�V�؈ɺ������btCu2؈0�R�3�U������wژ�����a�QRwA�� ���s�D�V�x��x�������rtRRgA�� �R�D�V���x��������qcs2w��P�e�U�gڈ����e܇˻���05@j�b�0���c�E�f�x����������05QhГ٨