	TITLE	C:\WINDOWS\Desktop\Gems2 CD\SourceCode\04 Geometry Management\03 Ratcliff\main.cpp
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
;	COMDAT ??8@YAHABU_GUID@@0@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?PauseApp@Application@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?RunApp@Application@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ExitApp@Application@@QAE_NXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?CloseApp@Application@@QAEX_N@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
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
;	COMDAT ?GetRadius@Sphere@@QBEMXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetRadius@Sphere@@QAEXM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0Sphere@@QAE@XZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?HasSpherePackFlag@SpherePack@@QBE_NW4SpherePackFlag@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetParent@SpherePack@@QAEXPAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Unlink@SpherePack@@QAEXXZ
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetNextSibling@SpherePack@@QAEXPAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetPrevSibling@SpherePack@@QAEXPAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?DistanceSquared@SpherePack@@QBEMPBV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?ComputeBindingDistance@SpherePack@@QAEXPAV1@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?SetState@CircleFactory@@QAEXW4CircleState@@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GApplication@@QAEPAXI@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??_GCircleFactory@@QAEPAXI@Z
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
;	COMDAT ??0?$Vector3d@M@@QAE@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??0?$Vector3d@M@@QAE@MMM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ??4?$Vector3d@M@@QAEAAV0@ABV0@@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Set@?$Vector3d@M@@QAEXMMM@Z
_TEXT	SEGMENT PARA USE32 PUBLIC 'CODE'
_TEXT	ENDS
;	COMDAT ?Distance2@?$Vector3d@M@@QBEMABV1@@Z
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
PUBLIC	?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
PUBLIC	?backbuffer@@3PAUHDC__@@A			; backbuffer
PUBLIC	?backbufferbitmap@@3PAUHBITMAP__@@A		; backbufferbitmap
PUBLIC	?textbuffer@@3PADA				; textbuffer
_BSS	SEGMENT
?mCircleFactory@Application@@0PAVCircleFactory@@A DD 01H DUP (?) ; Application::mCircleFactory
?backbuffer@@3PAUHDC__@@A DD 01H DUP (?)		; backbuffer
?backbufferbitmap@@3PAUHBITMAP__@@A DD 01H DUP (?)	; backbufferbitmap
?textbuffer@@3PADA DB 0400H DUP (?)			; textbuffer
_BSS	ENDS
CRT$XCU	SEGMENT
_$S19	DD	FLAT:_$E18
_$S25	DD	FLAT:_$E24
_$S32	DD	FLAT:_$E31
CRT$XCU	ENDS
_DATA	SEGMENT
$SG53997 DB	'spheretest_window_class', 00H
$SG53999 DB	'Sphere Tree for Game Programming Gems 2 : by John W. Rat'
	DB	'cliff.', 00H
	ORG $+1
$SG54000 DB	'spheretest_window_class', 00H
_DATA	ENDS
PUBLIC	?Startup@Application@@QAEHPAUHINSTANCE__@@@Z	; Application::Startup
PUBLIC	?Render@Application@@QAEHPAUHDC__@@@Z		; Application::Render
PUBLIC	_WinMain@16
EXTRN	??2@YAPAXI@Z:NEAR				; operator new
EXTRN	??3@YAXPAX@Z:NEAR				; operator delete
EXTRN	__imp__DeleteDC@4:NEAR
EXTRN	__imp__DeleteObject@4:NEAR
EXTRN	_exit:NEAR
EXTRN	_atoi:NEAR
EXTRN	__imp__timeGetTime@0:NEAR
EXTRN	__imp__Sleep@4:NEAR
EXTRN	__imp__TranslateMessage@4:NEAR
EXTRN	__imp__DispatchMessageA@4:NEAR
EXTRN	__imp__PeekMessageA@20:NEAR
EXTRN	??1CircleFactory@@QAE@XZ:NEAR			; CircleFactory::~CircleFactory
EXTRN	__imp__GetDC@4:NEAR
EXTRN	__imp__ReleaseDC@8:NEAR
_TEXT	SEGMENT
_appInstance$ = 8
_cmdLine$ = 16
_msg$ = -28
_WinMain@16 PROC NEAR

; 26   : {

	push	ebp
	mov	ebp, esp
	sub	esp, 28					; 0000001cH

; 27   : 	MSG			msg;
; 28   : 
; 29   :   int count = atoi( cmdLine );

	mov	eax, DWORD PTR _cmdLine$[ebp]
	push	ebx
	push	esi
	push	edi
	push	eax
	call	_atoi
	mov	edi, eax
	add	esp, 4

; 30   :   if ( count < 20 )    count = 1000; // default value is 1000

	cmp	edi, 20					; 00000014H
	jge	SHORT $L53954
	mov	edi, 1000				; 000003e8H

; 31   :   if ( count > 10000 ) count = 10000;

	jmp	SHORT $L53955
$L53954:
	cmp	edi, 10000				; 00002710H
	jle	SHORT $L53955
	mov	edi, 10000				; 00002710H
$L53955:

; 32   : 	
; 33   : 	Application* App = new Application(count);

	push	1127					; 00000467H
	call	??2@YAPAXI@Z				; operator new
	mov	esi, eax
	xor	ebx, ebx
	add	esp, 4
	cmp	esi, ebx
	je	SHORT $L55665
	mov	DWORD PTR [esi+1123], edi
	mov	BYTE PTR [esi+1120], bl
	mov	DWORD PTR [esi], ebx
	mov	DWORD PTR [esi+52], ebx
	mov	DWORD PTR [esi+56], ebx
	call	DWORD PTR __imp__timeGetTime@0
	mov	DWORD PTR [esi+60], eax
	mov	BYTE PTR [esi+1122], bl
	mov	BYTE PTR [esi+1121], bl
	jmp	SHORT $L55666
$L55665:
	xor	esi, esi
$L55666:

; 34   : 
; 35   : 	if (App->Startup(appInstance))

	mov	ecx, DWORD PTR _appInstance$[ebp]
	push	ecx
	mov	ecx, esi
	call	?Startup@Application@@QAEHPAUHINSTANCE__@@@Z ; Application::Startup
	test	eax, eax
	je	SHORT $L55751

; 36   : 	{
; 37   : 		delete (App);

	cmp	esi, ebx
	je	SHORT $L55708
	mov	edi, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	cmp	edi, ebx
	je	SHORT $L55705
	mov	ecx, edi
	call	??1CircleFactory@@QAE@XZ		; CircleFactory::~CircleFactory
	push	edi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L55705:
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L55708:

; 38   : 		exit(-1);

	push	-1
	call	_exit
$L55753:
$L55751:

; 39   : 	}
; 40   : 
; 41   : 	while (!App->ExitApp())

	mov	al, BYTE PTR [esi+1120]
	test	al, al
	jne	SHORT $L53966

; 47   : 		}
; 48   : 
; 49   : 		App->Process();

	mov	ebx, DWORD PTR __imp__GetDC@4
$L53965:

; 42   : 	{
; 43   : 		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))

	push	1
	push	0
	push	0
	lea	edx, DWORD PTR _msg$[ebp]
	push	0
	push	edx
	call	DWORD PTR __imp__PeekMessageA@20
	test	eax, eax
	je	SHORT $L53967

; 44   : 		{
; 45   : 			TranslateMessage(&msg);

	lea	eax, DWORD PTR _msg$[ebp]
	push	eax
	call	DWORD PTR __imp__TranslateMessage@4

; 46   : 			DispatchMessage(&msg);

	lea	ecx, DWORD PTR _msg$[ebp]
	push	ecx
	call	DWORD PTR __imp__DispatchMessageA@4
$L53967:

; 47   : 		}
; 48   : 
; 49   : 		App->Process();

	mov	edx, DWORD PTR [esi+52]
	push	edx
	call	ebx
	mov	edi, eax
	mov	ecx, esi
	push	edi
	call	?Render@Application@@QAEHPAUHDC__@@@Z	; Application::Render
	mov	eax, DWORD PTR [esi+52]
	push	edi
	push	eax
	call	DWORD PTR __imp__ReleaseDC@8
	push	1
	call	DWORD PTR __imp__Sleep@4
	mov	al, BYTE PTR [esi+1120]
	test	al, al
	je	SHORT $L53965

; 39   : 	}
; 40   : 
; 41   : 	while (!App->ExitApp())

	xor	ebx, ebx
$L53966:

; 50   : 	}
; 51   : 
; 52   : 	App->Shutdown();

	mov	ecx, DWORD PTR ?backbufferbitmap@@3PAUHBITMAP__@@A ; backbufferbitmap
	push	ecx
	call	DWORD PTR __imp__DeleteObject@4
	mov	edx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	edx
	call	DWORD PTR __imp__DeleteDC@4

; 53   : 
; 54   : 	delete(App);

	cmp	esi, ebx
	mov	DWORD PTR ?backbuffer@@3PAUHDC__@@A, ebx ; backbuffer
	je	SHORT $L55750
	mov	edi, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	cmp	edi, ebx
	je	SHORT $L55747
	mov	ecx, edi
	call	??1CircleFactory@@QAE@XZ		; CircleFactory::~CircleFactory
	push	edi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L55747:
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L55750:
	pop	edi
	pop	esi

; 55   : 
; 56   : 	return 0;

	xor	eax, eax
	pop	ebx

; 57   : }

	mov	esp, ebp
	pop	ebp
	ret	16					; 00000010H
$L55752:
_WinMain@16 ENDP
_TEXT	ENDS
PUBLIC	??0Application@@QAE@H@Z				; Application::Application
_TEXT	SEGMENT
_simcount$ = 8
??0Application@@QAE@H@Z PROC NEAR			; Application::Application

; 63   : {

	push	ebp
	mov	ebp, esp

; 64   :   mSimulationCount  = simcount;

	mov	eax, DWORD PTR _simcount$[ebp]
	push	ebx
	push	esi
	mov	esi, ecx

; 65   : 	mExitApplication	= false;

	xor	ebx, ebx
	mov	DWORD PTR [esi+1123], eax
	mov	BYTE PTR [esi+1120], bl

; 66   : 	mAppInstance			= NULL;

	mov	DWORD PTR [esi], ebx

; 67   : 	mWindow						= NULL;

	mov	DWORD PTR [esi+52], ebx

; 68   : 	mFrameRate				= 0;

	mov	DWORD PTR [esi+56], ebx

; 69   : 	mUpdateFrameRate	= timeGetTime();

	call	DWORD PTR __imp__timeGetTime@0
	mov	DWORD PTR [esi+60], eax

; 70   : 	mStep							= false;

	mov	BYTE PTR [esi+1122], bl

; 71   : 	mPause						= false;

	mov	BYTE PTR [esi+1121], bl

; 72   : }

	mov	eax, esi
	pop	esi
	pop	ebx
	pop	ebp
	ret	4
??0Application@@QAE@H@Z ENDP				; Application::Application
_TEXT	ENDS
PUBLIC	??1Application@@QAE@XZ				; Application::~Application
_TEXT	SEGMENT
??1Application@@QAE@XZ PROC NEAR			; Application::~Application

; 77   : {

	push	esi

; 78   : 	delete (mCircleFactory);

	mov	esi, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	test	esi, esi
	je	SHORT $L55758
	mov	ecx, esi
	call	??1CircleFactory@@QAE@XZ		; CircleFactory::~CircleFactory
	push	esi
	call	??3@YAXPAX@Z				; operator delete
	add	esp, 4
$L55758:
	pop	esi

; 79   : }

	ret	0
??1Application@@QAE@XZ ENDP				; Application::~Application
_TEXT	ENDS
PUBLIC	?WindowProc@Application@@SGJPAUHWND__@@IIJ@Z	; Application::WindowProc
EXTRN	__imp__CreateCompatibleBitmap@12:NEAR
EXTRN	__imp__CreateCompatibleDC@4:NEAR
EXTRN	__imp__LoadCursorA@8:NEAR
EXTRN	_srand:NEAR
EXTRN	__except_list:DWORD
EXTRN	___CxxFrameHandler:NEAR
EXTRN	__imp__GetStockObject@4:NEAR
EXTRN	??0CircleFactory@@QAE@H@Z:NEAR			; CircleFactory::CircleFactory
EXTRN	__imp__RegisterClassExA@4:NEAR
EXTRN	__imp__CreateWindowExA@48:NEAR
EXTRN	__imp__ShowWindow@8:NEAR
EXTRN	__imp__SelectObject@8:NEAR
EXTRN	__imp__SetBkMode@8:NEAR
EXTRN	__imp__SetTextColor@8:NEAR
EXTRN	__imp__SetWindowLongA@12:NEAR
xdata$x	SEGMENT
$T55773	DD	019930520H
	DD	01H
	DD	FLAT:$T55776
	DD	2 DUP(00H)
	DD	2 DUP(00H)
	ORG $+4
$T55776	DD	0ffffffffH
	DD	FLAT:$L55769
xdata$x	ENDS
_TEXT	SEGMENT
$T55766 = 8
__$EHRec$ = -12
_hinstance$ = 8
?Startup@Application@@QAEHPAUHINSTANCE__@@@Z PROC NEAR	; Application::Startup

; 84   : {

	push	ebp
	mov	ebp, esp
	mov	eax, DWORD PTR fs:__except_list
	push	-1
	push	$L55774
	push	eax
	mov	DWORD PTR fs:__except_list, esp
	push	ebx
	push	esi
	push	edi
	mov	esi, ecx

; 85   : 	srand(timeGetTime());

	call	DWORD PTR __imp__timeGetTime@0
	push	eax
	call	_srand

; 86   : 
; 87   : 	mAppInstance									= hinstance;

	mov	eax, DWORD PTR _hinstance$[ebp]
	add	esp, 4

; 88   : 
; 89   : 	mWindowClass.cbSize						= sizeof(WNDCLASSEX);

	lea	ebx, DWORD PTR [esi+4]

; 90   : 	mWindowClass.cbClsExtra				= 0;

	xor	edi, edi

; 91   : 	mWindowClass.cbWndExtra				= 0;
; 92   : 	mWindowClass.hbrBackground		= (HBRUSH)GetStockObject(COLOR_BACKGROUND);

	push	1
	mov	DWORD PTR [esi], eax
	mov	DWORD PTR [ebx], 48			; 00000030H
	mov	DWORD PTR [esi+16], edi
	mov	DWORD PTR [esi+20], edi
	call	DWORD PTR __imp__GetStockObject@4

; 93   : 	mWindowClass.hCursor					= LoadCursor(NULL, IDC_ARROW);

	push	32512					; 00007f00H
	push	edi
	mov	DWORD PTR [esi+36], eax
	call	DWORD PTR __imp__LoadCursorA@8

; 94   : 	mWindowClass.hIcon						= NULL;
; 95   : 	mWindowClass.hIconSm					= NULL;
; 96   : 	mWindowClass.hInstance				= hinstance;

	mov	ecx, DWORD PTR _hinstance$[ebp]

; 97   : 	mWindowClass.style						= NULL;
; 98   : 	mWindowClass.lpfnWndProc			= Application::WindowProc;
; 99   : 	mWindowClass.lpszMenuName			= NULL;
; 100  : 	mWindowClass.lpszClassName		= "spheretest_window_class";
; 101  : 
; 102  : 	ATOM atom = RegisterClassEx(&mWindowClass);

	push	ebx
	mov	DWORD PTR [esi+32], eax
	mov	DWORD PTR [esi+28], edi
	mov	DWORD PTR [esi+48], edi
	mov	DWORD PTR [esi+24], ecx
	mov	DWORD PTR [esi+8], edi
	mov	DWORD PTR [esi+12], OFFSET FLAT:?WindowProc@Application@@SGJPAUHWND__@@IIJ@Z ; Application::WindowProc
	mov	DWORD PTR [esi+40], edi
	mov	DWORD PTR [esi+44], OFFSET FLAT:$SG53997
	call	DWORD PTR __imp__RegisterClassExA@4

; 103  : 
; 104  : 	mWindow = CreateWindow(	"spheretest_window_class", 
; 105  : 													"Sphere Tree for Game Programming Gems 2 : by John W. Ratcliff.",
; 106  : 													WS_OVERLAPPEDWINDOW, 30, 30, SCREEN_WIDTH+8, SCREEN_HEIGHT+28, 
; 107  : 													NULL, NULL, mAppInstance, NULL);

	mov	edx, DWORD PTR [esi]
	push	edi
	push	edx
	push	edi
	push	edi
	push	796					; 0000031cH
	push	1032					; 00000408H
	push	30					; 0000001eH
	push	30					; 0000001eH
	push	13565952				; 00cf0000H
	push	OFFSET FLAT:$SG53999
	push	OFFSET FLAT:$SG54000
	push	edi
	call	DWORD PTR __imp__CreateWindowExA@48

; 108  : 
; 109  : 	SetWindowLong(mWindow, GWL_USERDATA, (long)this);

	push	esi
	push	-21					; ffffffebH
	push	eax
	mov	DWORD PTR [esi+52], eax
	call	DWORD PTR __imp__SetWindowLongA@12

; 110  : 	ShowWindow(mWindow, SW_SHOW);

	mov	eax, DWORD PTR [esi+52]
	push	5
	push	eax
	call	DWORD PTR __imp__ShowWindow@8

; 111  : 
; 112  : 	mCircleFactory = new CircleFactory(mSimulationCount);

	push	372					; 00000174H
	call	??2@YAPAXI@Z				; operator new
	add	esp, 4
	mov	DWORD PTR $T55766[ebp], eax
	cmp	eax, edi
	mov	DWORD PTR __$EHRec$[ebp+8], edi
	je	SHORT $L55767
	mov	ecx, DWORD PTR [esi+1123]
	push	ecx
	mov	ecx, eax
	call	??0CircleFactory@@QAE@H@Z		; CircleFactory::CircleFactory
	jmp	SHORT $L55768
$L55767:
	xor	eax, eax
$L55768:
	mov	DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A, eax ; Application::mCircleFactory

; 113  : 
; 114  : 	HDC hdc = GetDC(mWindow);

	mov	edx, DWORD PTR [esi+52]
	push	edx
	call	DWORD PTR __imp__GetDC@4
	mov	edi, eax

; 115  : 	backbuffer = CreateCompatibleDC(hdc);

	push	edi
	call	DWORD PTR __imp__CreateCompatibleDC@4

; 116  : 	HBITMAP bitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);

	push	768					; 00000300H
	push	1024					; 00000400H
	push	edi
	mov	DWORD PTR ?backbuffer@@3PAUHDC__@@A, eax ; backbuffer
	call	DWORD PTR __imp__CreateCompatibleBitmap@12

; 117  : 	SelectObject(backbuffer, bitmap);

	push	eax
	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	eax
	call	DWORD PTR __imp__SelectObject@8

; 118  : 	ReleaseDC(mWindow, hdc);

	mov	ecx, DWORD PTR [esi+52]
	push	edi
	push	ecx
	call	DWORD PTR __imp__ReleaseDC@8

; 119  : 
; 120  : 	SetTextColor(backbuffer, 0x00ffff00);

	mov	edx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	16776960				; 00ffff00H
	push	edx
	call	DWORD PTR __imp__SetTextColor@8

; 121  : 	SetBkMode(backbuffer, TRANSPARENT);

	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	1
	push	eax
	call	DWORD PTR __imp__SetBkMode@8

; 122  : 
; 123  : 	return 0;
; 124  : }

	mov	ecx, DWORD PTR __$EHRec$[ebp]
	pop	edi
	pop	esi
	xor	eax, eax
	mov	DWORD PTR fs:__except_list, ecx
	pop	ebx
	mov	esp, ebp
	pop	ebp
	ret	4
_TEXT	ENDS
text$x	SEGMENT
$L55769:
	mov	eax, DWORD PTR $T55766[ebp]
	push	eax
	call	??3@YAXPAX@Z				; operator delete
	pop	ecx
	ret	0
$L55774:
	mov	eax, OFFSET FLAT:$T55773
	jmp	___CxxFrameHandler
text$x	ENDS
?Startup@Application@@QAEHPAUHINSTANCE__@@@Z ENDP	; Application::Startup
PUBLIC	?Shutdown@Application@@QAEHXZ			; Application::Shutdown
_TEXT	SEGMENT
?Shutdown@Application@@QAEHXZ PROC NEAR			; Application::Shutdown

; 130  : 	DeleteObject(backbufferbitmap);

	mov	eax, DWORD PTR ?backbufferbitmap@@3PAUHBITMAP__@@A ; backbufferbitmap
	push	eax
	call	DWORD PTR __imp__DeleteObject@4

; 131  : 	DeleteDC(backbuffer);

	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	ecx
	call	DWORD PTR __imp__DeleteDC@4

; 132  : 	backbuffer = 0;

	mov	DWORD PTR ?backbuffer@@3PAUHDC__@@A, 0	; backbuffer

; 133  : 
; 134  : 	return 0;

	xor	eax, eax

; 135  : }

	ret	0
?Shutdown@Application@@QAEHXZ ENDP			; Application::Shutdown
_TEXT	ENDS
PUBLIC	?Process@Application@@QAEHXZ			; Application::Process
_TEXT	SEGMENT
?Process@Application@@QAEHXZ PROC NEAR			; Application::Process

; 140  : {	

	push	esi
	mov	esi, ecx
	push	edi

; 141  : 	HDC hdc = GetDC(mWindow);

	mov	eax, DWORD PTR [esi+52]
	push	eax
	call	DWORD PTR __imp__GetDC@4
	mov	edi, eax

; 142  : 	Render(hdc);

	mov	ecx, esi
	push	edi
	call	?Render@Application@@QAEHPAUHDC__@@@Z	; Application::Render

; 143  : 	ReleaseDC(mWindow, hdc);

	mov	ecx, DWORD PTR [esi+52]
	push	edi
	push	ecx
	call	DWORD PTR __imp__ReleaseDC@8

; 144  : 	Sleep(1);

	push	1
	call	DWORD PTR __imp__Sleep@4
	pop	edi

; 145  : 
; 146  : 	return 0;

	xor	eax, eax
	pop	esi

; 147  : }

	ret	0
?Process@Application@@QAEHXZ ENDP			; Application::Process
_TEXT	ENDS
EXTRN	__imp__BitBlt@36:NEAR
EXTRN	__imp__TextOutA@20:NEAR
EXTRN	_itoa:NEAR
EXTRN	?Process@CircleFactory@@QAEHXZ:NEAR		; CircleFactory::Process
EXTRN	?Render@CircleFactory@@QAEHXZ:NEAR		; CircleFactory::Render
_TEXT	SEGMENT
_hdc$ = 8
?Render@Application@@QAEHPAUHDC__@@@Z PROC NEAR		; Application::Render

; 152  : {	

	push	ebp
	mov	ebp, esp

; 153  : 	BitBlt(backbuffer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backbuffer, 0, 0, BLACKNESS);

	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	ebx
	push	esi
	push	edi
	push	66					; 00000042H
	mov	ebx, DWORD PTR __imp__BitBlt@36
	push	0
	push	0
	push	eax
	push	768					; 00000300H
	push	1024					; 00000400H
	push	0
	push	0
	mov	esi, ecx
	push	eax
	call	ebx

; 154  : 
; 155  : 	mFrameRate++;

	mov	edx, DWORD PTR [esi+56]

; 156  : 	if (timeGetTime() > mUpdateFrameRate)

	mov	edi, DWORD PTR [esi+60]
	inc	edx
	mov	DWORD PTR [esi+56], edx
	call	DWORD PTR __imp__timeGetTime@0
	cmp	eax, edi
	jbe	SHORT $L54022

; 157  : 	{
; 158  : 		mUpdateFrameRate += 1000;
; 159  : 		itoa(mFrameRate, mBuffer, 10);

	mov	ecx, DWORD PTR [esi+56]
	lea	eax, DWORD PTR [esi+64]
	push	10					; 0000000aH
	add	edi, 1000				; 000003e8H
	push	eax
	push	ecx
	mov	DWORD PTR [esi+60], edi
	call	_itoa
	add	esp, 12					; 0000000cH

; 160  : 		mFrameRate = 0;

	mov	DWORD PTR [esi+56], 0
$L54022:

; 161  : 	}
; 162  : 	TextOut(backbuffer, 2, 2, mBuffer, strlen(mBuffer)); 

	lea	edx, DWORD PTR [esi+64]
	or	ecx, -1
	mov	edi, edx
	xor	eax, eax
	repne scasb
	not	ecx
	dec	ecx
	push	ecx
	push	edx
	mov	edx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	2
	push	2
	push	edx
	call	DWORD PTR __imp__TextOutA@20

; 163  : 
; 164  : 	//  Here is where we put in the code to render the spheres
; 165  : 	if (!mPause)

	mov	al, BYTE PTR [esi+1121]
	test	al, al

; 166  : 	{
; 167  : 		mCircleFactory->Process();
; 168  : 	}
; 169  : 	else if (mStep)

	je	SHORT $L55781
	mov	al, BYTE PTR [esi+1122]
	test	al, al
	je	SHORT $L54025

; 170  : 	{
; 171  : 		mPause = true;

	mov	BYTE PTR [esi+1121], 1

; 172  : 		mStep = false;

	mov	BYTE PTR [esi+1122], 0
$L55781:

; 173  : 		mCircleFactory->Process();

	mov	ecx, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	call	?Process@CircleFactory@@QAEHXZ		; CircleFactory::Process
$L54025:

; 174  : 	}
; 175  : 
; 176  : 	mCircleFactory->Render();

	mov	ecx, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	call	?Render@CircleFactory@@QAEHXZ		; CircleFactory::Render

; 177  : 
; 178  : 	BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backbuffer, 0, 0, SRCCOPY);

	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	mov	ecx, DWORD PTR _hdc$[ebp]
	push	13369376				; 00cc0020H
	push	0
	push	0
	push	eax
	push	768					; 00000300H
	push	1024					; 00000400H
	push	0
	push	0
	push	ecx
	call	ebx
	pop	edi
	pop	esi

; 179  : 
; 180  : 	return 0;

	xor	eax, eax
	pop	ebx

; 181  : }

	pop	ebp
	ret	4
?Render@Application@@QAEHPAUHDC__@@@Z ENDP		; Application::Render
_TEXT	ENDS
PUBLIC	?DrawCircle@@YAHHHHH@Z				; DrawCircle
EXTRN	__imp__Arc@36:NEAR
EXTRN	__imp__CreatePen@12:NEAR
_TEXT	SEGMENT
_locx$ = 8
_locy$ = 12
_radius$ = 16
_color$ = 20
_oldpen$54035 = 20
?DrawCircle@@YAHHHHH@Z PROC NEAR			; DrawCircle

; 186  : {

	push	ebp
	mov	ebp, esp

; 187  : 	if (backbuffer)

	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	test	eax, eax
	je	SHORT $L55783

; 188  : 	{
; 189  : 		HPEN pen = CreatePen(PS_SOLID, 0, color);

	mov	eax, DWORD PTR _color$[ebp]
	push	ebx
	push	esi
	push	edi
	push	eax
	push	0
	push	0
	call	DWORD PTR __imp__CreatePen@12

; 190  : 		HGDIOBJ oldpen = SelectObject(backbuffer, pen);

	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	mov	edi, DWORD PTR __imp__SelectObject@8
	mov	esi, eax
	push	esi
	push	ecx
	call	edi

; 191  : 
; 192  : 		int left, top, right, bottom;
; 193  : 		int	startx, starty, endx, endy;
; 194  : 
; 195  : 		left		= locx - radius;
; 196  : 		top			= locy - radius;
; 197  : 		right		= locx + radius;
; 198  : 		bottom	= locy + radius;
; 199  : 		startx	= locx;
; 200  : 		starty  = locy - radius;
; 201  : 		endx	= locx;
; 202  : 		endy  = locy - radius;

	mov	ebx, DWORD PTR _locy$[ebp]
	mov	edx, DWORD PTR _radius$[ebp]
	mov	ecx, ebx
	mov	DWORD PTR _oldpen$54035[ebp], eax

; 203  : 
; 204  : 		Arc(backbuffer, left, top, right, bottom, startx, starty, endx, endy);

	mov	eax, DWORD PTR _locx$[ebp]
	sub	ecx, edx
	push	ecx
	push	eax
	push	ecx
	add	ebx, edx
	push	eax
	push	ebx
	lea	ebx, DWORD PTR [eax+edx]
	sub	eax, edx
	mov	edx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	ebx
	push	ecx
	push	eax
	push	edx
	call	DWORD PTR __imp__Arc@36

; 205  : 	
; 206  : 		SelectObject(backbuffer, oldpen);

	mov	eax, DWORD PTR _oldpen$54035[ebp]
	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	eax
	push	ecx
	call	edi

; 207  : 		DeleteObject(pen);

	push	esi
	call	DWORD PTR __imp__DeleteObject@4
	pop	edi
	pop	esi

; 208  : 	}
; 209  : 
; 210  : 	return 0;

	xor	eax, eax
	pop	ebx

; 211  : }

	pop	ebp
	ret	0
$L55783:

; 208  : 	}
; 209  : 
; 210  : 	return 0;

	xor	eax, eax

; 211  : }

	pop	ebp
	ret	0
?DrawCircle@@YAHHHHH@Z ENDP				; DrawCircle
_TEXT	ENDS
PUBLIC	?PrintText@@YAHHHHPADZZ				; PrintText
EXTRN	_vsprintf:NEAR
_TEXT	SEGMENT
_x$ = 8
_y$ = 12
_color$ = 16
_output$ = 20
?PrintText@@YAHHHHPADZZ PROC NEAR			; PrintText

; 216  : {

	push	ebp
	mov	ebp, esp

; 217  : 	vsprintf(textbuffer, output, (char *)(&output+1));

	mov	ecx, DWORD PTR _output$[ebp]
	push	ebx
	push	esi
	lea	eax, DWORD PTR _output$[ebp+4]
	push	edi
	push	eax
	push	ecx
	push	OFFSET FLAT:?textbuffer@@3PADA		; textbuffer
	call	_vsprintf

; 218  : 	int oldcolor = SetTextColor(backbuffer, color);

	mov	edx, DWORD PTR _color$[ebp]
	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	mov	esi, DWORD PTR __imp__SetTextColor@8
	add	esp, 12					; 0000000cH
	push	edx
	push	eax
	call	esi
	mov	ebx, eax

; 219  : 	TextOut(backbuffer, x, y, textbuffer, strlen(textbuffer));

	mov	edi, OFFSET FLAT:?textbuffer@@3PADA	; textbuffer
	or	ecx, -1
	xor	eax, eax
	repne scasb
	mov	edx, DWORD PTR _x$[ebp]
	mov	eax, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	not	ecx
	dec	ecx
	push	ecx
	mov	ecx, DWORD PTR _y$[ebp]
	push	OFFSET FLAT:?textbuffer@@3PADA		; textbuffer
	push	ecx
	push	edx
	push	eax
	call	DWORD PTR __imp__TextOutA@20

; 220  : 	SetTextColor(backbuffer, oldcolor);

	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	ebx
	push	ecx
	call	esi
	pop	edi
	pop	esi

; 221  : 	return 0;

	xor	eax, eax
	pop	ebx

; 222  : }

	pop	ebp
	ret	0
?PrintText@@YAHHHHPADZZ ENDP				; PrintText
_TEXT	ENDS
PUBLIC	?DrawLine@@YAHHHHHH@Z				; DrawLine
EXTRN	__imp__MoveToEx@16:NEAR
EXTRN	__imp__LineTo@12:NEAR
_TEXT	SEGMENT
_x1$ = 8
_y1$ = 12
_x2$ = 16
_y2$ = 20
_color$ = 24
?DrawLine@@YAHHHHHH@Z PROC NEAR				; DrawLine

; 227  : {

	push	ebp
	mov	ebp, esp

; 228  : 	HPEN pen = CreatePen(PS_SOLID, 0, color);

	mov	eax, DWORD PTR _color$[ebp]
	push	ebx
	push	esi
	push	edi
	push	eax
	push	0
	push	0
	call	DWORD PTR __imp__CreatePen@12

; 229  : 	HGDIOBJ oldpen = SelectObject(backbuffer, pen);

	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	mov	edi, DWORD PTR __imp__SelectObject@8
	mov	esi, eax
	push	esi
	push	ecx
	call	edi

; 230  : 
; 231  : 	MoveToEx(backbuffer, x1, y1, NULL);

	mov	edx, DWORD PTR _y1$[ebp]
	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	mov	ebx, eax
	mov	eax, DWORD PTR _x1$[ebp]
	push	0
	push	edx
	push	eax
	push	ecx
	call	DWORD PTR __imp__MoveToEx@16

; 232  : 	LineTo(backbuffer, x2, y2);

	mov	edx, DWORD PTR _y2$[ebp]
	mov	eax, DWORD PTR _x2$[ebp]
	mov	ecx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	edx
	push	eax
	push	ecx
	call	DWORD PTR __imp__LineTo@12

; 233  : 	
; 234  : 	SelectObject(backbuffer, oldpen);

	mov	edx, DWORD PTR ?backbuffer@@3PAUHDC__@@A ; backbuffer
	push	ebx
	push	edx
	call	edi

; 235  : 	DeleteObject(pen);

	push	esi
	call	DWORD PTR __imp__DeleteObject@4
	pop	edi
	pop	esi

; 236  : 
; 237  : 	return 0;

	xor	eax, eax
	pop	ebx

; 238  : }

	pop	ebp
	ret	0
?DrawLine@@YAHHHHHH@Z ENDP				; DrawLine
_TEXT	ENDS
EXTRN	__imp__DefWindowProcA@16:NEAR
EXTRN	__imp__GetWindowLongA@8:NEAR
_TEXT	SEGMENT
_hwnd$ = 8
_msg$ = 12
_wparam$ = 16
_lparam$ = 20
?WindowProc@Application@@SGJPAUHWND__@@IIJ@Z PROC NEAR	; Application::WindowProc

; 243  : {

	push	ebp
	mov	ebp, esp
	push	ebx

; 244  : 	Application* app = (Application*)GetWindowLong(hwnd, GWL_USERDATA);

	mov	ebx, DWORD PTR _hwnd$[ebp]
	push	esi
	push	edi
	push	-21					; ffffffebH
	push	ebx
	call	DWORD PTR __imp__GetWindowLongA@8

; 245  : 
; 246  : 	switch (msg)
; 247  : 	{

	mov	ecx, DWORD PTR _msg$[ebp]
	cmp	ecx, 16					; 00000010H
	je	$L54073
	mov	edi, DWORD PTR _wparam$[ebp]
	cmp	ecx, 258				; 00000102H
	jne	SHORT $L54076

; 252  : 		}
; 253  : 
; 254  : 		case WM_CHAR:
; 255  : 		{
; 256  : 		  switch (wparam)
; 257  : 			{

	lea	esi, DWORD PTR [edi-13]
	cmp	esi, 103				; 00000067H
	ja	SHORT $L54076
	xor	edx, edx
	mov	dl, BYTE PTR $L55813[esi]
	jmp	DWORD PTR $L55814[edx*4]
$L54079:

; 258  :         case 'a':
; 259  :         case 'A':
; 260  :   	  	  mCircleFactory->SetState(CS_SHOW_ALL);

	mov	eax, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	mov	DWORD PTR [eax+4], 0
$L54076:

; 279  : 					return 0;
; 280  : 			}
; 281  : 		}
; 282  : 	} // end switch(msg)
; 283  : 
; 284  : 	return DefWindowProc(hwnd, msg, wparam, lparam);

	mov	eax, DWORD PTR _lparam$[ebp]
	push	eax
	push	edi
	push	ecx
	push	ebx
	call	DWORD PTR __imp__DefWindowProcA@16
	pop	edi
	pop	esi
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L54080:

; 279  : 					return 0;
; 280  : 			}
; 281  : 		}
; 282  : 	} // end switch(msg)
; 283  : 
; 284  : 	return DefWindowProc(hwnd, msg, wparam, lparam);

	mov	eax, DWORD PTR _lparam$[ebp]
	mov	edx, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	push	eax
	push	edi
	push	ecx
	push	ebx
	mov	DWORD PTR [edx+4], 2
	call	DWORD PTR __imp__DefWindowProcA@16
	pop	edi
	pop	esi
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L54081:

; 261  :           break;
; 262  : 	  	  case 't':
; 263  :     		case 'T':
; 264  :   	  	  mCircleFactory->SetState(CS_SHOW_RAYTRACE);
; 265  :           break;
; 266  :         case 'f':
; 267  :         case 'F':
; 268  :   	  	  mCircleFactory->SetState(CS_SHOW_FRUSTUM);

	mov	eax, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	mov	DWORD PTR [eax+4], 1

; 279  : 					return 0;
; 280  : 			}
; 281  : 		}
; 282  : 	} // end switch(msg)
; 283  : 
; 284  : 	return DefWindowProc(hwnd, msg, wparam, lparam);

	mov	eax, DWORD PTR _lparam$[ebp]
	push	eax
	push	edi
	push	ecx
	push	ebx
	call	DWORD PTR __imp__DefWindowProcA@16
	pop	edi
	pop	esi
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L54082:

; 279  : 					return 0;
; 280  : 			}
; 281  : 		}
; 282  : 	} // end switch(msg)
; 283  : 
; 284  : 	return DefWindowProc(hwnd, msg, wparam, lparam);

	mov	eax, DWORD PTR _lparam$[ebp]
	mov	edx, DWORD PTR ?mCircleFactory@Application@@0PAVCircleFactory@@A ; Application::mCircleFactory
	push	eax
	push	edi
	push	ecx
	push	ebx
	mov	DWORD PTR [edx+4], 3
	call	DWORD PTR __imp__DefWindowProcA@16
	pop	edi
	pop	esi
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L54083:

; 269  :           break;
; 270  :         case 'r':
; 271  :         case 'R':
; 272  :           mCircleFactory->SetState(CS_SHOW_RANGE_TEST);
; 273  :           break;
; 274  : 				case VK_SPACE:
; 275  : 					app->PauseApp();

	mov	cl, 1
	pop	edi
	mov	BYTE PTR [eax+1122], cl
	mov	BYTE PTR [eax+1121], cl
	pop	esi
	xor	eax, eax
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L54084:

; 276  : 					return 0;
; 277  : 				case 13:
; 278  : 					app->RunApp();

	xor	cl, cl
	pop	edi
	mov	BYTE PTR [eax+1122], cl
	mov	BYTE PTR [eax+1121], cl
	pop	esi
	xor	eax, eax
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L54073:
	pop	edi

; 248  : 		case WM_CLOSE:
; 249  : 		{
; 250  : 			app->CloseApp(true);

	mov	BYTE PTR [eax+1120], 1
	pop	esi

; 251  : 			return 0;

	xor	eax, eax
	pop	ebx

; 285  : }

	pop	ebp
	ret	16					; 00000010H
$L55814:
	DD	$L54084
	DD	$L54083
	DD	$L54079
	DD	$L54081
	DD	$L54082
	DD	$L54080
	DD	$L54076
$L55813:
	DB	0
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	1
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	2
	DB	6
	DB	6
	DB	6
	DB	6
	DB	3
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	4
	DB	6
	DB	5
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	2
	DB	6
	DB	6
	DB	6
	DB	6
	DB	3
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	6
	DB	4
	DB	6
	DB	5
?WindowProc@Application@@SGJPAUHWND__@@IIJ@Z ENDP	; Application::WindowProc
_TEXT	ENDS
PUBLIC	?id@?$ctype@G@std@@$E				; std::ctype<unsigned short>::id
EXTRN	_atexit:NEAR
_DATA	SEGMENT
COMM	??_B?1???id@?$ctype@G@std@@$D@@9@51:BYTE							; std::ctype<unsigned short>::d::`local static guard'
_DATA	ENDS
_TEXT	SEGMENT
_$E31	PROC NEAR
	mov	cl, BYTE PTR ??_B?1???id@?$ctype@G@std@@$D@@9@51 ; std::ctype<unsigned short>::d::`local static guard'
	mov	al, 1
	test	cl, al
	jne	SHORT $L55819
	or	cl, al
	mov	BYTE PTR ??_B?1???id@?$ctype@G@std@@$D@@9@51, cl ; std::ctype<unsigned short>::d::`local static guard'
$L55819:
	push	OFFSET FLAT:?id@?$ctype@G@std@@$E	; std::ctype<unsigned short>::id
	call	_atexit
	pop	ecx
	ret	0
_$E31	ENDP
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
_$E18	PROC NEAR
	mov	ecx, OFFSET FLAT:__Ios_init
	call	??0Init@ios_base@std@@QAE@XZ		; std::ios_base::Init::Init
	push	OFFSET FLAT:_$E16
	call	_atexit
	pop	ecx
	ret	0
_$E18	ENDP
_TEXT	ENDS
EXTRN	??1Init@ios_base@std@@QAE@XZ:NEAR		; std::ios_base::Init::~Init
_TEXT	SEGMENT
_$E16	PROC NEAR
	mov	ecx, OFFSET FLAT:__Ios_init
	jmp	??1Init@ios_base@std@@QAE@XZ		; std::ios_base::Init::~Init
_$E16	ENDP
_TEXT	ENDS
EXTRN	??0_Winit@std@@QAE@XZ:NEAR			; std::_Winit::_Winit
_BSS	SEGMENT
	ALIGN	4

__Wios_init DB	01H DUP (?)
_BSS	ENDS
_TEXT	SEGMENT
_$E24	PROC NEAR
	mov	ecx, OFFSET FLAT:__Wios_init
	call	??0_Winit@std@@QAE@XZ			; std::_Winit::_Winit
	push	OFFSET FLAT:_$E22
	call	_atexit
	pop	ecx
	ret	0
_$E24	ENDP
_TEXT	ENDS
EXTRN	??1_Winit@std@@QAE@XZ:NEAR			; std::_Winit::~_Winit
_TEXT	SEGMENT
_$E22	PROC NEAR
	mov	ecx, OFFSET FLAT:__Wios_init
	jmp	??1_Winit@std@@QAE@XZ			; std::_Winit::~_Winit
_$E22	ENDP
_TEXT	ENDS
END
