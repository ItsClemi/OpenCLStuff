#include "stdafx.h"
#include "Matrix.h"


int main()
{
	InitializeSystem( );

	ICLProgram* pProgram;
	if( !GetCLManager( )->Initialize( ) || !GetCLManager( )->CreateProgram( "Matrix_Kernel.cl", "MatMulKernel", &pProgram ) )
	{
		_gettch( );
		return EXIT_FAILURE;
	}

	cl_int nSize = 16;
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


	C = A * B;

	for( int x = 0; x < C.GetWidth( ); x++ )
	{
		for( int y = 0; y < C.GetHeight( ); y++ )
		{
			std::cout << C.Elem( x, y ) << " ";
		}

		std::cout << std::endl;
	}

    return 0;
}

