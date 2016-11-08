#include "stdafx.h"
#include "OpenCLMgr.h"

#include <Common/CommonHelper.h>



OpenCLMgr::OpenCLMgr( )
{ }

OpenCLMgr::~OpenCLMgr( )
{
	cl_int status;

	if( m_pKernel )
	{
		status = clReleaseKernel( m_pKernel );
	}

	if( m_pProgram )
	{
		status = clReleaseProgram( m_pProgram );
	}

	if( m_pCommandQueue )
	{
		status = clReleaseCommandQueue( m_pCommandQueue );
	}

	if( m_pContext )
	{
		status = clReleaseContext( m_pContext );
	}

	SafeDeleteArray( m_pPlatformBuffer );
	SafeDeleteArray( m_pDevices );
}


OpenCLMgr* OpenCLMgr::GetInstance( )
{
	static OpenCLMgr mng;

	return &mng;
}

void OpenCLMgr::Init( const std::string&& szFile, const std::string&& szKernel )
{
	InitializeOpenCL( );

	std::vector< char > vecBuffer;
	LoadFromFile( std::move( szFile ), vecBuffer );

	cl_int status;
	const char* szBuffer = &vecBuffer[ 0 ];
	const size_t nBufferSize = { vecBuffer.size( ) - 1 };

	m_pProgram = clCreateProgramWithSource( m_pContext, 1, &szBuffer, &nBufferSize, &status );
	CheckStatus( status, "Error: creating OpenCL program" );


	status = clBuildProgram( m_pProgram, 1, m_pDevices + m_nDeviceNo, nullptr, nullptr, nullptr );
	if( status != CL_SUCCESS )
	{
		std::ostringstream os;
		{
			os << "=== build failed ===" << std::endl << GetBuildErrorMsg( );
		}
		throw std::runtime_error( os.str( ) );
	}

	m_pKernel = clCreateKernel( m_pProgram, szKernel.c_str( ), &status );
	CheckStatus( status, "Error: creating MatAddKernel kernel" );
}

void OpenCLMgr::InitializeOpenCL( )
{
	// Getting platforms and choose an available one.
	cl_uint numPlatforms;

	cl_int	status = clGetPlatformIDs( 0, nullptr, &numPlatforms );
	CheckStatus( status, "Error: Getting platforms!" );


	cl_platform_id platform = nullptr;
	if( numPlatforms > 0 )
	{
		m_pPlatformBuffer = new cl_platform_id[ numPlatforms ];

		status = clGetPlatformIDs( numPlatforms, m_pPlatformBuffer, nullptr );
		CheckStatus( status, "Error: Getting platforms ids" );

		platform = m_pPlatformBuffer[ 0 ];
	}

	// Query devices and choose a GPU device if has one. Otherwise use the CPU as device.*/
	cl_uint	numDevices = 0;
	status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices );
	CheckStatus( status, "Error: Getting device ids" );


	if( numDevices == 0 )	//no GPU available.
	{
		std::wcout << L"No GPU device available." << std::endl;
		std::wcout << L"Choose CPU as default device." << std::endl;

		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, 0, nullptr, &numDevices );
		CheckStatus( status, "Error: Getting number of cpu devices" );

		m_pDevices = new cl_device_id[ numDevices ];
		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, numDevices, m_pDevices, nullptr );

		CheckStatus( status, "Error: Getting cpu device id" );
	}
	else
	{
		m_pDevices = new cl_device_id[ numDevices ];
		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, numDevices, m_pDevices, nullptr );
		CheckStatus( status, "Error: Getting gpu device id" );

		m_bUseGPU = true;
	}

	if( m_nDeviceNo >= numDevices )
	{
		m_nDeviceNo = 0;
	}

	// Create context
	m_pContext = clCreateContext( nullptr, 1, m_pDevices, nullptr, nullptr, nullptr );
	CheckStatus( status, "Error: creating OpenCL context" );

	m_pCommandQueue = clCreateCommandQueue( m_pContext, m_pDevices[ m_nDeviceNo ], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &status );
	CheckStatus( status, "Error: creating command queue" );
}

void OpenCLMgr::LoadFromFile( const std::string&& szFile, std::vector< char >& vecBuffer )
{
	std::fstream f( szFile, std::fstream::in | std::fstream::binary );

	if( !f.is_open( ) )
	{
		throw std::runtime_error( __FUNCTION__ " failed to open file!" );
	}

	f.seekg( 0, std::fstream::end );
	auto size = f.tellg( );
	f.seekg( 0, std::fstream::beg );

	vecBuffer.resize( size.seekpos( ) + 1 );

	f.read( &vecBuffer[ 0 ], size );

	vecBuffer[ size ] = '\0';
}
