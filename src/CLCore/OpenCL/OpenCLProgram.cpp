#include "stdafx.h"
#include "OpenCLProgram.h"

#include "OpenCLManager.h"


COpenCLProgram::COpenCLProgram( ICLManager* pManager )
	: m_pManager( pManager )
{ 
}

COpenCLProgram::~COpenCLProgram( )
{
	if( m_pKernel )
	{
		clReleaseKernel( m_pKernel );
	}

	if( m_pProgram )
	{
		clReleaseProgram( m_pProgram );
	}
}

bool COpenCLProgram::CreateProgram( const char* szFile, const char* szKernel )
{
	std::vector< char > vecBuffer;
	if( !LoadFromFile( std::move( szFile ), vecBuffer ) )
	{
		return false;
	}

	cl_int status;
	const char* szBuffer = &vecBuffer[ 0 ];
	const size_t nBufferSize = { vecBuffer.size( ) - 1 };

	m_pProgram = clCreateProgramWithSource( GetManager( )->GetContext( ), 1, &szBuffer, &nBufferSize, &status );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " Error: creating OpenCL program " << std::hex << status << std::endl;
		return false;
	}

	status = clBuildProgram( m_pProgram, 1, GetManager( )->GetTargetDevicesPtr( ), nullptr, nullptr, nullptr );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout 
			<< __FUNCTION__ << " === build failed ===" << std::endl 
			<< " Status: " << std::hex << status << std::endl
			<< GetBuildErrorMsg( );

		return false;
	}

	m_pKernel = clCreateKernel( m_pProgram, szKernel, &status );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " Error: creating kernel " << szKernel << " " << std::hex << status << std::endl;
		return false;
	}

	m_szProgramName = szKernel;

	return true;
}

bool COpenCLProgram::LoadFromFile( const char* szFile, std::vector< char >& vecBuffer )
{
	std::fstream f( szFile, std::fstream::in | std::fstream::binary );

	if( !f.is_open( ) )
	{
		std::cout << __FUNCTION__ << " failed to open file " << szFile;
		return false;
	}

	f.seekg( 0, std::fstream::end );
	auto size = f.tellg( );
	f.seekg( 0, std::fstream::beg );

	vecBuffer.resize( size.seekpos( ) + 1 );
	{
		f.read( &vecBuffer[ 0 ], size );
	}
	vecBuffer[ size ] = '\0';

	return true;
}

std::string COpenCLProgram::GetBuildErrorMsg( )
{
	cl_int status;

	std::string szErrorBuffer;

	size_t nStrLen;
	status = clGetProgramBuildInfo( GetProgram( ), const_cast< cl_device_id >( GetManager( )->GetTargetDevice( ) ), CL_PROGRAM_BUILD_LOG, 0, nullptr, &nStrLen );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " failed to get clGetProgramBuildInfo length status: " << std::hex << status << std::endl;
		return std::string( );
	}

	szErrorBuffer.resize( nStrLen + 1 );

	status = clGetProgramBuildInfo( m_pProgram, GetManager( )->GetTargetDevice( ), CL_PROGRAM_BUILD_LOG, szErrorBuffer.length( ), const_cast< char* >( szErrorBuffer.c_str( ) ), nullptr );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " failed to get clGetProgramBuildInfo " << std::hex << status << std::endl;
		return std::string( );
	}

	return szErrorBuffer;
}

