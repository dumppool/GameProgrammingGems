//-------------------------------------------------------------------
// Deferred function invocation system - for sytems with 32bit GPRs
// by Mark Jawad (mark.jawad@gmail.com)
//
// This version is targeted for ARM chips that follow the AAPCS
// calling convention. Note: our target platform doesn't make any
// special use of r9, so we're free to use it here. Make sure your 
// target system is running an identical ABI before plugging in 
// this code. If not, modifications will be required.
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


// num args is a bit field of {0, 1, 2, 4, 8} ==> 
//							  {none, 1 arg, 2 args, 3 args, 4 args}
//
// extraBytes should be a multiple of 4 (but we don't check that here)
//
#define MAKE_COMMAND(numArgs, extraBytes)	( (numArgs) | ((extraBytes)<<4) )



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


//
// Note: Each compiler has a different approach for dealing with 
// functions that contain inline assembly. This particular function
// is entirely in assembly, and as such might need to be moved to an
// assembly (.s) file. This simply depends on your environment.
//
asm void dfpProcessAndClear(register dfpPtr list)
{
//
// -=- NOTE: This is assembly code for an ARM9 CPU.		-=-
// -=-       It will not compile in THUMB mode.			-=-
// -=-		 Use ARM mode instead.						-=-
//
#define curPtr			r4
#define lastPtr			r5
#define dfpBlockPtr		r6
#define argsToLoad		r7
#define bytesToSkip		r8
#define procAddr		r9

entry:
	stmfd       sp!,{r4-r10,lr}			// store multiple (frame descending) w/ update
										// note that we store an even number of registers
										// in order to keep the stack 8byte aligned.
	
	ldr			curPtr, [list, #8]		// get start of list
	ldr			lastPtr, [list, #0]		// get end of list
	
	cmp 		curPtr, lastPtr			// compare curPtr-lastPtr
	beq			finish					// branch if curPtr == lastPtr
	
	mov			dfpBlockPtr, list		// stash away list pointer for later
	
loop:
	// 1) load packed control word + functionAddr
	// 2) then extract num args + bytes to skip from control word
	ldmia 		curPtr!, {bytesToSkip,procAddr}		// load ctrlWord + procAddr
	mov			argsToLoad, bytesToSkip, LSL #28 	// lower 4 bits are num args to load
	mov			bytesToSkip, bytesToSkip, LSR #4	// remaining bits are bytes to skip

	// argsToLoad got converted to condition bits.. move em into CPSR_FLAGS
	msr			CPSR_F, argsToLoad
	
	// now load the appropriate set of registers
	// based on the condition bits. order is: n,z,c,v
	ldmvsia		curPtr!, {r0}			// <cond>vs == v flag only (only load 1 arg)
	ldmcsia		curPtr!, {r0-r1}		// <cond>cs == c flag only (only load 2 args)
	ldmeqia		curPtr!, {r0-r2}		// <cond>eq == z flag only (only load 3 args)
	ldmmiia		curPtr!, {r0-r3}		// <cond>mi == n flag only (only load 4 args)

	add			curPtr, curPtr, bytesToSkip	// add any left over bytes
	blx 		procAddr					// make the call
	
	cmp 		curPtr, lastPtr				// set flags to result of: (curPtr - lastPtr)
	bmi			loop						// branch if (curPtr < lastPtr) ; ie, we haven't reached the end
	
finish:
	// now set curPtr back to bufferStart
	ldr 		r0, [dfpBlockPtr, #8]		// start of list
	str			r0, [dfpBlockPtr, #0]		// cur now == start of list
	ldmfd       sp!,{r4-r10,pc}				// load multiple  (frame descending) w/ update.
											// Has the side-effect of returning to the caller.
	

#undef curPtr
#undef lastPtr
#undef dfpBlockPtr
#undef procAddr
#undef bytesToSkip
#undef argsToLoad
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
