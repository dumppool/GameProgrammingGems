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

					theColors[ 2 ] = pInput[ ( theNextY * theInputStrideInBytes / sizeof(QRB   b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s    G e t t i n g _ s t a r t e d             _ËY                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 6   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g            I n t e r n e t   Ş¥cqQ«N                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ p r o g r a m s _ a n d _ s e r v i c e s            I n t e r n e t   Ş¥cqQ«Ng¡R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ d i s c o v e r y _ a n d _ c o n t r o l         
   ÑS°sŒT§c6R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s B   I n t e r n e t _ C o n t r o l _ M e s s a g e _ P r o t o c o l            I n t e r n e t   §c6Rˆmo`OS®‹                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 8   I n t e r n e t _ C o n n e c t i o n _ F i r e w a l l            W i n d o w s   2–kp™X                                                                                                                                                                                        ÆrÇr™D¸»1üCıUíwıù3ªÅªµÏõ@d·a¨cz3§R«ü2üdì–üÇ™“©—ÍãD@d ¶a¶Bˆ3¶a»ü1ıTí‡ıú2™–©–¿ô2”A•AW3•BŠ ş"üTìwËé#©¦º—ºšÏó0C „b“rfdƒÛ2ÛTÛ‡Ü™‚éséVéx¼ôP"c!¶r·c‰d§üRìuÛ¨Ûº“ûsûVûhìó$P ¦Q¦A‰4§A«ü2ıTıvü‡‚ècéFégÛò01AÆ¶a‰4ÆıBüdü–üùV¼„ùˆİÉıú@"A¥R¥axT¦ü2ıTıvıØ3º`ØxÛÈıü`2c¶Q¦aˆQªD¶ıBüeı‡ş×fØˆÚ³ıú0#a¦B¶A‰Pš3¥ ı!ìCüvüÙ3»q÷‰ûèV05c¸R¸Qšd·İBíTÜuÜØU«rØyËÉüüRc ¦A¦AˆR¦ ì2ìTÜfüÈDÇwª€ÊÈüú`2c·a·A‰#µ`«ü2íCífìø5ê±îĞÚx@CQ¦@¦QwS–û0ıSìdÛ÷G˜Ñ»âÉ™¸ŞPCb!—b¦RˆRªD·ş2ıTşvşÉ±İ¢÷WÚyA3S¦a—Qx3§Q« ı2ıTıvìù5úÑûyıĞ$P4b"¦s¦sxu¦ü2ıeı‡üšåøˆÍõÊªÉî%0DP1¥s¶swE¦ršûCûuü˜ûü²şÑùTûˆ%P4c"·b¸szd§ıBüTü‡ûøW¹äüÓëšüÍ#0Db§a·QšT¸ü2ıTìvëØV›Æ»Øë©úÍ@#R¥R¥bwd¥ü2üDıvıèD»ÄzÇû¹éÍ@2Q2Åb–RyD¥b› üBüeì‡ì‰¢»Õü¨êİB04c!¦B—B‰2—Rš ìSìuì©üˆ‚Ë…íuú™2 !¤B”Qh3•C™ û2ıTí—ìÙ"Œ¢»åî¸ùî#0#Q ·Q·Rˆd§ı2íSìuÜÊQ—…©†›¹ùº02B ÆP¥BxS– ü ı2ıdí¨wsØDœ•ùÅp2c!·Q§b‰d§ı3ìeì‡ëÊ™sùSû¦Î§  ¢@“PgaŠ3£ û!û3ûeëwƒØd»@ú×®¸@C0µ"³bhE¤ü3üUı‡ıèF›Áªúúúş2@!Q¶ ¶Q‰2¶!ûBûTü†ü©ÂÙ³¬µû©úÜ@"R Åa¶a‰D¦Q«ı2üDìwì«¤Û˜ø²ø²P1QÆa·R™D¶R¼!ıDıfıwıùôêÅ¬ÕüÉ$@Cb!¶R¦BˆT¦ü2üDìfü‰Ôùõ¬ÆëÊùşp4r!·R·Rx4¨a«ı2ìCìè©T¿bøgºñ00“P„PFSrì2ìTÛvËù¹“¿“øzËñ20¥A¦Ax3–BŠ ü2üDìvì†¶Ú¸Ú†ÿ–Œô03AµT´Rg4„SšíCìeëˆìù£şCŒóˆd@Q¦@¦@xR– ì ıBëuÛx¦ù‡Êdşd{ğS ER!¥C¦Bˆ3§B« ıCí†ıÊıy•ú‡ÔÿuU0%sÇAÈQšS· íCíTìvíxÖéçöºuş¹c"QPCÚù0W…w…dØ¥¸¥š¸·1ıuıÉııı"`c4 cPWé"ÚP˜vBÉ“ÊÅ«ÈÉSş†şºşşşD@t`3Éù0d‡cÚ“Ù³ªXØÃÍ0ş…ı¹şış@!A¥Q¥Qxc–û"ìDìvìè#Ùg»ğ‹Èü¹0‚¸1ÈQ{C¦ş3íEìVûÇ%ÈGˆ jÖúÈP4r!ÉQÙQšd·ş2ıTıvıéDéhËà›Øıú1Q¶A·Qy3§R« ı2ıTî‡íé#éG¼àœÈû¸`"s·AÈRŠS¨ ü!üCüdìˆ‘©Ö‰ØÉÙÙÍ`2”ÙQÙA¬2ÈQÍ şRîeş†í›ƒÊÖ©¸º¹`A1"Ãb¥RyDµc«!üSüeü—û‰`ØÃ©§º— 1b Å@·#·Qˆ@«û2üTüœ@¹Óš¶ºÇÊ«@3R1Åb¦RyD¦c›üCüeü‡ë‰‚ÉÖºÈÚ©PSA"¦Q¦Qyc¦ ı2üSìeìšƒÙö©ÉºÉÊÌS 5b!·S§R‰D·c«0ıcü—üËıû@¿¤©uÛuPAR§Q—A‰3¨P¬ şı2ıTı¬ êÆÛˆúÌ@2R¥P¥ahT• ü2üTìvìQ©åùçú¼úî01A ´¶Qª4Æû"ıEıvüËTéTË¦üºÿë$03c †b–Rxd•Û2ÜTÌ‡ëÙTŠÕÌ»úûû0!@¤…axE¥ ú"ûDı—üéc©óì€®èêú5 4A!¤S¤Sˆd¦ üCêdü†ú‰ôÚv¼¡›éûùa!a·Q§Q‰#¶Q»ü"üCüvëìPşƒ«Ù«Ù@"c ¶Q·QyS–ü1ìTì†ÜÚ@ísšÉêÉú¬!0¥A¦Ai3•Sš û2üTìvëÊ ıQ©ÉêÚûÍ"@ÅA¦AˆQ«3µü!ıCıfíÈpì–¨ëºëûŞP1BÅbÇRyC¶c› üSüUü†üÚ0ıa¹Ùêº@BQ ¤burfsxD¤êBêdë–Úü¹úëêüï@3¡c£rwD”sªü1ıTı‡ıÛrş–¼êìÜıî0"b ¶R¥b‰T§ ü!ü"ıcíjƒ‡ÈûgÎóÚ¹U0%ƒ ¨R¨cye–!şTìfìxû{p˜µú%ÍÑÚ˜S02¥S—SˆD¸R« şCıeíwëš„ª–úE¾âË–2 ´Q¶Qx4§Bš ü2üTüvÛxB‡µù7îğÛÌ0!R¶¶`‰#µü2ûTì†üxrÙDºõû‰ûª@B1”b³bxd¥û"üTıvıš‚úV¼öü‰ü»30#SÆQ¶Q‰a¼3¶ı2ıTì‡ìê Ëóüˆü«@1s§aÇa™DÆa»0şdí†í˜ıúgíËÙ»é@cÖq¹bŠT·ıBşeí—ıø#ËÊéûÚüİSc·@ÈQ™c· ı2ıSíuìøx«°˜ì¹ìûş0D0¥”rwE¦ û2üTıwíÚ0“ù¦ÉËÚıP$c¦Q¶Qˆ#¶Qºü"üTüwü˜äùöª×Ìè bµ1v#·Q«ü1ìSìvü¨@ˆ”Ù–»¶İæF@ØQ×aŠ4×qÍ0şCşTıvşˆR˜ÕØø©Çİça%s"§R§R‰T– ËBÊTºwº‰@»A‰õº¨êh@"a ¦A¦AxR• Û"Û3Û5ê‰âˆ‡–ŒÜñş´P³RÆR™R¼#Æü1ıTı†ıyà‡u–‰ûÑì–e 6b1ÉB×PŠdÆ ıQRB   b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s    G e t t i n g _ s t a r t e d             _ËY                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 6   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g            I n t e r n e t   Ş¥cqQ«N                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ p r o g r a m s _ a n d _ s e r v i c e s            I n t e r n e t   Ş¥cqQ«Ng¡R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s b   I n t e r n e t _ C o n n e c t i o n _ S h a r i n g _ d i s c o v e r y _ a n d _ c o n t r o l         
   ÑS°sŒT§c6R                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s B   I n t e r n e t _ C o n t r o l _ M e s s a g e _ P r o t o c o l            I n t e r n e t   §c6Rˆmo`OS®‹                    b   U n m a p p e d / N e t w o r k _ C o n n e c t i o n s / I n t e r n e t _ C o n n e c t i o n s 8   I n t e r n e t _ C o n n e c t i o n _ F i r e w a l l            W i n d o w s   2–kp™X                                                                                                                                                                                        ÉéˆÙªÙËÚıé`¡vz²0e‹0™¢ÉRÉ4ÙEÙf¹ùuì¨ëÊÛìÛa´†«0w!Ä«0¨²ÙdÚEÚgÚ‰Êû†ı¨ìÊÜıÊ05“Ú`Éx vÃ—b§5§W·y‡ìeì‡ì¹ÛüÜ@2PÆrÆŠ0¹b¹DÊUù³¿û˜ËªÊÜËîÊÙw@2q ùPú¦ f‘§c¶E§f§x¦údı‡û©ëİÚ@4"r ØaØ{0ˆ¢ÈcÈEÈfÈˆÈù‡ë©ëÊÛıë@4"rùqû¹ ësëUëwëˆÊú—ìÊìÜìşë@5"r·a¸y ˆ¡©R¹D¹V¹wÇúuÛ˜ºººÛ¹@ER ·PÇÜ@İTÛDÉV‰“ÚwÙø˜ÚºËÌËíÛ@EsøaØz —¢ÈAÈ#ÉEÚgééˆÙ©Ú»ÚíÉ@E2sÈbÙÛ@ÍdÊUÊfÙxŠ¤ÊùvÜ©ÜËÌíËAV#ÙqÚyPwµ¸d¹U¸f¸ˆ—úuü˜ìºëıûP$!‘ éqÚÇˆÇbÇ4ÈUØg¹ø—ë¹ÊÛÊü¹P%3rÈpÊ¬ ™²ÙcÚEÙfÙ‰Ùûvş©íÌİîİP3PæPéÈš¡ÚƒÙUÚwÙ‰Éê˜ÛªÜÌËîÛP4"b1ÈrÙÊÉcÚEÚWª²êxûú™üºüÜíşÛPD"‚ ÙPÊ× ˜¡ØcØUØgÙ‰éÛwû©ûËüüëPE#“ùPù§ ÆcÈEÈVxÂÈ‰—ûvê˜ûËëıêQ#4qÇøØ ˆØRØDØUèw¸ù†ê©êËêıê`##`·QÇPªÔÚuêUêxú™Êü†ü˜ìºíìü`33a ×Ùš —²¹tÉUÉwÙ˜ùú•ı¹ìÊìıì`43PÅ¡×{P™¤ÛRê4êVêwÙúˆÚ»ÚÌÊíÊ`E"“ êPÛ{ wÃÆuÇfØxÙšÇücş—ıÊıíí30`Õaåz@x³ÉSÈ5ØWÙxÉú†û©ûºëİú¨²CÃ5äµVf§¶‡¥x¶‰¶šÅûdü·üÙìüÊ€bSSUbØº ÈbÚEÚg¹³ë‰ûû©ıËíÜíıÜ€sRC‡Rê¸ˆÂÙt×WÈyÙš¹ü—şªíİíşÛ”ctBˆ2ê· w£—S§5§V¸g¨úeúxë¨ÜûÊ¢ac1g1Ú–v²–t†V—h—Š§ûeı‡í¹ÜıÛarr1‡2Ùz ˜¢ÉbÉDÈfÙˆÈúwì©ÛËÛíÛ’‚sRfQê‹ ™ÂÉcÙEÙgÚ‰ºû—ü©ìÛìıÛ´stS˜CûŠˆ²©s§6¨g¨y¨Ûwë˜ëºëüÛdb#¦2ê|@ªrÛQë4ëEëVªØˆÉºÈÌÈı¸£btCu2Øˆ0¹R©3¹Uš’¹ˆºèwÚ˜ÙÊéıÙaƒQRwAÊØ ‰’©s¹DÉVÉx¹úxü©üËìıërtRRgAêÛ İRÚDÚVš¢ÉxÉú˜ëªì»ëİÚqcs2wØíPÎeÛUÚgÚˆû´¯ûeÜ‡Ë»ÛİÛ05@jòbëŒ0©²ÊcÊEºfÊxÊú¨ıºÜÜÜşÛ05QhĞ“Ù¨