#include "stdafx.h"
#include "Matrix.h"

#include "OpenCLMgr.h"

#define INDEX(M,x,y) ((y)*((M).width)+(x))

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

Matrix Matrix::operator+( const Matrix& m )
{
	cl_int status;

	if( m.width == width && m.height == height )
	{
		Matrix result( width, height );

		if( !OpenCLMgr::GetInstance( )->IsGPUAvailable( ) )
		{
			// use CPU
			for( int i = 0; i < width*height; i++ )
			{
				result.data[ i ] = data[ i ] + m.data[ i ];
			}

			return result;
		}
		else	// use GPU
		{
			auto pContext = OpenCLMgr::GetInstance( )->GetContext( );
			auto pKernel = OpenCLMgr::GetInstance( )->GetKernel( );
			auto pCommandQueue = OpenCLMgr::GetInstance( )->GetCommandQueue( );


			cl_mem pAElements = clCreateBuffer( pContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof( float ) * width * height, data, NULL );
			cl_mem pBElements = clCreateBuffer( pContext, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof( float ) * m.width * m.height, m.data, nullptr );
			cl_mem pCElements = clCreateBuffer( pContext, CL_MEM_WRITE_ONLY, sizeof( float ) * result.width * result.height, nullptr, nullptr );

			clSetKernelArg( pKernel, 0, sizeof( cl_int ), &width );
			clSetKernelArg( pKernel, 1, sizeof( cl_int ), &height );
			clSetKernelArg( pKernel, 2, sizeof( cl_mem ), &pAElements );

			clSetKernelArg( pKernel, 3, sizeof( cl_int ), &m.width );
			clSetKernelArg( pKernel, 4, sizeof( cl_int ), &m.height );
			clSetKernelArg( pKernel, 5, sizeof( cl_mem ), &pBElements );

			clSetKernelArg( pKernel, 6, sizeof( cl_int ), &result.width );
			clSetKernelArg( pKernel, 7, sizeof( cl_int ), &result.height );
			clSetKernelArg( pKernel, 8, sizeof( cl_mem ), &pCElements );

			size_t wX = 16 + ( ( static_cast< size_t >( width ) >> 4 ) * 16 );
			size_t wY = 16 + ( ( static_cast< size_t >( height ) >> 4 ) * 16 );

			size_t global_work_size[ 2 ] = { wX, wY };
			size_t local_work_size[ 2 ] = { 16, 16 };

			status = clEnqueueNDRangeKernel( pCommandQueue, pKernel, 2, nullptr, global_work_size, local_work_size, 0, nullptr, nullptr );


			status = clEnqueueReadBuffer( pCommandQueue, pCElements, CL_TRUE, 0, result.width * result.height * sizeof( cl_float ), result.data, 0, nullptr, nullptr );

			status = clReleaseMemObject( pCElements );
			status = clReleaseMemObject( pBElements );
			status = clReleaseMemObject( pAElements );

			return result;
		}
	}
	else
	{
		return Matrix( );
	}
}

cl_float& Matrix::operator[]( int index )
{
	if( index >= 0 && index < width*height )
	{
		return data[ index ];
	}
	else
	{
		return dummy;
	}
}


cl_float& Matrix::Elem( int ix, int iy )
{
	int index = INDEX( *this, ix, iy );
	return ( *this )[ index ];
}
