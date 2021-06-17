//-------------------------------------------------------------------
//
// Deferred function invocation system - for sytems with 32bit GPRs
// by Mark Jawad (mark.jawad@gmail.com)
//
//
// This software allows you to store function addresses + up to 4
// arguments to call them with. You can then process the list which
// will go through and call the stored functions (with the
// arguments you specified).
//
//
// The reason we need this is as follows. When an interrupt occurs,
// the OS or game system SDK will call your registered callback.
// Inside of there we might need to do lots of work. But because
// the callbacks are executing from within the interrupt handler,
// further interrupts are diabled until the callback (and hence the
// interrupt handler) return. So, if we're doing a lot of work 
// (which could take lots of time), no other interrupts will get 
// serviced, and the system may become unstable. 
//
// That is bad, and we wish to avoid it.
//
// The solution is to queue up the parameters that were sent to the
// callback, plus a function pointer which we can call that will
// handle the parameters. We then schedule a known time during game
// processing where we can take care of it. The queue needs to be 
// double-buffered so that we can continue to queue up incoming 
// callbacks while processing a queue. The net result is that these
// async events come in at random times, and we just dump the 
// necessary info into the deferred proc list and then get out of
// the interrupt handler. Once per frame, we deal with whatever 
// got queued up. It's nice and clean, and helps eliminate 
// hard-to-track-down timing bugs.
//
//
// Other use: on handhelds, we use it to schedule functions that need
// to get called during VBlank. These usually transfer data to VRAM.
// Instead of doing lots of "if" tests to determine what work needs
// to be done, we just run through the list and dispatch whatever
// was queued. That's good, because retrace time is limited and we
// need to make the most of it. See the article for more details.
//
//-------------------------------------------------------------------
#ifndef	_DEFERRED_PROC_H_INCLUDED_
#define	_DEFERRED_PROC_H_INCLUDED_


//
// Terminology:
//	"arg" / "argument" - 	a 32-bit parameter that is passed to a
//							function via a register. 
//
// [DA] - DirectArg
//		function call where up to 4 args are loaded into
//		the appropriate registers, then the function is called.
//
//
// [IA] - IndirectArg 
//		function call which receives 1 argument; this arg is a pointer
//		to a data block where the rest of the necessary data is stored.
//
//
// Note: Each Arg is the size of a native register -- 32 bits, in
//       our case.
//
//



//-------------------------------------
// Typedefs for the API
//-------------------------------------
typedef void *			pointer;
typedef unsigned int	native_word; // could also potentially alias size_t, if available.

typedef pointer (*pfnMemoryAllocator)(native_word numBytes);


typedef struct
{
	native_word *curPtr;			// next native_word in our list that we'll be writing to
	native_word *bufferEnd;			// 1 byte past the end of our working buffer
	native_word *bufferStart;		// beginning (0th word) of our working buffer

} _dfp_block_, *dfpPtr;


extern dfpPtr		dfpCreate(pfnMemoryAllocator allocator, const native_word numWordsForProcess);
extern void 		dfpProcessAndClear(dfpPtr list);


extern void			dfpAddFuncCall(dfpPtr list, const pointer func);
extern void			dfpAddFuncCall1DA(dfpPtr list, const pointer func, const native_word arg0);
extern void			dfpAddFuncCall2DA(dfpPtr list, const pointer func, const native_word arg0, const native_word arg1);
extern native_word* dfpAddFuncCall34DA(dfpPtr list, const pointer func, const native_word numArgs);

// Note: numBytes is rounded up to a multiple of 4
extern pointer		dfpAddFuncCallIA(dfpPtr list, const pointer func, const native_word numBytesForDataBlock);



//-------------------------------------
// Handy macros - use these to call
// the 3 and 4 arg DA versions
//-------------------------------------
#define dfpAddFuncCall3DA(list, func, a1, a2, a3)		\
	{													\
		native_word *args = dfpAddFuncCall34DA(list, func, 3);	\
		args[0] = (native_word)a1;								\
		args[1] = (native_word)a2;								\
		args[2] = (native_word)a3;								\
	}


#define dfpAddFuncCall4DA(list, func, a1, a2, a3, a4 )	\
	{													\
		native_word *args = dfpAddFuncCall34DA(list, func, 4);	\
		args[0] = (native_word)a1;								\
		args[1] = (native_word)a2;								\
		args[2] = (native_word)a3;								\
		args[3] = (native_word)a4;								\
	}


//-------------------------------------------------------------------
// Notes
//-------------------------------------------------------------------


// dfpCreate
/////////////////////////////////////////////////////////////////////
// Create a deferred processing list. numWordsForProcess is the 
// maximum number of 32-bit words that the list can hold, not
// including the 3 words that make up the instance's header. Each
// function call takes 2 words, plus extra words for parameters
// or aux data blocks. 
//
// There is no corresponding Destroy, since no real cleanup is
// necessary. As long as you're not going to call any more dfp* 
// functions, you can simply reclaim your buffer and move on.
//
//


// dfpProcessAndClear
/////////////////////////////////////////////////////////////////////
// Call any functions (with their stored parameters) in the list, 
// then wipe the list and begin fresh. Not thread / interrupt safe.
// 
// NOTE: So don't attempt to add anything to the list while it's
//       processing. It'll simply be discarded! This is why we must
//		 double buffer it when using the system to capture callback
//		 handlers + parameters.
//


// dfpAddFuncCall
/////////////////////////////////////////////////////////////////////
// Add a function call with no arguments


// dfpAddFuncCall1DA
// dfpAddFuncCall2DA
// dfpAddFuncCall3DA (macro)
// dfpAddFuncCall4DA (macro)
/////////////////////////////////////////////////////////////////////
// Add a DA-style function call with between 1 and 4 parameters


// dfpAddFuncCall34DA
/////////////////////////////////////////////////////////////////////
// Add a DA-style function call with 3 or 4 args. Returns a pointer
// where the arguments are to be stored to.
//
// Use the dfpAddFuncCall3DA and dfpAddFuncCall4DA macros instead
// for consistancy when coding.


// dfpAddFuncCallIA 
/////////////////////////////////////////////////////////////////////
// Add an IA-style function call. Returns a pointer to the data block.
// Store what you need there; the address of the data block will be
// passed to the function as it's only arg. The data in this buffer
// will be lost after dfpProcessAndClear returns.



//_DEFERRED_PROC_H_INCLUDED_
#endif
