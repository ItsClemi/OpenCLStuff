#pragma once

#include "OpenCL/OpenCLManager.h"


class CSystem
{
public:
	CSystem( );
	~CSystem( );

public:

	inline COpenCLManager* GetCLManager( )
	{
		return m_pCLManager.get( );
	}

private:
	std::unique_ptr< COpenCLManager >	m_pCLManager = std::make_unique< COpenCLManager >( );
};


extern std::unique_ptr< CSystem > gSystem;