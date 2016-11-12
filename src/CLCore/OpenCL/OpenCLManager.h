#pragma once

#ifndef CL_SUCCEEDED
#define CL_SUCCEEDED( x )  ( x == CL_SUCCESS )
#endif

class COpenCLManager : public ICLManager
{
public:
	COpenCLManager( );
	virtual ~COpenCLManager( );

protected:
	virtual bool Initialize( ) override;

	virtual bool CreateProgram( const char* szFile, const char* szKernel, ICLProgram** pProgramOut ) override;

protected:
	virtual ICLProgram*	GetProgram( const char* szKernel )
	{
		for( auto i : m_vecProgram )
		{
			if( strcmp( i->GetProgramName( ), szKernel ) == 0 )
			{
				return i;
			}
		}

		return nullptr;
	}

	virtual const cl_context GetContext( ) const override
	{
		return m_pContext;
	}

	virtual const cl_command_queue GetCommandQueue( ) const override
	{
		return m_pCommandQueue;
	}

	virtual const cl_device_id GetTargetDevice( ) const override
	{
		return reinterpret_cast< cl_device_id >( m_pDevices[ m_nDeviceNo ] );
	}

	virtual const cl_device_id* GetTargetDevicesPtr( ) const override
	{
		return reinterpret_cast< cl_device_id* >( m_pDevices + m_nDeviceNo ); // ?
	}

protected:
	virtual const bool IsGPUAvailable( ) const override
	{
		return m_bUseGPU;
	}


private:
	cl_uint				m_nDeviceNo = 1;
	cl_device_id*		m_pDevices = nullptr;

	cl_platform_id*		m_pPlatformBuffer = nullptr;

	cl_context			m_pContext = nullptr;
	cl_command_queue	m_pCommandQueue = nullptr;
	cl_program			m_pProgram = nullptr;

	cl_kernel			m_pKernel = nullptr;

	bool				m_bUseGPU = false;

	std::vector< ICLProgram* >		m_vecProgram;
};