#pragma once


class COpenCLProgram : public ICLProgram
{
public:
	COpenCLProgram( ICLManager* pManager );
	virtual ~COpenCLProgram( );

public:
	bool CreateProgram( const char* szFile, const char* szKernel );

private:
	bool LoadFromFile( const char* szFile, std::vector< char >& vecBuffer );

protected:
	virtual const cl_context GetContext( ) const override
	{
		return m_pManager->GetContext( );
	}

	virtual const cl_command_queue GetCommandQueue( ) const override
	{
		return m_pManager->GetCommandQueue( );
	}

	virtual const cl_kernel GetKernel( ) const override
	{
		return m_pKernel;
	}

	virtual const char*	GetProgramName( ) const override
	{
		return m_szProgramName.c_str( );
	}

private:
	std::string	GetBuildErrorMsg( );


private:
	inline ICLManager*	GetManager( ) const { return m_pManager; }
	inline cl_program	GetProgram( ) const { return m_pProgram; }


private:
	ICLManager*		m_pManager;
	cl_kernel		m_pKernel = nullptr;
	cl_program		m_pProgram = nullptr;
	std::string		m_szProgramName;

};