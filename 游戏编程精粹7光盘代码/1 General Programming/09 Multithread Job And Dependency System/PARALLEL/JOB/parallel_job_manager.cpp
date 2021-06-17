#include "parallel_job_manager.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel.h"
#include "parallel_job.h"
#include "parallel_job_function.h"
#include "parallel_job_handle.h"
#include "parallel_job_working_thread.h"
#include "parallel_platform.h"
#include "parallel_section.h"

// -- PUBLIC

// .. SPECIFICS

#if PARALLEL_PLATFORM_Singlethread
    #include "parallel_job_manager_singlethread.hpp"
#elif PARALLEL_PLATFORM_Multithread
    #include "parallel_job_manager_multithread.hpp"
#endif

// .. META

PARALLEL_JOB_MANAGER
    * PARALLEL_JOB_MANAGER::Instance = 0;