#pragma once

#include <CL/cl.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream> 


#pragma comment( lib, "OpenCL.lib" )

class OpenCLMgr
{
public:
	OpenCLMgr( );
	~OpenCLMgr( );

public:
	static OpenCLMgr* GetInstance( );

public:
	void Init( const std::string&& szFile, const std::string&& szKernel );

private:
	void InitializeOpenCL( );

	void LoadFromFile( const std::string&& szFile, std::vector< char >& vecBuffer );

private:
	inline void CheckStatus( cl_int nStatus, std::string&& szError ) const
	{
		if( nStatus != CL_SUCCESS )
		{
			throw std::runtime_error( szError );
		}
	}

private:
	inline std::string GetBuildErrorMsg( )
	{
		cl_int status;

		std::string szErrorBuffer;

		size_t nStrLen;
		status = clGetProgramBuildInfo( GetProgram( ), GetDeviceId( m_nDeviceNo ), CL_PROGRAM_BUILD_LOG, 0, nullptr, &nStrLen );
		CheckStatus( status, "failed to get clGetProgramBuildInfo length" );

		szErrorBuffer.resize( nStrLen + 1 );

		status = clGetProgramBuildInfo( m_pProgram, GetDeviceId( m_nDeviceNo ), CL_PROGRAM_BUILD_LOG, szErrorBuffer.length( ), const_cast< char* >( szErrorBuffer.c_str( ) ), nullptr );
		CheckStatus( status, "failed to get clGetProgramBuildInfo" );

		return szErrorBuffer;
	}

public:
	inline const cl_context GetContext( ) const
	{
		return m_pContext;
	}

	inline const cl_command_queue GetCommandQueue( ) const
	{
		return m_pCommandQueue;
	}

	inline const cl_program GetProgram( ) const
	{
		return m_pProgram;
	}

	inline const cl_kernel GetKernel( ) const
	{
		return m_pKernel;
	}

	inline const cl_device_id GetDeviceId( const size_t nDeviceNum ) const
	{
		return m_pDevices[ nDeviceNum ];
	}

	inline const cl_device_id GetDevice( )
	{
		return m_pDevices[ 0 ];
	}

	inline bool IsGPUAvailable( )
	{
		return m_bUseGPU;
	}

private:
	cl_uint				m_nDeviceNo = 1;
	cl_device_id*		m_pDevices = nullptr;
	
	cl_platform_id*		m_pPlatformBuffer;

	cl_context			m_pContext = nullptr;
	cl_command_queue	m_pCommandQueue = nullptr;
	cl_program			m_pProgram = nullptr;

	cl_kernel			m_pKernel = nullptr;

	bool				m_bUseGPU = false;
};