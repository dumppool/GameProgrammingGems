#include "parallel_thread.h"

// -- LOCAL

// .. REFERENCES


#if PLATFORM_Stub
    #include "parallel_thread_stub_members.hpp"
#elif PLATFORM_Pc
    #include "parallel_thread_pc_members.hpp"
#endif
