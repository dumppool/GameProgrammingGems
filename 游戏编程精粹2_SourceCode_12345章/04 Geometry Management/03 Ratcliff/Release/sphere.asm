	TITLE	C:\WINDOWS\Desktop\Gems2 CD\SourceCode\04 Geometry Management\03 Ratcliff\sphere.cpp
	.386P
include listing.inc
if @Version gt 510
.model FLAT
else
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
_BSS	SEGMENT DWORD USE32 PUBLIC 'BSS'
_BSS	ENDS
$$SYMBOLS	SEGMENT BYTE USE32 'DEBSYM'
$$SYMBOLS	ENDS
$$TYPES	SEGMENT BYTE USE32 'DEBTYP'
$$TYPES	ENDS
_TLS	SEGMENT DWORD USE32 PUBLIC 'TLS'
_TLS	ENDS
;	COMDAT ??_C@_00A@?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0FG@ILEN@?3Sun?3Sunday?3Mon?3Monday?3Tue?3Tuesd@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0IH@JLLA@?3Jan?3January?3Feb?3February?3Mar?3Ma@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_05NAGO@false?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_04NCCD@true?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_02ELCB@no?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_03LGKI@yes?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_01FKHJ@C?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BP@KLJH@invalid?5vector?$DMbool?$DO?5subscript?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_C@_0BF@MMAA@missing?5locale?5facet?$AA@
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
CRT$XCA	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCA	ENDS
CRT$XCU	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCU	ENDS
CRT$XCL	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCL	ENDS
CRT$XCC	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCC	ENDS
CRT$XCZ	SEGMENT DWORD USE32 PUBLIC 'DATA'
CRT$XCZ	ENDS
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT _sqrtf
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0_Lockit@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_Lockit@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bad_exception@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@bad_exception@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gbad_exception@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bad_exception@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bad_alloc@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@bad_alloc@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gbad_alloc@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bad_alloc@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??2@YAPAXIPAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemchr
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemcmp
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemcpy
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemmove
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT _wmemset
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy@?$char_traits@G@std@@SAPAGPAGPBGI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?to_char_type@?$char_traits@G@std@@SAGABG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?to_int_type@?$char_traits@G@std@@SAGABG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?eq_int_type@?$char_traits@G@std@@SA_NABG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?eof@?$char_traits@G@std@@SAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?assign@?$char_traits@D@std@@SAXAADABD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?length@?$char_traits@D@std@@SAIPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy@?$char_traits@D@std@@SAPADPADPBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?move@?$char_traits@D@std@@SAPADPADPBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?to_char_type@?$char_traits@D@std@@SADABH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?to_int_type@?$char_traits@D@std@@SAHABD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?eq_int_type@?$char_traits@D@std@@SA_NABH0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?eof@?$char_traits@D@std@@SAHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0logic_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1logic_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?what@logic_error@std@@UBEPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@logic_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Glogic_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0logic_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1domain_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@domain_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gdomain_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0domain_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1invalid_argument@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@invalid_argument@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Ginvalid_argument@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0invalid_argument@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1length_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@length_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Glength_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0length_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0out_of_range@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1out_of_range@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@out_of_range@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gout_of_range@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0out_of_range@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0runtime_error@std@@QAE@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1runtime_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?what@runtime_error@std@@UBEPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@runtime_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gruntime_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0runtime_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1overflow_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@overflow_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Goverflow_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0overflow_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1underflow_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@underflow_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gunderflow_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0underflow_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1range_error@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@range_error@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Grange_error@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0range_error@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bad_cast@std@@QAE@PBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bad_cast@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@bad_cast@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gbad_cast@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bad_cast@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bad_typeid@std@@QAE@PBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1bad_typeid@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@bad_typeid@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gbad_typeid@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0bad_typeid@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G__non_rtti_object@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1__non_rtti_object@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0_Timevec@std@@QAE@PAX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1_Timevec@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4_Timevec@std@@QAEAAV01@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Getctype@_Locinfo@std@@QBE?AU_Ctypevec@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Getcvt@_Locinfo@std@@QBE?AU_Cvtvec@@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Bid@locale@std@@QAEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Incref@facet@locale@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Decref@facet@locale@std@@QAEPAV123@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1facet@locale@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0facet@locale@std@@IAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0locale@std@@QAE@W4_Uninitialized@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0locale@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1locale@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gfacet@locale@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Narrow@std@@YAHG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Widen@std@@YAGDPAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0codecvt_base@std@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_always_noconv@codecvt_base@std@@MBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_max_length@codecvt_base@std@@MBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_encoding@codecvt_base@std@@MBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gcodecvt_base@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1codecvt_base@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$codecvt@GDH@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Init@?$codecvt@GDH@std@@IAEXABV_Locinfo@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_in@?$codecvt@GDH@std@@MBEHAAHPBD1AAPBDPAG3AAPAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_out@?$codecvt@GDH@std@@MBEHAAHPBG1AAPBGPAD3AAPAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_length@?$codecvt@GDH@std@@MBEHAAHPBG1I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_always_noconv@?$codecvt@GDH@std@@MBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_max_length@?$codecvt@GDH@std@@MBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_encoding@?$codecvt@GDH@std@@MBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$codecvt@GDH@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0ctype_base@std@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gctype_base@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1ctype_base@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?is@?$ctype@D@std@@QBE_NFD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$ctype@D@std@@QAE@PBF_NI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$ctype@D@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Term@?$ctype@D@std@@KAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Init@?$ctype@D@std@@IAEXABV_Locinfo@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_tolower@?$ctype@D@std@@MBEDD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_tolower@?$ctype@D@std@@MBEPBDPADPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_toupper@?$ctype@D@std@@MBEDD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_toupper@?$ctype@D@std@@MBEPBDPADPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$ctype@D@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1failure@ios_base@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Doraise@failure@ios_base@std@@MBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdstate@ios_base@std@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setstate@ios_base@std@@QAEXH_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?good@ios_base@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?fail@ios_base@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?exceptions@ios_base@std@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?flags@ios_base@std@@QBEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setf@ios_base@std@@QAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setf@ios_base@std@@QAEHHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?unsetf@ios_base@std@@QAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?getloc@ios_base@std@@QBE?AVlocale@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gfailure@ios_base@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0failure@ios_base@std@@QAE@ABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_Gios_base@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0reference@?$vector@_NV?$allocator@I@std@@@std@@QAE@IPAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4reference@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@ABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4reference@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Breference@?$vector@_NV?$allocator@I@std@@@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0const_iterator@?$vector@_NV?$allocator@I@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0const_iterator@?$vector@_NV?$allocator@I@std@@@std@@QAE@IPBI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Dconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE?BVreference@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Econst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Yconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Zconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Hconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE?AV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Gconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE?AV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Gconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBEHV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8const_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??9const_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Mconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Dec@const_iterator@?$vector@_NV?$allocator@I@std@@@std@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Inc@const_iterator@?$vector@_NV?$allocator@I@std@@@std@@IAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0iterator@?$vector@_NV?$allocator@I@std@@@std@@QAE@IPAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Diterator@?$vector@_NV?$allocator@I@std@@@std@@QBE?AVreference@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Eiterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Fiterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Yiterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Ziterator@?$vector@_NV?$allocator@I@std@@@std@@QAEAAV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Hiterator@?$vector@_NV?$allocator@I@std@@@std@@QBE?AV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Giterator@?$vector@_NV?$allocator@I@std@@@std@@QBE?AV012@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Giterator@?$vector@_NV?$allocator@I@std@@@std@@QBEHV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??9iterator@?$vector@_NV?$allocator@I@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Miterator@?$vector@_NV?$allocator@I@std@@@std@@QBE_NABV012@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?capacity@?$vector@_NV?$allocator@I@std@@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?begin@?$vector@_NV?$allocator@I@std@@@std@@QAE?AViterator@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?begin@?$vector@_NV?$allocator@I@std@@@std@@QBE?AVconst_iterator@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?end@?$vector@_NV?$allocator@I@std@@@std@@QAE?AViterator@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?end@?$vector@_NV?$allocator@I@std@@@std@@QBE?AVconst_iterator@12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?size@?$vector@_NV?$allocator@I@std@@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?insert@?$vector@_NV?$allocator@I@std@@@std@@QAE?AViterator@12@V312@_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?insert@?$vector@_NV?$allocator@I@std@@@std@@QAEXViterator@12@I_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?insert@?$vector@_NV?$allocator@I@std@@@std@@QAEXViterator@12@Vconst_iterator@12@1@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?erase@?$vector@_NV?$allocator@I@std@@@std@@QAE?AViterator@12@V312@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?erase@?$vector@_NV?$allocator@I@std@@@std@@QAE?AViterator@12@V312@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?swap@?$vector@_NV?$allocator@I@std@@@std@@QAEXAAV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Nw@?$vector@_NV?$allocator@I@std@@@std@@KAII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Trim@?$vector@_NV?$allocator@I@std@@@std@@IAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Xran@?$vector@_NV?$allocator@I@std@@@std@@IBEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBDABV?$allocator@D@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV01@PBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?c_str@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_ostream@DU?$char_traits@D@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?put@?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEAAV12@D@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?flush@?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEAAV12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_ios@DU?$char_traits@D@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setstate@?$basic_ios@DU?$char_traits@D@std@@@std@@QAEXH_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@?$basic_ios@DU?$char_traits@D@std@@@std@@QBEPAV?$basic_streambuf@DU?$char_traits@D@std@@@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_ostream@GU?$char_traits@G@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?put@?$basic_ostream@GU?$char_traits@G@std@@@std@@QAEAAV12@G@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?flush@?$basic_ostream@GU?$char_traits@G@std@@@std@@QAEAAV12@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_ios@GU?$char_traits@G@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setstate@?$basic_ios@GU?$char_traits@G@std@@@std@@QAEXH_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?rdbuf@?$basic_ios@GU?$char_traits@G@std@@@std@@QBEPAV?$basic_streambuf@GU?$char_traits@G@std@@@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_istream@DU?$char_traits@D@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0sentry@?$basic_istream@DU?$char_traits@D@std@@@std@@QAE@AAV12@_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Bsentry@?$basic_istream@DU?$char_traits@D@std@@@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_streambuf@DU?$char_traits@D@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sgetc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?snextc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?overflow@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pbackfail@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?showmanyc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?underflow@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?uflow@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?xsgetn@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHPADH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?xsputn@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHPBDH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?seekoff@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAE?AV?$fpos@H@2@JW4seekdir@ios_base@2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?seekpos@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAE?AV?$fpos@H@2@V32@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setbuf@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEPAV12@PADH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sync@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?imbue@?$basic_streambuf@DU?$char_traits@D@std@@@std@@MAEXABVlocale@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_istream@GU?$char_traits@G@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0sentry@?$basic_istream@GU?$char_traits@G@std@@@std@@QAE@AAV12@_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Bsentry@?$basic_istream@GU?$char_traits@G@std@@@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$basic_streambuf@GU?$char_traits@G@std@@@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sgetc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@QAEGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?snextc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@QAEGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?overflow@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pbackfail@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?showmanyc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?underflow@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?uflow@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?xsgetn@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEHPAGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?xsputn@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEHPBGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?seekoff@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAE?AV?$fpos@H@2@JW4seekdir@ios_base@2@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?seekpos@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAE?AV?$fpos@H@2@V32@H@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?setbuf@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEPAV12@PAGH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sync@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?imbue@?$basic_streambuf@GU?$char_traits@G@std@@@std@@MAEXABVlocale@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?is@?$ctype@G@std@@QBE_NFG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$ctype@G@std@@UAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_is@?$ctype@G@std@@MBE_NFG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_is@?$ctype@G@std@@MBEPBGPBG0PAF@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_scan_is@?$ctype@G@std@@MBEPBGFPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_scan_not@?$ctype@G@std@@MBEPBGFPBG0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_tolower@?$ctype@G@std@@MBEGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_tolower@?$ctype@G@std@@MBEPBGPAGPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_toupper@?$ctype@G@std@@MBEGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_toupper@?$ctype@G@std@@MBEPBGPAGPBG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_widen@?$ctype@G@std@@MBEGD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_widen@?$ctype@G@std@@MBEPBDPBD0PAG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_narrow@?$ctype@G@std@@MBEDGD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?do_narrow@?$ctype@G@std@@MBEPBGPBG0DPAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$vector@IV?$allocator@I@std@@@std@@QAE@ABV?$allocator@I@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$vector@IV?$allocator@I@std@@@std@@QAE@IABIABV?$allocator@I@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1?$vector@IV?$allocator@I@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?reserve@?$vector@IV?$allocator@I@std@@@std@@QAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?capacity@?$vector@IV?$allocator@I@std@@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?begin@?$vector@IV?$allocator@I@std@@@std@@QAEPAIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?begin@?$vector@IV?$allocator@I@std@@@std@@QBEPBIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?end@?$vector@IV?$allocator@I@std@@@std@@QAEPAIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?resize@?$vector@IV?$allocator@I@std@@@std@@QAEXIABI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?size@?$vector@IV?$allocator@I@std@@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?max_size@?$vector@IV?$allocator@I@std@@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?get_allocator@?$vector@IV?$allocator@I@std@@@std@@QBE?AV?$allocator@I@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??A?$vector@IV?$allocator@I@std@@@std@@QAEAAII@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?erase@?$vector@IV?$allocator@I@std@@@std@@QAEPAIPAI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Eq@?$vector@IV?$allocator@I@std@@@std@@QBE_NABV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Lt@?$vector@IV?$allocator@I@std@@@std@@QBE_NABV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?swap@?$vector@IV?$allocator@I@std@@@std@@QAEXAAV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$reverse_iterator@Viterator@?$vector@_NV?$allocator@I@std@@@std@@_NVreference@23@PAV423@H@std@@QAE@Viterator@?$vector@_NV?$allocator@I@std@@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$reverse_iterator@Vconst_iterator@?$vector@_NV?$allocator@I@std@@@std@@_N$$CBVreference@23@PBV423@H@std@@QAE@Vconst_iterator@?$vector@_NV?$allocator@I@std@@@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$Vector3d@M@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$Vector3d@M@@QAE@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$Vector3d@M@@QAE@MMM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$Vector3d@M@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??H?$Vector3d@M@@QBE?AV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??G?$Vector3d@M@@QBE?AV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??D?$Vector3d@M@@QBE?AV0@M@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??X?$Vector3d@M@@QAEXM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetX@?$Vector3d@M@@QBEMXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetY@?$Vector3d@M@@QBEMXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?GetZ@?$Vector3d@M@@QBEMXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetX@?$Vector3d@M@@QAEXM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetY@?$Vector3d@M@@QAEXM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetZ@?$Vector3d@M@@QAEXM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@?$Vector3d@M@@QAEXMMM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Length2@?$Vector3d@M@@QBEMXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Distance2@?$Vector3d@M@@QBEMABV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Dot@?$Vector3d@M@@QBEMABV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_ostream@DU?$char_traits@D@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_ios@DU?$char_traits@D@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_ostream@GU?$char_traits@G@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_ios@GU?$char_traits@G@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_istream@DU?$char_traits@D@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_streambuf@DU?$char_traits@D@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_istream@GU?$char_traits@G@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$basic_streambuf@GU?$char_traits@G@std@@@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_G?$ctype@G@std@@UAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_D?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_D?$basic_ostream@GU?$char_traits@G@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_D?$basic_istream@DU?$char_traits@D@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_D?$basic_istream@GU?$char_traits@G@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Nullstr@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@CAPBDXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?clear@?$basic_ios@DU?$char_traits@D@std@@@std@@QAEXH_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0sentry@?$basic_ostream@DU?$char_traits@D@std@@@std@@QAE@AAV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1sentry@?$basic_ostream@DU?$char_traits@D@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Bsentry@?$basic_ostream@DU?$char_traits@D@std@@@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$fpos@H@std@@QAE@J@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?clear@?$basic_ios@GU?$char_traits@G@std@@@std@@QAEXH_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0sentry@?$basic_ostream@GU?$char_traits@G@std@@@std@@QAE@AAV12@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??1sentry@?$basic_ostream@GU?$char_traits@G@std@@@std@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??Bsentry@?$basic_ostream@GU?$char_traits@G@std@@@std@@QBE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ipfx@?$basic_istream@DU?$char_traits@D@std@@@std@@QAE_N_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pubsync@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sbumpc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sputc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@QAEHD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?gptr@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IBEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pptr@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IBEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?egptr@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IBEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?gbump@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?epptr@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IBEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Gninc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IAEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pbump@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ipfx@?$basic_istream@GU?$char_traits@G@std@@@std@@QAE_N_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pubsync@?$basic_streambuf@GU?$char_traits@G@std@@@std@@QAEHXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sbumpc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@QAEGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?sputc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@QAEGG@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?gptr@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IBEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pptr@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IBEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?egptr@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IBEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?gbump@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?epptr@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IBEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Gninc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IAEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?pbump@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IAEXH@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?widen@?$ctype@G@std@@QBEGD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?narrow@?$ctype@G@std@@QBEDGD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?allocate@?$allocator@I@std@@QAEPAIIPBX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?deallocate@?$allocator@I@std@@QAEXPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?max_size@?$allocator@I@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$vector@IV?$allocator@I@std@@@std@@QAE@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$vector@IV?$allocator@I@std@@@std@@QAEAAV01@ABV01@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?end@?$vector@IV?$allocator@I@std@@@std@@QBEPBIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?insert@?$vector@IV?$allocator@I@std@@@std@@QAEXPAIIABI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Destroy@?$vector@IV?$allocator@I@std@@@std@@IAEXPAI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Ucopy@?$vector@IV?$allocator@I@std@@@std@@IAEPAIPBI0PAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Ufill@?$vector@IV?$allocator@I@std@@@std@@IAEXPAIIABI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?capacity@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Refcnt@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEAAEPBD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?deallocate@?$allocator@D@std@@QAEXPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?opfx@?$basic_ostream@DU?$char_traits@D@std@@@std@@QAE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?osfx@?$basic_ostream@DU?$char_traits@D@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?tie@?$basic_ios@DU?$char_traits@D@std@@@std@@QBEPAV?$basic_ostream@DU?$char_traits@D@std@@@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?opfx@?$basic_ostream@GU?$char_traits@G@std@@@std@@QAE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?osfx@?$basic_ostream@GU?$char_traits@G@std@@@std@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?tie@?$basic_ios@GU?$char_traits@G@std@@@std@@QBEPAV?$basic_ostream@GU?$char_traits@G@std@@@2@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Pninc@?$basic_streambuf@DU?$char_traits@D@std@@@std@@IAEPADXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Pninc@?$basic_streambuf@GU?$char_traits@G@std@@@std@@IAEPAGXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?construct@?$allocator@I@std@@QAEXPAIABI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?destroy@?$allocator@I@std@@QAEXPAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Split@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?allocate@?$allocator@D@std@@QAEPADIPBX@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?max_size@?$allocator@D@std@@QBEIXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?use_facet@std@@YAABV?$ctype@D@1@ABVlocale@1@PBV21@_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?use_facet@std@@YAABV?$ctype@G@1@ABVlocale@1@PBV21@_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy_backward@std@@YA?AViterator@?$vector@_NV?$allocator@I@std@@@1@V231@00@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?fill@std@@YAXViterator@?$vector@_NV?$allocator@I@std@@@1@0AB_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Distance@std@@YAXVconst_iterator@?$vector@_NV?$allocator@I@std@@@1@0AAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy@std@@YA?AViterator@?$vector@_NV?$allocator@I@std@@@1@Vconst_iterator@31@0V231@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy@std@@YA?AViterator@?$vector@_NV?$allocator@I@std@@@1@V231@00@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?swap@std@@YAXAAI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy@std@@YAPAIPAI00@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?equal@std@@YA_NPBI00@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?lexicographical_compare@std@@YA_NPBI000@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8std@@YA_NABV?$allocator@I@0@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?swap@std@@YAXAAPAI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??8std@@YA_NABV?$allocator@D@0@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Allocate@std@@YAPAIHPAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy@std@@YAPAIPBI0PAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?fill@std@@YAXPAI0ABI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?copy_backward@std@@YAPAIPAI00@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Construct@std@@YAXPAIABI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Destroy@std@@YAXPAI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Allocate@std@@YAPADHPAD@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$ctype@G@std@@QAE@I@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Save@?$_Tidyfac@V?$ctype@D@std@@@std@@SAPAV?$ctype@D@2@PAV32@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Save@?$_Tidyfac@V?$ctype@G@std@@@std@@SAPAV?$ctype@G@2@PAV32@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Init@?$ctype@G@std@@IAEXABV_Locinfo@2@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Tidy@?$_Tidyfac@V?$ctype@D@std@@@std@@SAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Tidy@?$_Tidyfac@V?$ctype@G@std@@@std@@SAXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?id@?$ctype@G@std@@$D
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Iter_cat@std@@YA?AUrandom_access_iterator_tag@1@ABU?$iterator@Urandom_access_iterator_tag@std@@_NH@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?_Distance@std@@YAXVconst_iterator@?$vector@_NV?$allocator@I@std@@@1@0AAIUrandom_access_iterator_tag@1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?mismatch@std@@YA?AU?$pair@PBIPBI@1@PBI00@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?id@?$ctype@G@std@@$E
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$pair@PBIPBI@std@@QAE@ABQBI0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_R0?AVruntime_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA2?AVlogic_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVbad_typeid@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVbad_cast@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA3?AVrange_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVout_of_range@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVfailure@ios_base@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVoverflow_error@std@@@8??0overflow_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA2?AVruntime_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVunderflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA3?AVoverflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVrange_error@std@@@8??0range_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVbad_alloc@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CT??_R0?AVlength_error@std@@@8??0length_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVlength_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ?_Psave@?1??use_facet@std@@YAABV?$ctype@G@2@ABVlocale@2@PBV32@_N@Z@4PBVfacet@42@B
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_R0?AVlength_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVdomain_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVfailure@ios_base@std@@@8??0failure@ios_base@std@@QAE@ABV012@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA2?AVbad_exception@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVruntime_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVout_of_range@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVbad_typeid@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVunderflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVrange_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA2?AVbad_cast@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVoverflow_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA2?AVbad_alloc@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVbad_cast@std@@@8??0bad_cast@std@@QAE@ABV01@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_7?$basic_ios@DU?$char_traits@D@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$codecvt@GDH@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7__non_rtti_object@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_8?$basic_istream@DU?$char_traits@D@std@@@std@@7B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_ios@GU?$char_traits@G@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_istream@DU?$char_traits@D@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7bad_cast@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_istream@GU?$char_traits@G@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7facet@locale@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_8?$basic_istream@GU?$char_traits@G@std@@@std@@7B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7range_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7runtime_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7underflow_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$ctype@D@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_8?$basic_ostream@DU?$char_traits@D@std@@@std@@7B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_streambuf@GU?$char_traits@G@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7ios_base@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7domain_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7logic_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7failure@ios_base@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_ostream@GU?$char_traits@G@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7overflow_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7bad_typeid@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ?npos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@2IB
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7length_error@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_streambuf@DU?$char_traits@D@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7ctype_base@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7out_of_range@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$ctype@G@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7invalid_argument@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ?_Facsav@?$_Tidyfac@V?$ctype@D@std@@@std@@0PAV?$ctype@D@2@A
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ?_Facsav@?$_Tidyfac@V?$ctype@G@std@@@std@@0PAV?$ctype@G@2@A
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_8?$basic_ostream@GU?$char_traits@G@std@@@std@@7B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7bad_exception@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7bad_alloc@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7?$basic_ostream@DU?$char_traits@D@std@@@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT ??_7codecvt_base@std@@6B@
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __TI3?AVfailure@ios_base@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVinvalid_argument@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVbad_alloc@std@@@8??0bad_alloc@std@@QAE@ABV01@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVlogic_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_R0?AVexception@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CT??_R0?AVbad_typeid@std@@@8??0bad_typeid@std@@QAE@ABV01@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVout_of_range@std@@@8??0out_of_range@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVdomain_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVbad_cast@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVinvalid_argument@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ?_Psave@?1??use_facet@std@@YAABV?$ctype@D@2@ABVlocale@2@PBV32@_N@Z@4PBVfacet@42@B
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CT??_R0?AVexception@@@8??0exception@@QAE@ABV0@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVunderflow_error@std@@@8??0underflow_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVdomain_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVlength_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVunderflow_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CT??_R0?AVinvalid_argument@std@@@8??0invalid_argument@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVbad_exception@std@@@8??0bad_exception@std@@QAE@ABV01@@Z12
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVfailure@ios_base@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __TI2?AVlogic_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI3?AVoverflow_error@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVdomain_error@std@@@8??0domain_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CT??_R0?AVruntime_error@std@@@8??0runtime_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __CTA2?AVbad_typeid@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVinvalid_argument@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT ??_R0?AVrange_error@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __TI2?AVbad_exception@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ?_C@?1??_Nullstr@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@CAPBDXZ@4DB
CONST	SEGMENT DWORD USE32 PUBLIC 'CONST'
CONST	ENDS
;	COMDAT __CT??_R0?AVlogic_error@std@@@8??0logic_error@std@@QAE@ABV01@@Z28
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT __TI2?AVbad_alloc@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
;	COMDAT ??_R0?AVbad_exception@std@@@8
_DATA	SEGMENT DWORD USE32 PUBLIC 'DATA'
_DATA	ENDS
;	COMDAT __CTA3?AVout_of_range@std@@
xdata$x	SEGMENT DWORD USE32 PUBLIC 'CONST'
xdata$x	ENDS
FLAT	GROUP _DATA, CONST, _BSS, CRT$XCA, CRT$XCU, CRT$XCL, CRT$XCC, CRT$XCZ, xdata$x
	ASSUME	CS: FLAT, DS: FLAT, SS: FLAT
endif
CRT$XCU	SEGMENT
_$S18	DD	FLAT:_$E17
_$S24	DD	FLAT:_$E23
_$S31	DD	FLAT:_$E30
CRT$XCU	ENDS
PUBLIC	?Set@Sphere@@QAEHABV?$Vector3d@M@@M@Z		; Sphere::Set
EXTRN	__fltused:NEAR
_TEXT	SEGMENT
_center$ = 8
_radius$ = 12
?Set@Sphere@@QAEHABV?$Vector3d@M@@M@Z PROC NEAR		; Sphere::Set

; 21   : {

	push	ebp
	mov	ebp, esp

; 22   : 	mCenter = center;

	mov	eax, DWORD PTR _center$[ebp]

; 23   : 	mRadius = radius;
; 24   : 	mRadius2 = radius*radius;

	fld	DWORD PTR _radius$[ebp]
	mov	edx, DWORD PTR [eax]
	fmul	DWORD PTR _radius$[ebp]
	mov	DWORD PTR [ecx], edx
	mov	edx, DWORD PTR [eax+4]
	mov	DWORD PTR [ecx+4], edx
	mov	eax, DWORD PTR [eax+8]
	mov	edx, DWORD PTR _radius$[ebp]
	mov	DWORD PTR [ecx+8], eax
	fstp	DWORD PTR [ecx+16]
	mov	DWORD PTR [ecx+12], edx

; 25   : }

	pop	ebp
	ret	8
?Set@Sphere@@QAEHABV?$Vector3d@M@@M@Z ENDP		; Sphere::Set
_TEXT	ENDS
PUBLIC	__real@4@bfff8000000000000000
PUBLIC	__real@4@00000000000000000000
PUBLIC	?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z ; Sphere::RayIntersection
PUBLIC	??0?$Vector3d@M@@QAE@ABV0@@Z			; Vector3d<float>::Vector3d<float>
;	COMDAT __real@4@bfff8000000000000000
; File C:\WINDOWS\Desktop\Gems2 CD\SourceCode\04 Geometry Management\03 Ratcliff\vector.h
CONST	SEGMENT
__real@4@bfff8000000000000000 DD 0bf800000r	; -1
CONST	ENDS
;	COMDAT __real@4@00000000000000000000
CONST	SEGMENT
__real@4@00000000000000000000 DD 000000000r	; 0
CONST	ENDS
_TEXT	SEGMENT
$T13831 = -12
_Diff$13842 = -24
_Scaled$13882 = -12
_Sum$13900 = -24
_rayOrigin$ = 8
_dir$ = 12
_intersect$ = 16
_V$ = -24
_v$ = 8
_disc$ = 12
?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z PROC NEAR ; Sphere::RayIntersection

; 35   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 24					; 00000018H
	push	esi
	mov	esi, ecx
	push	edi

; 36   : 	//notation:
; 37   : 	//point E  = rayOrigin
; 38   : 	//point O  = sphere center
; 39   : 
; 40   : 	Vector3d<float> EO = mCenter - rayOrigin;

	mov	edi, DWORD PTR _rayOrigin$[ebp]
	fld	DWORD PTR [esi]
	fsub	DWORD PTR [edi]
	lea	eax, DWORD PTR _Diff$13842[ebp]
	lea	ecx, DWORD PTR $T13831[ebp]
	push	eax
	fstp	DWORD PTR _Diff$13842[ebp]
	fld	DWORD PTR [esi+4]
	fsub	DWORD PTR [edi+4]
	fstp	DWORD PTR _Diff$13842[ebp+4]
	fld	DWORD PTR [esi+8]
	fsub	DWORD PTR [edi+8]
	fstp	DWORD PTR _Diff$13842[ebp+8]
	call	??0?$Vector3d@M@@QAE@ABV0@@Z		; Vector3d<float>::Vector3d<float>

; 41   :   Vector3d<float> V = dir;

	mov	eax, DWORD PTR _dir$[ebp]
	fld	DWORD PTR [eax]
	fld	DWORD PTR [eax+8]

; 42   :   float dist2 = EO.x*EO.x + EO.y*EO.y + EO.z * EO.z;

	fld	DWORD PTR $T13831[ebp+8]
	fmul	DWORD PTR $T13831[ebp+8]
	fld	DWORD PTR $T13831[ebp+4]
	fmul	DWORD PTR $T13831[ebp+4]
	mov	ecx, DWORD PTR [eax+4]
	mov	DWORD PTR _V$[ebp+4], ecx

; 66   : 	}
; 67   : 	return false;

	faddp	ST(1), ST(0)
	fld	DWORD PTR $T13831[ebp]
	fmul	DWORD PTR $T13831[ebp]
	faddp	ST(1), ST(0)
	fcom	DWORD PTR [esi+16]
	fnstsw	ax
	test	ah, 1
	je	SHORT $L13853
	fxch	ST(2)
	fmul	DWORD PTR __real@4@bfff8000000000000000
	fxch	ST(2)
	fld	DWORD PTR _V$[ebp+4]
	fmul	DWORD PTR __real@4@bfff8000000000000000
	fstp	DWORD PTR _V$[ebp+4]
	fxch	ST(1)
	fmul	DWORD PTR __real@4@bfff8000000000000000
	fxch	ST(1)
$L13853:
	fld	ST(1)
	fmul	DWORD PTR $T13831[ebp+8]
	fld	DWORD PTR _V$[ebp+4]
	fmul	DWORD PTR $T13831[ebp+4]
	faddp	ST(1), ST(0)
	fld	ST(3)
	fmul	DWORD PTR $T13831[ebp]
	faddp	ST(1), ST(0)
	fst	DWORD PTR _v$[ebp]
	fmul	DWORD PTR _v$[ebp]
	fsubr	ST(0), ST(1)
	fsubr	DWORD PTR [esi+16]
	fstp	DWORD PTR _disc$[ebp]
	fstp	ST(0)
	fld	DWORD PTR _disc$[ebp]
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L12166
	mov	eax, DWORD PTR _intersect$[ebp]
	test	eax, eax
	je	SHORT $L13913
	fld	DWORD PTR _disc$[ebp]
	fsqrt
	fsubr	DWORD PTR _v$[ebp]
	fxch	ST(2)
	fmul	ST(0), ST(2)
	fstp	DWORD PTR _Scaled$13882[ebp]
	fld	DWORD PTR _V$[ebp+4]
	fmul	ST(0), ST(2)
	fstp	DWORD PTR _Scaled$13882[ebp+4]
	fmul	ST(0), ST(1)
	fxch	ST(1)
	fstp	ST(0)
	fld	DWORD PTR _Scaled$13882[ebp]
	fadd	DWORD PTR [edi]
	fld	DWORD PTR _Scaled$13882[ebp+4]
	fadd	DWORD PTR [edi+4]
	fstp	DWORD PTR _Sum$13900[ebp+4]
	fxch	ST(1)
	fadd	DWORD PTR [edi+8]
	mov	edx, DWORD PTR _Sum$13900[ebp+4]
	pop	edi
	mov	DWORD PTR [eax+4], edx
	pop	esi
	fstp	DWORD PTR _Sum$13900[ebp+8]
	mov	ecx, DWORD PTR _Sum$13900[ebp+8]
	fstp	DWORD PTR [eax]
	mov	DWORD PTR [eax+8], ecx
	mov	al, 1

; 68   : }

	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
$L13913:

; 43   :   // Bug Fix For Gem, if origin is *inside* the sphere, invert the
; 44   :   // direction vector so that we get a valid intersection location.
; 45   :   if ( dist2 < mRadius2 ) V*=-1;
; 46   : 
; 47   : 	float v = EO.Dot(V);
; 48   : 
; 49   : 	float disc = mRadius2 - (EO.Length2() - v*v);
; 50   : 
; 51   : 	if (disc > 0.0f)

	fstp	ST(0)
	pop	edi

; 52   : 	{
; 53   : 
; 54   : 		if ( intersect )
; 55   : 		{
; 56   : 
; 57   : 		  float d = sqrtf(disc);
; 58   : 
; 59   :       float dist2 = rayOrigin.Distance2(mCenter);
; 60   : 
; 61   :       *intersect = rayOrigin + V*(v-d);
; 62   : 
; 63   :     }
; 64   : 
; 65   : 		return true;

	mov	al, 1
	fstp	ST(0)
	pop	esi

; 68   : }

	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
$L12166:

; 43   :   // Bug Fix For Gem, if origin is *inside* the sphere, invert the
; 44   :   // direction vector so that we get a valid intersection location.
; 45   :   if ( dist2 < mRadius2 ) V*=-1;
; 46   : 
; 47   : 	float v = EO.Dot(V);
; 48   : 
; 49   : 	float disc = mRadius2 - (EO.Length2() - v*v);
; 50   : 
; 51   : 	if (disc > 0.0f)

	fstp	ST(0)
	pop	edi

; 66   : 	}
; 67   : 	return false;

	xor	al, al
	fstp	ST(0)
	pop	esi

; 68   : }

	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z ENDP ; Sphere::RayIntersection
_TEXT	ENDS
PUBLIC	?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0MPAV2@@Z ; Sphere::RayIntersection
PUBLIC	??4?$Vector3d@M@@QAEAAV0@ABV0@@Z		; Vector3d<float>::operator=
_TEXT	SEGMENT
$T13942 = -48
_Diff$13946 = -36
_sect$13955 = -24
_rayOrigin$ = 8
_V$ = 12
_distance$ = 16
_intersect$ = 20
_sect$ = -12
?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0MPAV2@@Z PROC NEAR ; Sphere::RayIntersection

; 75   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 48					; 00000030H
	push	esi

; 76   :   Vector3d<float> sect;
; 77   :   bool hit = RayIntersectionInFront(rayOrigin,V,&sect);

	mov	esi, DWORD PTR _rayOrigin$[ebp]
	push	edi
	mov	edi, DWORD PTR _V$[ebp]
	lea	eax, DWORD PTR _sect$13955[ebp]
	push	eax
	push	edi
	push	esi
	call	?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z ; Sphere::RayIntersection
	test	al, al
	je	$L12181
	fld	DWORD PTR _sect$13955[ebp]
	fsub	DWORD PTR [esi]
	lea	ecx, DWORD PTR _Diff$13946[ebp]
	push	ecx
	lea	ecx, DWORD PTR $T13942[ebp]
	fstp	DWORD PTR _Diff$13946[ebp]
	fld	DWORD PTR _sect$13955[ebp+4]
	fsub	DWORD PTR [esi+4]
	fstp	DWORD PTR _Diff$13946[ebp+4]
	fld	DWORD PTR _sect$13955[ebp+8]
	fsub	DWORD PTR [esi+8]
	fstp	DWORD PTR _Diff$13946[ebp+8]
	call	??0?$Vector3d@M@@QAE@ABV0@@Z		; Vector3d<float>::Vector3d<float>
	fld	DWORD PTR $T13942[ebp+8]
	fmul	DWORD PTR [edi+8]
	fld	DWORD PTR $T13942[ebp+4]
	fmul	DWORD PTR [edi+4]

; 78   : 
; 79   :   if ( hit )
; 80   :   {
; 81   :     float d = rayOrigin.Distance2(sect);
; 82   :     if ( d > (distance*distance) ) return false;
; 83   :     if ( intersect ) *intersect = sect;
; 84   :     return true;
; 85   :   }
; 86   :   return false;

	faddp	ST(1), ST(0)
	fld	DWORD PTR $T13942[ebp]
	fmul	DWORD PTR [edi]
	faddp	ST(1), ST(0)
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 1
	jne	SHORT $L12181
	lea	edx, DWORD PTR _sect$[ebp]
	test	edx, edx
	je	SHORT $L13971
	lea	eax, DWORD PTR _sect$13955[ebp]
	lea	ecx, DWORD PTR _sect$[ebp]
	push	eax
	call	??4?$Vector3d@M@@QAEAAV0@ABV0@@Z	; Vector3d<float>::operator=
$L13971:
	fld	DWORD PTR _sect$[ebp]
	fsub	DWORD PTR [esi]
	fld	DWORD PTR _sect$[ebp+4]
	fsub	DWORD PTR [esi+4]
	fld	DWORD PTR _sect$[ebp+8]
	fsub	DWORD PTR [esi+8]
	fld	ST(0)
	fmul	ST(0), ST(1)
	fld	ST(3)
	fmul	ST(0), ST(4)
	faddp	ST(1), ST(0)
	fld	ST(2)
	fmul	ST(0), ST(3)
	faddp	ST(1), ST(0)
	fld	DWORD PTR _distance$[ebp]
	fmul	DWORD PTR _distance$[ebp]
	fcompp
	fstp	ST(0)
	fnstsw	ax
	fstp	ST(0)
	test	ah, 1
	fstp	ST(0)
	jne	SHORT $L12181
	mov	eax, DWORD PTR _intersect$[ebp]
	test	eax, eax
	je	SHORT $L13969
	mov	ecx, DWORD PTR _sect$[ebp]
	mov	edx, DWORD PTR _sect$[ebp+4]
	mov	DWORD PTR [eax], ecx
	mov	ecx, DWORD PTR _sect$[ebp+8]
	mov	DWORD PTR [eax+4], edx
	mov	DWORD PTR [eax+8], ecx
$L13969:
	pop	edi
	mov	al, 1
	pop	esi

; 87   : }

	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
$L12181:
	pop	edi

; 78   : 
; 79   :   if ( hit )
; 80   :   {
; 81   :     float d = rayOrigin.Distance2(sect);
; 82   :     if ( d > (distance*distance) ) return false;
; 83   :     if ( intersect ) *intersect = sect;
; 84   :     return true;
; 85   :   }
; 86   :   return false;

	xor	al, al
	pop	esi

; 87   : }

	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0MPAV2@@Z ENDP ; Sphere::RayIntersection
_TEXT	ENDS
PUBLIC	?RayIntersectionInFront@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z ; Sphere::RayIntersectionInFront
_TEXT	SEGMENT
_rayOrigin$ = 8
_V$ = 12
_intersect$ = 16
_sect$ = -12
?RayIntersectionInFront@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z PROC NEAR ; Sphere::RayIntersectionInFront

; 93   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 12					; 0000000cH
	push	esi

; 94   :   Vector3d<float> sect;
; 95   :   bool hit = RayIntersection(rayOrigin,V,&sect);

	mov	esi, DWORD PTR _V$[ebp]
	lea	eax, DWORD PTR _sect$[ebp]
	push	edi
	mov	edi, DWORD PTR _rayOrigin$[ebp]
	push	eax
	push	esi
	push	edi
	call	?RayIntersection@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z ; Sphere::RayIntersection

; 96   : 
; 97   :   if ( hit )

	test	al, al
	je	SHORT $L12197

; 98   :   {
; 99   : 
; 100  :     Vector3d<float> dir = sect - rayOrigin;

	fld	DWORD PTR _sect$[ebp]
	fsub	DWORD PTR [edi]
	fld	DWORD PTR _sect$[ebp+4]
	fsub	DWORD PTR [edi+4]
	fld	DWORD PTR _sect$[ebp+8]
	fsub	DWORD PTR [edi+8]

; 101  : 
; 102  :     float dot = dir.Dot(V);

	fmul	DWORD PTR [esi+8]
	fxch	ST(1)
	fmul	DWORD PTR [esi+4]

; 103  : 
; 104  :     if ( dot >= 0  ) // then it's in front!
; 105  :     {
; 106  :       if ( intersect ) *intersect = sect;
; 107  :       return true;
; 108  :     }
; 109  :   }
; 110  :   return false;

	faddp	ST(1), ST(0)
	fxch	ST(1)
	fmul	DWORD PTR [esi]
	faddp	ST(1), ST(0)
	fcomp	DWORD PTR __real@4@00000000000000000000
	fnstsw	ax
	test	ah, 1
	jne	SHORT $L12197
	mov	eax, DWORD PTR _intersect$[ebp]
	test	eax, eax
	je	SHORT $L14001
	mov	ecx, DWORD PTR _sect$[ebp]
	mov	edx, DWORD PTR _sect$[ebp+4]
	mov	DWORD PTR [eax], ecx
	mov	ecx, DWORD PTR _sect$[ebp+8]
	mov	DWORD PTR [eax+4], edx
	mov	DWORD PTR [eax+8], ecx
$L14001:
	pop	edi
	mov	al, 1
	pop	esi

; 111  : }

	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
$L12197:
	pop	edi

; 103  : 
; 104  :     if ( dot >= 0  ) // then it's in front!
; 105  :     {
; 106  :       if ( intersect ) *intersect = sect;
; 107  :       return true;
; 108  :     }
; 109  :   }
; 110  :   return false;

	xor	al, al
	pop	esi

; 111  : }

	mov	esp, ebp
	pop	ebp
	ret	12					; 0000000cH
?RayIntersectionInFront@Sphere@@QAE_NABV?$Vector3d@M@@0PAV2@@Z ENDP ; Sphere::RayIntersectionInFront
_TEXT	ENDS
PUBLIC	?Report@Sphere@@QAEXXZ				; Sphere::Report
_TEXT	SEGMENT
?Report@Sphere@@QAEXXZ PROC NEAR			; Sphere::Report

; 115  : }

	ret	0
?Report@Sphere@@QAEXXZ ENDP				; Sphere::Report
_TEXT	ENDS
PUBLIC	?Compute@Sphere@@QAEXABVSphereInterface@@@Z	; Sphere::Compute
PUBLIC	__real@4@3ffe8000000000000000
PUBLIC	__real@4@3fff8000000000000000
;	COMDAT __real@4@3ffe8000000000000000
; File C:\WINDOWS\Desktop\Gems2 CD\SourceCode\04 Geometry Management\03 Ratcliff\vector.h
CONST	SEGMENT
__real@4@3ffe8000000000000000 DD 03f000000r	; 0.5
CONST	ENDS
;	COMDAT __real@4@3fff8000000000000000
CONST	SEGMENT
__real@4@3fff8000000000000000 DD 03f800000r	; 1
CONST	ENDS
_TEXT	SEGMENT
_source$ = 8
_xmin$ = -40
_xmax$ = -52
_ymin$ = -64
_ymax$ = -76
_zmin$ = -88
_zmax$ = -100
_dia1$ = -112
_dia2$ = -28
_count$ = -8
_caller_p$12220 = -28
_xspan$ = 8
_yspan$ = -4
_zspan$ = -16
_maxspan$ = -12
_caller_p$12239 = -112
_old_to_new$12243 = -4
?Compute@Sphere@@QAEXABVSphereInterface@@@Z PROC NEAR	; Sphere::Compute

; 133  : {

	push	ebp
	mov	ebp, esp
	sub	esp, 112				; 00000070H
	push	ebx
	push	esi
	push	edi

; 134  : 
; 135  :   Vector3d<float> xmin,xmax,ymin,ymax,zmin,zmax,dia1,dia2;
; 136  : 
; 137  :   /* FIRST PASS: find 6 minima/maxima points */
; 138  :   xmin.Set(BIGNUMBER,BIGNUMBER,BIGNUMBER);
; 139  :   xmax.Set(-BIGNUMBER,-BIGNUMBER,-BIGNUMBER);
; 140  :   ymin.Set(BIGNUMBER,BIGNUMBER,BIGNUMBER);
; 141  :   ymax.Set(-BIGNUMBER,-BIGNUMBER,-BIGNUMBER);
; 142  :   zmin.Set(BIGNUMBER,BIGNUMBER,BIGNUMBER);
; 143  :   zmax.Set(-BIGNUMBER,-BIGNUMBER,-BIGNUMBER);
; 144  : 
; 145  :   int count = source.GetVertexCount();

	mov	edi, DWORD PTR _source$[ebp]
	mov	esi, ecx
	mov	DWORD PTR _xmin$[ebp], 1287568416	; 4cbebc20H
	mov	eax, DWORD PTR [edi]
	mov	ecx, edi
	mov	DWORD PTR _xmin$[ebp+4], 1287568416	; 4cbebc20H
	mov	DWORD PTR _xmin$[ebp+8], 1287568416	; 4cbebc20H
	mov	DWORD PTR _xmax$[ebp], -859915232	; ccbebc20H
	mov	DWORD PTR _xmax$[ebp+4], -859915232	; ccbebc20H
	mov	DWORD PTR _xmax$[ebp+8], -859915232	; ccbebc20H
	mov	DWORD PTR _ymin$[ebp], 1287568416	; 4cbebc20H
	mov	DWORD PTR _ymin$[ebp+4], 1287568416	; 4cbebc20H
	mov	DWORD PTR _ymin$[ebp+8], 1287568416	; 4cbebc20H
	mov	DWORD PTR _ymax$[ebp], -859915232	; ccbebc20H
	mov	DWORD PTR _ymax$[ebp+4], -859915232	; ccbebc20H
	mov	DWORD PTR _ymax$[ebp+8], -859915232	; ccbebc20H
	mov	DWORD PTR _zmin$[ebp], 1287568416	; 4cbebc20H
	mov	DWORD PTR _zmin$[ebp+4], 1287568416	; 4cbebc20H
	mov	DWORD PTR _zmin$[ebp+8], 1287568416	; 4cbebc20H
	mov	DWORD PTR _zmax$[ebp], -859915232	; ccbebc20H
	mov	DWORD PTR _zmax$[ebp+4], -859915232	; ccbebc20H
	mov	DWORD PTR _zmax$[ebp+8], -859915232	; ccbebc20H
	call	DWORD PTR [eax]

; 146  : 
; 147  :   for (int i=0; i<count; i++)

	xor	ebx, ebx
	mov	DWORD PTR _count$[ebp], eax
	test	eax, eax
	jle	$L12219
$L12217:

; 148  : 	{
; 149  :     Vector3d<float> caller_p;
; 150  :     source.GetVertex(i,caller_p);

	mov	edx, DWORD PTR [edi]
	lea	eax, DWORD PTR _caller_p$12220[ebp]
	push	eax
	push	ebx
	mov	ecx, edi
	call	DWORD PTR [edx+4]

; 151  : 
; 152  :   	if (caller_p.GetX()<xmin.GetX()) 	xmin = caller_p; /* New xminimum point */

	fld	DWORD PTR _caller_p$12220[ebp]
	fcomp	DWORD PTR _xmin$[ebp]
	fnstsw	ax
	test	ah, 1
	je	SHORT $L14057
	mov	ecx, DWORD PTR _caller_p$12220[ebp]
	mov	edx, DWORD PTR _caller_p$12220[ebp+4]
	mov	eax, DWORD PTR _caller_p$12220[ebp+8]
	mov	DWORD PTR _xmin$[ebp], ecx
	mov	DWORD PTR _xmin$[ebp+4], edx
	mov	DWORD PTR _xmin$[ebp+8], eax
$L14057:

; 153  :   	if (caller_p.GetX()>xmax.GetX())	xmax = caller_p;

	fld	DWORD PTR _caller_p$12220[ebp]
	fcomp	DWORD PTR _xmax$[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L14066
	mov	ecx, DWORD PTR _caller_p$12220[ebp]
	mov	edx, DWORD PTR _caller_p$12220[ebp+4]
	mov	eax, DWORD PTR _caller_p$12220[ebp+8]
	mov	DWORD PTR _xmax$[ebp], ecx
	mov	DWORD PTR _xmax$[ebp+4], edx
	mov	DWORD PTR _xmax$[ebp+8], eax
$L14066:

; 154  :   	if (caller_p.GetY()<ymin.GetY())	ymin = caller_p;

	fld	DWORD PTR _caller_p$12220[ebp+4]
	fld	DWORD PTR _ymin$[ebp+4]
	fcompp
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L14077
	mov	ecx, DWORD PTR _caller_p$12220[ebp]
	mov	edx, DWORD PTR _caller_p$12220[ebp+4]
	mov	eax, DWORD PTR _caller_p$12220[ebp+8]
	mov	DWORD PTR _ymin$[ebp], ecx
	mov	DWORD PTR _ymin$[ebp+4], edx
	mov	DWORD PTR _ymin$[ebp+8], eax
$L14077:

; 155  :   	if (caller_p.GetY()>ymax.GetY())	ymax = caller_p;

	fld	DWORD PTR _caller_p$12220[ebp+4]
	fld	DWORD PTR _ymax$[ebp+4]
	fcompp
	fnstsw	ax
	test	ah, 1
	je	SHORT $L14088
	mov	ecx, DWORD PTR _caller_p$12220[ebp]
	mov	edx, DWORD PTR _caller_p$12220[ebp+4]
	mov	eax, DWORD PTR _caller_p$12220[ebp+8]
	mov	DWORD PTR _ymax$[ebp], ecx
	mov	DWORD PTR _ymax$[ebp+4], edx
	mov	DWORD PTR _ymax$[ebp+8], eax
$L14088:

; 156  :   	if (caller_p.GetZ()<zmin.GetZ())	zmin = caller_p;

	fld	DWORD PTR _caller_p$12220[ebp+8]
	fld	DWORD PTR _zmin$[ebp+8]
	fcompp
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L14099
	mov	ecx, DWORD PTR _caller_p$12220[ebp]
	mov	edx, DWORD PTR _caller_p$12220[ebp+4]
	mov	eax, DWORD PTR _caller_p$12220[ebp+8]
	mov	DWORD PTR _zmin$[ebp], ecx
	mov	DWORD PTR _zmin$[ebp+4], edx
	mov	DWORD PTR _zmin$[ebp+8], eax
$L14099:

; 157  :   	if (caller_p.GetZ()>zmax.GetZ()) zmax = caller_p;

	fld	DWORD PTR _caller_p$12220[ebp+8]
	fld	DWORD PTR _zmax$[ebp+8]
	fcompp
	fnstsw	ax
	test	ah, 1
	je	SHORT $L12218
	mov	ecx, DWORD PTR _caller_p$12220[ebp]
	mov	edx, DWORD PTR _caller_p$12220[ebp+4]
	mov	eax, DWORD PTR _caller_p$12220[ebp+8]
	mov	DWORD PTR _zmax$[ebp], ecx
	mov	DWORD PTR _zmax$[ebp+4], edx
	mov	DWORD PTR _zmax$[ebp+8], eax
$L12218:
	mov	eax, DWORD PTR _count$[ebp]
	inc	ebx
	cmp	ebx, eax
	jl	$L12217
$L12219:

; 158  : 	}
; 159  : 
; 160  :   /* Set xspan = distance between the 2 points xmin & xmax (squared) */
; 161  :   float dx = xmax.GetX() - xmin.GetX();

	fld	DWORD PTR _xmax$[ebp]
	fsub	DWORD PTR _xmin$[ebp]

; 162  :   float dy = xmax.GetY() - xmin.GetY();

	fld	DWORD PTR _xmax$[ebp+4]
	fsub	DWORD PTR _xmin$[ebp+4]

; 163  :   float dz = xmax.GetZ() - xmin.GetZ();

	fld	DWORD PTR _xmax$[ebp+8]
	fsub	DWORD PTR _xmin$[ebp+8]

; 164  :   float xspan = dx*dx + dy*dy + dz*dz;
; 165  : 
; 166  :   /* Same for y & z spans */
; 167  :   dx = ymax.GetX() - ymin.GetX();
; 168  :   dy = ymax.GetY() - ymin.GetY();
; 169  :   dz = ymax.GetZ() - ymin.GetZ();
; 170  :   float yspan = dx*dx + dy*dy + dz*dz;
; 171  : 
; 172  :   dx = zmax.GetX() - zmin.GetX();
; 173  :   dy = zmax.GetY() - zmin.GetY();
; 174  :   dz = zmax.GetZ() - zmin.GetZ();
; 175  :   float zspan = dx*dx + dy*dy + dz*dz;
; 176  : 
; 177  :   /* Set points dia1 & dia2 to the maximally separated pair */
; 178  :   dia1 = xmin;

	mov	ecx, DWORD PTR _xmin$[ebp+4]
	mov	edx, DWORD PTR _xmin$[ebp+8]

; 179  :   dia2 = xmax; /* assume xspan biggest */

	mov	eax, DWORD PTR _xmax$[ebp]
	mov	DWORD PTR _dia1$[ebp+4], ecx
	fld	ST(0)
	fmul	ST(0), ST(1)
	fld	ST(2)
	fmul	ST(0), ST(3)
	mov	ecx, DWORD PTR _xmax$[ebp+4]
	mov	DWORD PTR _dia1$[ebp+8], edx
	mov	edx, DWORD PTR _xmax$[ebp+8]
	mov	DWORD PTR _dia2$[ebp], eax
	faddp	ST(1), ST(0)
	fld	ST(3)
	fmul	ST(0), ST(4)
	faddp	ST(1), ST(0)
	mov	DWORD PTR _dia2$[ebp+4], ecx
	mov	DWORD PTR _dia2$[ebp+8], edx
	fstp	DWORD PTR _xspan$[ebp]

; 180  :   float maxspan = xspan;

	mov	eax, DWORD PTR _xspan$[ebp]
	fstp	ST(0)
	fstp	ST(0)
	fstp	ST(0)
	fld	DWORD PTR _ymax$[ebp]
	fsub	DWORD PTR _ymin$[ebp]
	fld	DWORD PTR _ymax$[ebp+4]
	fsub	DWORD PTR _ymin$[ebp+4]
	fld	DWORD PTR _ymax$[ebp+8]
	fsub	DWORD PTR _ymin$[ebp+8]
	fld	ST(0)
	fmul	ST(0), ST(1)
	fld	ST(2)
	fmul	ST(0), ST(3)
	mov	DWORD PTR _maxspan$[ebp], eax
	faddp	ST(1), ST(0)
	fld	ST(3)
	fmul	ST(0), ST(4)
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _yspan$[ebp]
	fstp	ST(0)
	fstp	ST(0)
	fstp	ST(0)
	fld	DWORD PTR _zmax$[ebp]
	fsub	DWORD PTR _zmin$[ebp]
	fld	DWORD PTR _zmax$[ebp+4]
	fsub	DWORD PTR _zmin$[ebp+4]
	fld	DWORD PTR _zmax$[ebp+8]
	fsub	DWORD PTR _zmin$[ebp+8]
	fld	ST(0)
	fmul	ST(0), ST(1)
	fld	ST(2)
	fmul	ST(0), ST(3)
	faddp	ST(1), ST(0)
	fld	ST(3)
	fmul	ST(0), ST(4)
	faddp	ST(1), ST(0)
	fstp	DWORD PTR _zspan$[ebp]
	fstp	ST(0)
	fstp	ST(0)
	fstp	ST(0)
	fld	DWORD PTR _xmin$[ebp]

; 181  : 
; 182  :   if (yspan>maxspan)

	fld	DWORD PTR _yspan$[ebp]
	fcomp	DWORD PTR _xspan$[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L14188

; 183  : 	{
; 184  : 	  maxspan = yspan;

	mov	ecx, DWORD PTR _yspan$[ebp]

; 185  :   	dia1 = ymin;

	mov	edx, DWORD PTR _ymin$[ebp+4]
	mov	eax, DWORD PTR _ymin$[ebp+8]
	mov	DWORD PTR _maxspan$[ebp], ecx

; 186  :   	dia2 = ymax;

	mov	ecx, DWORD PTR _ymax$[ebp]
	mov	DWORD PTR _dia1$[ebp+4], edx
	mov	edx, DWORD PTR _ymax$[ebp+4]
	mov	DWORD PTR _dia1$[ebp+8], eax
	mov	eax, DWORD PTR _ymax$[ebp+8]
	mov	DWORD PTR _dia2$[ebp], ecx
	fstp	ST(0)
	fld	DWORD PTR _ymin$[ebp]
	mov	DWORD PTR _dia2$[ebp+4], edx
	mov	DWORD PTR _dia2$[ebp+8], eax
$L14188:

; 187  : 	}
; 188  : 
; 189  :   if (zspan>maxspan)

	fld	DWORD PTR _zspan$[ebp]
	fcomp	DWORD PTR _maxspan$[ebp]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L14194

; 190  : 	{
; 191  : 	  dia1 = zmin;

	mov	ecx, DWORD PTR _zmin$[ebp+4]
	mov	edx, DWORD PTR _zmin$[ebp+8]

; 192  : 	  dia2 = zmax;

	mov	eax, DWORD PTR _zmax$[ebp]
	mov	DWORD PTR _dia1$[ebp+4], ecx
	mov	ecx, DWORD PTR _zmax$[ebp+4]
	mov	DWORD PTR _dia1$[ebp+8], edx
	mov	edx, DWORD PTR _zmax$[ebp+8]
	mov	DWORD PTR _dia2$[ebp], eax
	fstp	ST(0)
	fld	DWORD PTR _zmin$[ebp]
	mov	DWORD PTR _dia2$[ebp+4], ecx
	mov	DWORD PTR _dia2$[ebp+8], edx
$L14194:

; 193  : 	}
; 194  : 
; 195  : 
; 196  :   /* dia1,dia2 is a diameter of initial sphere */
; 197  :   /* calc initial center */
; 198  :   mCenter.SetX( (dia1.GetX()+dia2.GetX())*0.5f );

	fadd	DWORD PTR _dia2$[ebp]

; 199  :   mCenter.SetY( (dia1.GetY()+dia2.GetY())*0.5f );
; 200  :   mCenter.SetZ( (dia1.GetZ()+dia2.GetZ())*0.5f );
; 201  :   /* calculate initial radius**2 and radius */
; 202  :   dx = dia2.GetX()-mCenter.GetX(); /* x component of radius vector */
; 203  :   dy = dia2.GetY()-mCenter.GetY(); /* y component of radius vector */
; 204  :   dz = dia2.GetZ()-mCenter.GetZ(); /* z component of radius vector */
; 205  :   mRadius2 = dx*dx + dy*dy + dz*dz;
; 206  :   mRadius = float(sqrt(mRadius2));
; 207  : 
; 208  :   /* SECOND PASS: increment current sphere */
; 209  : 
; 210  :   for (i=0; i<count; i++)

	mov	eax, DWORD PTR _count$[ebp]
	xor	ebx, ebx
	test	eax, eax
	fmul	DWORD PTR __real@4@3ffe8000000000000000
	fst	DWORD PTR [esi]
	fld	DWORD PTR _dia1$[ebp+4]
	fadd	DWORD PTR _dia2$[ebp+4]
	fmul	DWORD PTR __real@4@3ffe8000000000000000
	fst	DWORD PTR 8+[ebp]
	fstp	DWORD PTR [esi+4]
	fld	DWORD PTR _dia1$[ebp+8]
	fadd	DWORD PTR _dia2$[ebp+8]
	fmul	DWORD PTR __real@4@3ffe8000000000000000
	fst	DWORD PTR -16+[ebp]
	fstp	DWORD PTR [esi+8]
	fsubr	DWORD PTR _dia2$[ebp]
	fld	DWORD PTR _dia2$[ebp+4]
	fsub	DWORD PTR 8+[ebp]
	fld	DWORD PTR _dia2$[ebp+8]
	fsub	DWORD PTR -16+[ebp]
	fld	ST(0)
	fmul	ST(0), ST(1)
	fld	ST(2)
	fmul	ST(0), ST(3)
	faddp	ST(1), ST(0)
	fld	ST(3)
	fmul	ST(0), ST(4)
	faddp	ST(1), ST(0)
	fstp	ST(3)
	fstp	ST(0)
	fstp	ST(0)
	fst	DWORD PTR [esi+16]
	fsqrt
	fstp	DWORD PTR [esi+12]
	jle	$L12238
$L12236:

; 211  : 	{
; 212  :     Vector3d<float> caller_p;
; 213  :     source.GetVertex(i,caller_p);

	mov	eax, DWORD PTR [edi]
	lea	ecx, DWORD PTR _caller_p$12239[ebp]
	push	ecx
	push	ebx
	mov	ecx, edi
	call	DWORD PTR [eax+4]

; 214  :   	dx = caller_p.GetX()-mCenter.GetX();

	fld	DWORD PTR [esi]
	fsubr	DWORD PTR _caller_p$12239[ebp]

; 215  : 	  dy = caller_p.GetY()-mCenter.GetY();

	fld	DWORD PTR _caller_p$12239[ebp+4]
	fsub	DWORD PTR [esi+4]

; 216  :   	dz = caller_p.GetZ()-mCenter.GetZ();

	fld	DWORD PTR _caller_p$12239[ebp+8]
	fsub	DWORD PTR [esi+8]

; 217  : 	  float old_to_p_sq = dx*dx + dy*dy + dz*dz;

	fld	ST(0)
	fmul	ST(0), ST(1)
	fld	ST(2)
	fmul	ST(0), ST(3)
	faddp	ST(1), ST(0)
	fld	ST(3)
	fmul	ST(0), ST(4)
	faddp	ST(1), ST(0)
	fstp	ST(3)
	fstp	ST(0)
	fstp	ST(0)

; 218  :   	if (old_to_p_sq > mRadius2) 	/* do r**2 test first */

	fcom	DWORD PTR [esi+16]
	fnstsw	ax
	test	ah, 65					; 00000041H
	jne	SHORT $L14322

; 219  : 		{ 	/* this point is outside of current sphere */
; 220  : 	  	float old_to_p = float(sqrt(old_to_p_sq));

	fsqrt

; 221  : 		  /* calc radius of new sphere */
; 222  :   		mRadius = (mRadius + old_to_p) * 0.5f;

	fld	ST(0)
	fadd	DWORD PTR [esi+12]
	fmul	DWORD PTR __real@4@3ffe8000000000000000
	fstp	DWORD PTR 8+[ebp]

; 223  : 	  	mRadius2 = mRadius*mRadius; 	/* for next r**2 compare */

	fld	DWORD PTR 8+[ebp]
	fmul	DWORD PTR 8+[ebp]
	mov	edx, DWORD PTR 8+[ebp]
	mov	DWORD PTR [esi+12], edx
	fstp	DWORD PTR [esi+16]

; 224  :   		float old_to_new = old_to_p - mRadius;

	fld	ST(0)
	fsub	DWORD PTR 8+[ebp]
	fstp	DWORD PTR _old_to_new$12243[ebp]

; 225  : 	  	/* calc center of new sphere */
; 226  :       float recip = 1.0f /old_to_p;

	fdivr	DWORD PTR __real@4@3fff8000000000000000

; 227  : 
; 228  :   		float cx = (mRadius*mCenter.GetX() + old_to_new*caller_p.GetX()) * recip;

	fld	DWORD PTR [esi]
	fmul	DWORD PTR 8+[ebp]
	fld	DWORD PTR _caller_p$12239[ebp]
	fmul	DWORD PTR _old_to_new$12243[ebp]
	faddp	ST(1), ST(0)
	fmul	ST(0), ST(1)

; 229  : 	  	float cy = (mRadius*mCenter.GetY() + old_to_new*caller_p.GetY()) * recip;
; 230  : 		  float cz = (mRadius*mCenter.GetZ() + old_to_new*caller_p.GetZ()) * recip;
; 231  : 
; 232  :       mCenter.Set(cx,cy,cz);

	fstp	DWORD PTR [esi]
	fld	DWORD PTR _caller_p$12239[ebp+4]
	fmul	DWORD PTR _old_to_new$12243[ebp]
	fld	DWORD PTR [esi+4]
	fmul	DWORD PTR 8+[ebp]
	faddp	ST(1), ST(0)
	fmul	ST(0), ST(1)
	fstp	DWORD PTR [esi+4]
	fld	DWORD PTR [esi+8]
	fmul	DWORD PTR 8+[ebp]
	fld	DWORD PTR _caller_p$12239[ebp+8]
	fmul	DWORD PTR _old_to_new$12243[ebp]
	faddp	ST(1), ST(0)
	fmul	ST(0), ST(1)
	fstp	DWORD PTR [esi+8]
$L14322:
	mov	eax, DWORD PTR _count$[ebp]
	inc	ebx
	cmp	ebx, eax
	fstp	ST(0)
	jl	$L12236
$L12238:
	pop	edi
	pop	esi
	pop	ebx

; 233  : 		}
; 234  : 	}
; 235  : }

	mov	esp, ebp
	pop	ebp
	ret	4
?Compute@Sphere@@QAEXABVSphereInterface@@@Z ENDP	; Sphere::Compute
_TEXT	ENDS
;	COMDAT ??0?$Vector3d@M@@QAE@ABV0@@Z
_TEXT	SEGMENT
_a$ = 8
??0?$Vector3d@M@@QAE@ABV0@@Z PROC NEAR			; Vector3d<float>::Vector3d<float>, COMDAT

; 30   :   Vector3d(const Vector3d &a) // constructor copies existing vector.

	push	ebp
	mov	ebp, esp
	mov	eax, ecx

; 31   :   {
; 32   :     x = a.x;

	mov	ecx, DWORD PTR _a$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx

; 33   :     y = a.y;

	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR [eax+4], edx

; 34   :     z = a.z;

	mov	ecx, DWORD PTR [ecx+8]
	mov	DWORD PTR [eax+8], ecx

; 35   :   };

	pop	ebp
	ret	4
??0?$Vector3d@M@@QAE@ABV0@@Z ENDP			; Vector3d<float>::Vector3d<float>
_TEXT	ENDS
;	COMDAT ??4?$Vector3d@M@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT
_A$ = 8
??4?$Vector3d@M@@QAEAAV0@ABV0@@Z PROC NEAR		; Vector3d<float>::operator=, COMDAT

; 59   :       { x=A.x; y=A.y; z=A.z;

	push	ebp
	mov	ebp, esp
	mov	eax, ecx
	mov	ecx, DWORD PTR _A$[ebp]
	mov	edx, DWORD PTR [ecx]
	mov	DWORD PTR [eax], edx
	mov	edx, DWORD PTR [ecx+4]
	mov	DWORD PTR [eax+4], edx
	mov	ecx, DWORD PTR [ecx+8]
	mov	DWORD PTR [eax+8], ecx

; 60   :         return(*this);  };

	pop	ebp
	ret	4
??4?$Vector3d@M@@QAEAAV0@ABV0@@Z ENDP			; Vector3d<float>::operator=
_TEXT	ENDS
PUBLIC	?id@?$ctype@G@std@@$E				; std::ctype<unsigned short>::id
EXTRN	_atexit:NEAR
_DATA	SEGMENT
COMM	??_B?1???id@?$ctype@G@std@@$D@@9@51:BYTE							; std::ctype<unsigned short>::d::`local static guard'
_DATA	ENDS
_TEXT	SEGMENT
_$E30	PROC NEAR
	mov	cl, BYTE PTR ??_B?1???id@?$ctype@G@std@@$D@@9@51 ; std::ctype<unsigned short>::d::`local static guard'
	mov	al, 1
	test	cl, al
	jne	SHORT $L14331
	or	cl, al
	mov	BYTE PTR ??_B?1???id@?$ctype@G@std@@$D@@9@51, cl ; std::ctype<unsigned short>::d::`local static guard'
$L14331:
	push	OFFSET FLAT:?id@?$ctype@G@std@@$E	; std::ctype<unsigned short>::id
	call	_atexit
	pop	ecx
	ret	0
_$E30	ENDP
_TEXT	ENDS
;	COMDAT ?id@?$ctype@G@std@@$E
_TEXT	SEGMENT
?id@?$ctype@G@std@@$E PROC NEAR				; std::ctype<unsigned short>::id, COMDAT
	ret	0
?id@?$ctype@G@std@@$E ENDP				; std::ctype<unsigned short>::id
_TEXT	ENDS
EXTRN	??0Init@ios_base@std@@QAE@XZ:NEAR		; std::ios_base::Init::Init
_BSS	SEGMENT
__Ios_init DB	01H DUP (?)
_BSS	ENDS
_TEXT	SEGMENT
_$E17	PROC NEAR
	mov	ecx, OFFSET FLAT:__Ios_init
	call	??0Init@ios_base@std@@QAE@XZ		; std::ios_base::Init::Init
	push	OFFSET FLAT:_$E15
	call	_atexit
	pop	ecx
	ret	0
_$E17	ENDP
_TEXT	ENDS
EXTRN	??1Init@ios_base@std@@QAE@XZ:NEAR		; std::ios_base::Init::~Init
_TEXT	SEGMENT
_$E15	PROC NEAR
	mov	ecx, OFFSET FLAT:__Ios_init
	jmp	??1Init@ios_base@std@@QAE@XZ		; std::ios_base::Init::~Init
_$E15	ENDP
_TEXT	ENDS
EXTRN	??0_Winit@std@@QAE@XZ:NEAR			; std::_Winit::_Winit
_BSS	SEGMENT
	ALIGN	4

__Wios_init DB	01H DUP (?)
_BSS	ENDS
_TEXT	SEGMENT
_$E23	PROC NEAR
	mov	ecx, OFFSET FLAT:__Wios_init
	call	??0_Winit@std@@QAE@XZ			; std::_Winit::_Winit
	push	OFFSET FLAT:_$E21
	call	_atexit
	pop	ecx
	ret	0
_$E23	ENDP
_TEXT	ENDS
EXTRN	??1_Winit@std@@QAE@XZ:NEAR			; std::_Winit::~_Winit
_TEXT	SEGMENT
_$E21	PROC NEAR
	mov	ecx, OFFSET FLAT:__Wios_init
	jmp	??1_Winit@std@@QAE@XZ			; std::_Winit::~_Winit
_$E21	ENDP
_TEXT	ENDS
END
