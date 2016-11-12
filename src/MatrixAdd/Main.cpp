#include "stdafx.h"

#include "Matrix.h"

int main( )
{
	InitializeSystem( );

	if( !GetCLManager( )->Initialize( ) || !GetCLManager( )->CreateProgram( "Matrix_Kernel.cl", "MatAddKernel", nullptr ) )
	{
		_gettch( );
		return EXIT_FAILURE;
	}

	std::cout << "matrix size?" << std::endl; 
	
	int nSize = 0;
	std::cin >> nSize;

	if( nSize <= 0 )
	{
		std::cout << "invalid number!" << std::endl;
		return 0;
	}


	Matrix A( nSize, nSize );
	Matrix B( nSize, nSize );
	Matrix C;

	for( int x = 0; x < A.GetWidth( ); x++ )
	{
		for( int y = 0; y < A.GetHeight( ); y++ )
		{
			A.Elem( x, y ) = 1.0f;
		}
	}

	for( int x = 0; x < B.GetWidth( ); x++ )
	{
		for( int y = 0; y < B.GetHeight( ); y++ )
		{
			B.Elem( x, y ) = 0.0f;
		}
		B.Elem( x, x ) = 2.0f;
	}


	C = A + B;

	for( int x = 0; x < C.GetWidth( ); x++ )
	{
		for( int y = 0; y < C.GetHeight( ); y++ )
		{
			std::cout << C.Elem( x, y ) << " ";
		}

		std::cout << std::endl;
	}


	_gettch( );

	return 0;
}

