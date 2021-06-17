//-------------------------------------------------------------------
// Deferred function invocation system - for sytems with 32bit GPRs
// by Mark Jawad (mark.jawad@gmail.com)
//
// This version is targeted for PPC chips that follow the EABI
// calling convention. Not all PPC systems use the EABI calling
// convention (such as Mac OS X), so make sure you know what ABI 
// your target platform is running before trying to plug this in. 
// If different, modifications will be required.
//
//---------------------------
//
// This system was originally targeted for ARM processors. It can 
// easily be modified for other platforms, but note that there's 
// no FPU on most of the ARM chips, so we're GPR only here. If 
// you want to use floats or vector regs on other platforms, you'll
// have to extend the system (or make a copy of this with 
// appropriate enhancements).
//
//-------------------------------------------------------------------
#include "deferred_proc.h"
#include <assert.h>


//--------------------------------
// Macros for making the command 
// word used to tag the type of
// function call that comes next.
//--------------------------------

// bits that go into cr1
#define ZEROARG		0x0800
#define ONEARG		0x0400
#define TWOARG		0x0200
#define THREEARG	0x0100
#define FOURARG		0x0000


#define BYTES_FROM_WORDS(a)	((2+(a))<<2)

// Note: cmd differs greatly from ARM version!
#define MAKE_COMMAND(numArgs, totalBytes)	( ((numArgs)<<16) | (totalBytes) )



//-------------------------------------------------------------------
// 
//-------------------------------------------------------------------
dfpPtr dfpCreate(pfnMemoryAllocator allocator, const native_word numWordsForProcess)
{
	native_word numBytes = sizeof(_dfp_block_) + (numWordsForProcess * sizeof(native_word));
	
	_dfp_block_ *header = (_dfp_block_ *)allocator(numBytes);
	native_word *space = (native_word*)header;
	space += (sizeof(_dfp_block_) / sizeof(native_word) );
	
	header->bufferStart = space;
	header->curPtr = space;
	
	space += numWordsForProcess;
	header->bufferEnd = space; // if curptr >= this, we're toast!

	return(header);
}


/*
 * EPPC Stack frame:
 *
 * ---highest address---
 *	fprs									(optional; saved non-volatile)
 *  gprs									(optional; saved non-volatile)
 *  cr										(optional; saved non-volatile)
 *  local/temp storage space 				(optional)
 *  func params								(optional)
 *  padding to get us to 8 byte multiple 	(if needed)
 *  LR Save Word 							(req'd for non-leaf)
 *  Back Chain Word							(req'd for non-leaf)
 * ---lowest address---
 *
 * r1 = stack pointer; always points to the current Back Chain
 * r2, r13 also off-limits (small data anchors)
 */

//
// Note: Each compiler has a different approach for dealing with 
// functions that contain inline assembly. This particular function
// is entirely in assembly, and as such might need to be moved to an
// assembly (.s) file. This simply depends on your environment.
//
asm void dfpProcessAndClear(register dfpPtr list)
{
#define curPtr		r30
#define lastPtr		r31
#define cmdBlock	r0
#define funcPtr		ctr
#define tmp			r0
#define bytesToSkip	cmdBlock


		// do prolog (stackframe + register save)
		nofralloc							// tell the compiler that we'll do the prolog/epilog ourselves
		
		mflr 	r0
		stwu	r1, -16(r1) 				// drop lower in the stack + set our backchain 
		stw		r0, 20(r1)					// update caller's frame with the LR that is used
											// to get to said frame
		stmw	r30, 8(r1)					// save r30-r31
		
		// logic begin
		lwz	curPtr, 8(list)					// load list->bufferStart
		lwz lastPtr, 0(list)				// load list->curPtr (which is end of valid data)
		
		cmpw curPtr, lastPtr				// bail if nothing is in the list
		beq	@done
		stw curPtr, 0(list)					// reset the list; dangerous if something
											// trys to use the list from an interrupt!
		
		
@loop:	lwz  cmdBlock, 0(curPtr)
		lwz  r3, 4(curPtr)
		mtcrf	0x40, cmdBlock				// set condition code; copy bits in cr1 range directly into cr1
		mtctr	r3
		andi. bytesToSkip, cmdBlock, 0xffff	// set bytesToSkip

		// jump based on condition code TRUE (BO field = 12)
		bc 12, 4, @0		// no params
		bc 12, 5, @1		// 1 param
		bc 12, 6, @2		// 2 params
		bc 12, 7, @3		// 3 params
							// else we're 4 params
@4:		lwz r6, 20(curPtr)
@3:		lwz r5, 16(curPtr)
@2:		lwz r4, 12(curPtr)
@1:		lwz r3, 8(curPtr)
@0:		add curPtr, curPtr, bytesToSkip
		bctrl								// jump off to the deferred function
		
		cmpw curPtr, lastPtr				// are we done yet?
		bne @loop
		
		// do epilog (stackframe + register restore)
@done: 	lmw		r30, 8(r1)					// restore r30-r31
		lwz		r0, 20(r1)					// pull lr from previous backchain
		addi	r1, r1, 16					// restore the previous backchain
		mtlr	r0							// restore the lr
		blr									// go back from whence we came


#undef curPtr
#undef lastPtr
#undef cmdBlock
#undef funcPtr
#undef tmp
#undef bytesToSkip
}

//-------------------------------------------------------------------
// Add func + args to 
//-------------------------------------------------------------------
void dfpAddFuncCall(dfpPtr list, const pointer func)
{
	_dfp_block_ *header = (_dfp_block_*)list;
	native_word *cmd = header->curPtr;
	cmd[0] = MAKE_COMMAND(0, 0);
	cmd[1] = (native_word)func;
	cmd += 2;
	header->curPtr = cmd;
	assert(header->curPtr < header->bufferEnd);
}

void dfpAddFuncCall1DA(dfpPtr list, const pointer func, const native_word arg0)
{
	_dfp_block_ *header = (_dfp_block_*)list;
	native_word *cmd = header->curPtr;
	cmd[0] = MAKE_COMMAND(0x01, 0);
	cmd[1] = (native_word)func;
	cmd[2] = arg0;
	cmd += 3;
	header->curPtr = cmd;
	assert(header->curPtr < header->bufferEnd);
}

void dfpAddFuncCall2DA(dfpPtr list, const pointer func, const native_word arg0, const native_word arg1)
{
	_dfp_block_ *header = (_dfp_block_*)list;
	native_word *cmd = header->curPtr;
	cmd[0] = MAKE_COMMAND(0x02, 0);
	cmd[1] = (native_word)func;
	cmd[2] = arg0;
	cmd[3] = arg1;
	cmd += 4;
	header->curPtr = cmd;
	assert(header->curPtr < header->bufferEnd);
}

native_word* dfpAddFuncCall34DA(dfpPtr list, const pointer func, const native_word numArgs)
{
	assert( (numArgs==3) || (numArgs==4) );
	
	native_word *rc;

	_dfp_block_ *header = (_dfp_block_*)list;
	native_word *cmd = header->curPtr;
	if( numArgs == 3 )
	{
		cmd[0] = MAKE_COMMAND(0x04, 0);
	} else {
		cmd[0] = MAKE_COMMAND(0x08, 0);
	}
	cmd[1] = (native_word)func;
	cmd += 2;
	rc = cmd;
	cmd += numArgs;
	header->curPtr = cmd;
	assert(header->curPtr < header->bufferEnd);
	return(rc);	
}

// num bytes is rounded up to a multiple of 4
pointer dfpAddFuncCallIA(dfpPtr list, const pointer func, const native_word numBytes)
{
	assert(numBytes != 0);
	
	native_word *rc;

	_dfp_block_ *header = (_dfp_block_*)list;
	native_word *cmd = header->curPtr;
	native_word totalBytes = ptrRoundUp(numBytes, 4);
	cmd[0] = MAKE_COMMAND(1, totalBytes);
	cmd[1] = (native_word)func;
	cmd[2] = (native_word)&cmd[3];
	cmd += 3;
	
	rc = cmd;
	cmd += (totalBytes>>2); // since we're stepping 4 bytes at a time
	header->curPtr = cmd;
	assert(header->curPtr < header->bufferEnd);
	return((pointer)rc);
}



/*
static void test0(void)
{
	printf("hello!\n");
}
static void test1(native_word a)
{
	printf("a[%p]\n", a);
}

static void test2(native_word a, native_word b)
{
	printf("a[%p] b[%p]\n", a, b);
}

static void test3(native_word a, native_word b, native_word c)
{
	printf("a[%p] b[%p] c[%p]\n", a, b, c);
}

static void test4(native_word a, native_word b, native_word c, native_word d)
{
	printf("a[%p] b[%p] c[%p] d[%p]\n", a, b, c, d);
}

typedef struct
{
	unsigned int	v1;
	signed short	v2;
	unsigned char	v3;
	unsigned char	v4;
} _ia_test_;

static void test5(_ia_test_ *tst)
{
	printf("v1[%d] v2[%d] v3[%d] v4[%d]\n", tst->v1,
				tst->v2, tst->v3, tst->v4);
}
*/


/*
All registered company names, brand names, product names, trademarks or
tradenames are the property of their respective holders, and are used 
herein for incidental informational purposes only.
*/
