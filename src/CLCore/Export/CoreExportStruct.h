#pragma once


__interface ICLProgram;

__interface ICLManager
{
	virtual bool Initialize( ) = 0;

	virtual bool CreateProgram( const char* szFile, const char* szKernel, ICLProgram** pProgramOut ) = 0;

	virtual ICLProgram*				GetProgram( const char* szKernel );

	virtual const cl_context		GetContext( ) const = 0;
	virtual const cl_command_queue	GetCommandQueue( ) const = 0;

	virtual const cl_device_id		GetTargetDevice( ) const = 0;
	virtual const cl_device_id*		GetTargetDevicesPtr( ) const = 0;

	virtual const bool				IsGPUAvailable( ) const = 0;
};

__interface ICLProgram
{
	virtual const cl_context		GetContext( ) const = 0;
	virtual const cl_command_queue	GetCommandQueue( ) const = 0;
	virtual const cl_kernel			GetKernel( ) const = 0;

	virtual const char*				GetProgramName( ) const = 0;
};