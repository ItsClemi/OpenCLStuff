#include "stdafx.h"
#include "Matrix.h"



cl_float Matrix::dummy;
int Matrix::useGPU = 1;

Matrix::Matrix( )
{
	width = height = 0;
	data = NULL;
}

Matrix::Matrix( int w, int h )
{
	width = w;
	height = h;
	data = new cl_float[ width * height ];
}

Matrix::Matrix( const Matrix& m )
{
	width = m.width;
	height = m.height;
	data = new cl_float[ width * height ];
	memcpy( data, m.data, width * height * sizeof( cl_float ) );
}

Matrix::~Matrix( )
{
	delete[ ] data;
}

Matrix& Matrix::operator=( const Matrix& m )
{
	if( this != &m )
	{
		delete[ ] data;
		width = m.width;
		height = m.height;
		data = new cl_float[ width * height ];
		memcpy( data, m.data, width*height * sizeof( cl_float ) );
	}
	return *this;
}

Matrix Matrix::operator*( const Matrix & m )
{
	cl_int status;

	if( GetWidth( ) == m.width && GetHeight( ) == m.height )
	{
		Matrix result( width, height );

		ICLProgram* pProgram = GetCLManager( )->GetProgram( "MatMulKernel" );

		cl_mem pAElements = clCreateBuffer( pProgram->GetContext( ), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof( cl_float ) * width * height, data, nullptr );
		cl_mem pBElements = clCreateBuffer( pProgram->GetContext( ), CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof( cl_float ) * m.width * m.height, m.data, nullptr );
		cl_mem pCElements = clCreateBuffer( pProgram->GetContext( ), CL_MEM_WRITE_ONLY, sizeof( cl_float ) * result.width * result.height, nullptr, nullptr );

		clSetKernelArg( pProgram->GetKernel( ), 0, sizeof( cl_int ), &width );
		clSetKernelArg( pProgram->GetKernel( ), 1, sizeof( cl_int ), &height );
		clSetKernelArg( pProgram->GetKernel( ), 2, sizeof( cl_mem ), &pAElements );

		clSetKernelArg( pProgram->GetKernel( ), 3, sizeof( cl_int ), &m.width );
		clSetKernelArg( pProgram->GetKernel( ), 4, sizeof( cl_int ), &m.height );
		clSetKernelArg( pProgram->GetKernel( ), 5, sizeof( cl_mem ), &pBElements );

		clSetKernelArg( pProgram->GetKernel( ), 6, sizeof( cl_int ), &result.width );
		clSetKernelArg( pProgram->GetKernel( ), 7, sizeof( cl_int ), &result.height );
		clSetKernelArg( pProgram->GetKernel( ), 8, sizeof( cl_mem ), &pCElements );
		
		size_t lsize = 16;
		size_t gwidth, gheight;
		gwidth = ( ( result.width + lsize - 1 ) / lsize )*lsize;
		gheight = ( ( result.height + lsize - 1 ) / lsize )*lsize;
		size_t global_work_size[ 2 ] = { gwidth, gheight };
		size_t local_work_size[ 2 ] = { lsize, lsize };

		status = clEnqueueNDRangeKernel( pProgram->GetCommandQueue( ), pProgram->GetKernel( ), 2, nullptr, global_work_size, local_work_size, 0, nullptr, nullptr );
	
		status = clEnqueueReadBuffer( pProgram->GetCommandQueue( ), pCElements, CL_TRUE, 0, result.width * result.height * sizeof( cl_float ), result.data, 0, nullptr, nullptr );

		status = clReleaseMemObject( pCElements );
		status = clReleaseMemObject( pBElements );
		status = clReleaseMemObject( pAElements );

		return result;
	}

	return Matrix( );
}



