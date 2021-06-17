#include "parallel_dependency_manager.h"
#include "parallel_job_manager.h"

PARALLEL_DEPENDENCY_IDENTIFIER
   PreRenderIdentifier,
   AnimationSystemIdentifier;

int AnimationResult[ 10 ];

void MultithreadComputeAnimation1( void * context )
{
	AnimationResult[ int( context ) ] = int( context );
	Sleep( DWORD( 2.0 * ( double( rand() ) /
		double( RAND_MAX ) ) ) );
	printf( "Animation1 %i\n", (int)context );
}

void MultithreadComputeAnimation2( void * context )
{
	AnimationResult[ (int)context ] *= AnimationResult[ (int)context ];
	Sleep( DWORD( 2 * ( (double) rand() /
		(double) RAND_MAX ) ) );
	printf( "Animation2 %i %i\n",(int)context, AnimationResult[ (int)context ] );
}

void ComputeAnimation()
{

	PARALLEL_JOB_HANDLE
		first_job,
		second_job;

	for( int i =0; i< 10; i++ )
	{
		first_job = PARALLEL_JOB_MANAGER_CreateJob( MultithreadComputeAnimation1, (void*)i );
		second_job = PARALLEL_JOB_MANAGER_CreateJob( MultithreadComputeAnimation2, (void*)i );

		PARALLEL_DEPENDENCY_MANAGER_AddDependency( first_job, second_job );
		PARALLEL_DEPENDENCY_MANAGER_AddDependency( second_job, AnimationSystemIdentifier );

		PARALLEL_JOB_MANAGER_ScheduleJob( second_job );
		PARALLEL_JOB_MANAGER_ScheduleJob( first_job );
	}
}

void Render()
{
	printf( "Render waiting for job to finish\n" );
	PARALLEL_DEPENDENCY_MANAGER_WaitForDependency( PreRenderIdentifier );
	printf( "Job are finished\n" );
}

int main( int argc, char* argv[] )
{
	PreRenderIdentifier = PARALLEL_DEPENDENCY_MANAGER_CreateDependencyEntry( "PreRender" );
	AnimationSystemIdentifier = PARALLEL_DEPENDENCY_MANAGER_CreateDependencyEntry( "Animation" );
	PARALLEL_DEPENDENCY_MANAGER_AddStaticDependency( AnimationSystemIdentifier, PreRenderIdentifier );
	PARALLEL_JOB_MANAGER::GetInstance().Initialize();

	while( !(GetKeyState( 'X' ) & 0x80 ) )
	{
		ComputeAnimation();
		Render();
	}

    return 0;
}