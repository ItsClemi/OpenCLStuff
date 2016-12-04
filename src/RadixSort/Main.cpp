#include "stdafx.h"
#include "Radix.h"


int main( )
{
	InitializeSystem( );

	ICLProgram* pProgramScan;
	ICLProgram* pProgramSweepup;

	if( !GetCLManager( )->Initialize( ) ||
		!GetCLManager( )->CreateProgram( "Radix_Kernel.cl", "scan_kernel", &pProgramScan )
		)
	{
		_gettch( );
		return EXIT_FAILURE;
	}

	srand( GetTickCount( ) );

	size_t nNum = 256;
	cl_int* pNumbers = new cl_int[ nNum ];

	for( size_t i = 0; i < nNum; i++ )
	{
		pNumbers[ i ] = rand( );
	}


	auto pSorted = Scan( pNumbers, nNum, pProgramScan );


	cl_int* pTemp = new cl_int[ nNum ];
	ZeroMemory( pTemp, sizeof( cl_int ) * nNum );

	for( size_t i = 1; i < nNum; i++ )
	{
		pTemp[ i ] = pNumbers[ i - 1 ] + pTemp[ i - 1 ];
	}

	std::cout << memcmp( pSorted, pTemp, nNum ) << std::endl;

	

// 	int in[ ] = { 3, 1, 7, 0, 4, 1, 6, 3 };
// 	int out[ 8 ] = { };
// 	for( size_t i = 1; i < ARRAYSIZE( in ); i++ )
// 	{
// 		out[ i ] = in[ i - 1 ] + out[ i - 1 ];
// 	}
// 



	return 0;
}

