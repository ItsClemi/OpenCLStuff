#include "stdafx.h"
#include "OpenCLMgr.h"

#include "Matrix.h"

int main( )
{
	try
	{
		OpenCLMgr::GetInstance( )->Init( "Matrix_Kernel.cl", "MatAddKernel" );
	}
	catch( const std::exception& e )
	{
		std::cout << "failed to initialize kernel!" << std::endl << e.what( ) << std::endl;
		system( "pause" );
	}


	Matrix A( 500, 500 );
	Matrix B( 500, 500 );
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


	return 0;
}

