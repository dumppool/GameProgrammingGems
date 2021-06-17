#include "parallel_dependency_manager.h"

// -- LOCAL

// .. REFERENCES

#include "counted_object.h"
#include "parallel_dependency_group_entry.h"
#include "parallel_dependency_identifier.h"
#include "parallel_dependency_index_dispenser.h"
#include "parallel_dependency_job_entry.h"
#include "parallel_job_handle.h"
#include "parallel_platform.h"
#include "parallel_section.h"

// -- PUBLIC

// .. SPECIFICS

#if PARALLEL_PLATFORM_Singlethread
    #include "parallel_dependency_manager_singlethread.hpp"
#elif PARALLEL_PLATFORM_Multithread
    #include "parallel_dependency_manager_multithread.hpp"
#endif

PARALLEL_DEPENDENCY_MANAGER
    * PARALLEL_DEPENDENCY_MANAGER::Instance = 0;