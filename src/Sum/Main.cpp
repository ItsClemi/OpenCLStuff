#include "stdafx.h"
#include "Sum.h"

int main()
{
	InitializeSystem( );

	ICLProgram* pProgram;
	if( !GetCLManager( )->Initialize( ) || !GetCLManager( )->CreateProgram( "Sum_Kernel.cl", "summe_kernel", &pProgram ) )
	{
		_gettch( );
		return EXIT_FAILURE;
	}


	std::cout << "Size of array?" << std::endl;

	size_t nSize;
	std::cin >> nSize;

	cl_int* pNumbers = new cl_int[ nSize ];

	for( cl_int i = 0; i < nSize; i++ )
	{
		pNumbers[ i ] = 1;
	}

	cl_int nResult = sum( pNumbers, nSize, pProgram );

	std::cout << "input " << nSize << " result " << nResult << std::endl;

	delete[ ] pNumbers;

	_gettch( );
    return 0;
}

