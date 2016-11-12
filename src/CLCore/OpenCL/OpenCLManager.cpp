#include "stdafx.h"
#include "OpenCLManager.h"
#include "OpenCLProgram.h"


COpenCLManager::COpenCLManager( )
{
}

COpenCLManager::~COpenCLManager( )
{
	for( auto i : m_vecProgram )
	{
		SafeDelete( i );
	}
	m_vecProgram.clear( );


	if( m_pCommandQueue )
	{
		clReleaseCommandQueue( m_pCommandQueue );
	}

	if( m_pContext )
	{
		clReleaseContext( m_pContext );
	}

	SafeDeleteArray( m_pPlatformBuffer );
	SafeDeleteArray( m_pDevices );
}

bool COpenCLManager::Initialize( )
{
	// Getting platforms and choose an available one.
	cl_uint numPlatforms;

	cl_int status = clGetPlatformIDs( 0, nullptr, &numPlatforms );

	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " Error: Getting platforms!" << std::endl;
		return false;
	}

	cl_platform_id platform = nullptr;
	if( numPlatforms > 0 )
	{
		m_pPlatformBuffer = new cl_platform_id[ numPlatforms ];

		status = clGetPlatformIDs( numPlatforms, m_pPlatformBuffer, nullptr );

		if( !CL_SUCCEEDED( status ) )
		{
			std::cout << __FUNCTION__ << " Error: Getting platforms ids " << std::hex << status << std::endl;
			return false;
		}

		platform = m_pPlatformBuffer[ 0 ];
	}

	// Query devices and choose a GPU device if has one. Otherwise use the CPU as device.
	cl_uint	numDevices = 0;
	status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 0, nullptr, &numDevices );

	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " Error: Getting device ids " << std::hex << status << std::endl;
		return false;
	}


	if( numDevices == 0 )
	{
		std::cout << "No GPU device available." << std::endl;
		std::cout << "Choose CPU as default device." << std::endl;

		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, 0, nullptr, &numDevices );
		if( !CL_SUCCEEDED( status ) )
		{
			std::cout << __FUNCTION__ << " Error: Getting number of cpu devices " << std::hex << status << std::endl;
			return false;
		}

		m_pDevices = new cl_device_id[ numDevices ];

		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_CPU, numDevices, m_pDevices, nullptr );
		if( !CL_SUCCEEDED( status ) )
		{
			std::cout << __FUNCTION__ << " Error: Getting cpu device id " << std::hex << status << std::endl;
			return false;
		}
	}
	else
	{
		m_pDevices = new cl_device_id[ numDevices ];

		status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, numDevices, m_pDevices, nullptr );
		if( !CL_SUCCEEDED( status ) )
		{
			std::cout << __FUNCTION__ << " Error: Getting gpu device id " << std::hex << status << std::endl;
			return false;
		}

		m_bUseGPU = true;
	}

	if( m_nDeviceNo >= numDevices )
	{
		m_nDeviceNo = 0;
	}

	// Create context
	m_pContext = clCreateContext( nullptr, 1, m_pDevices, nullptr, nullptr, nullptr );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " Error: creating OpenCL context " << std::hex << status << std::endl;
		return false;
	}

	m_pCommandQueue = clCreateCommandQueue( m_pContext, m_pDevices[ m_nDeviceNo ], CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE | CL_QUEUE_PROFILING_ENABLE, &status );
	if( !CL_SUCCEEDED( status ) )
	{
		std::cout << __FUNCTION__ << " Error: creating command queue " << std::hex << status << std::endl;
		return false;
	}

	return true;
}

bool COpenCLManager::CreateProgram( const char* szFile, const char* szKernel, ICLProgram** pProgramOut )
{
	COpenCLProgram* pProgram = new COpenCLProgram( this );
	{
		if( !pProgram->CreateProgram( szFile, szKernel ) )
		{
			SafeDelete( pProgram );

			return false;
		}

		m_vecProgram.push_back( pProgram );
	}

	if( pProgramOut != nullptr )
	{
		*pProgramOut = pProgram;
	}

	return true;
}

