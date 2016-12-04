#include "stdafx.h"
#include "Radix.h"

cl_int* Scan( cl_int* pInput, size_t nSize, ICLProgram* pProgram )
{
	cl_int status;


	cl_mem inputBuffer = clCreateBuffer( pProgram->GetContext( ), CL_MEM_READ_ONLY, sizeof( cl_int ) * nSize, nullptr, nullptr );
	clEnqueueWriteBuffer( pProgram->GetCommandQueue( ), inputBuffer, CL_TRUE, 0, nSize * sizeof( cl_int ), pInput, 0, nullptr, nullptr );

	cl_mem outputBuffer = clCreateBuffer( pProgram->GetContext( ), CL_MEM_WRITE_ONLY, sizeof( cl_int ) * nSize, nullptr, nullptr );

	int n = static_cast< int > ( nSize );
	status = clSetKernelArg( pProgram->GetKernel( ), 0, sizeof( cl_mem ), ( void* )&inputBuffer );
	status = clSetKernelArg( pProgram->GetKernel( ), 1, sizeof( cl_mem ), ( void* )&outputBuffer );
	status = clSetKernelArg( pProgram->GetKernel( ), 2, sizeof( cl_int ), ( void* )&n );

	size_t global_work_size[ 1 ] = { nSize };
	size_t local_work_size[ 1 ] = { 256 };

	status = clEnqueueNDRangeKernel( pProgram->GetCommandQueue( ), pProgram->GetKernel( ), 1, NULL, global_work_size, local_work_size, 0, NULL, NULL );
	if( status != CL_SUCCESS )
	{
		std::cout << __FUNCTION__ << " Error: enqueuing kernel! " << std::hex << status << std::endl;
		//return status;
	}

	cl_int* pOut = new cl_int[ nSize ];

	status = clEnqueueReadBuffer( pProgram->GetCommandQueue( ), outputBuffer, CL_TRUE, 0, sizeof( cl_int ) * nSize, pOut, 0, nullptr, nullptr );
	if( status != CL_SUCCESS )
	{
		std::cout << __FUNCTION__ << " Error: reading buffer! " << std::hex << status << std::endl;
	}

	return pOut;
}
