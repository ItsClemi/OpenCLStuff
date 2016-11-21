#pragma once

#define INDEX(M,x,y) ((y)*((M).width)+(x))

class Matrix
{
public:
	~Matrix( );

	Matrix( );
	Matrix( int w, int h );
	Matrix( const Matrix& m );

	Matrix& operator=( const Matrix& m );

	Matrix operator*( const Matrix& m );

	cl_float& operator[]( int index )
	{
		if( index >= 0 && index < width*height )
		{
			return data[ index ];
		}
		
		return dummy;
	}


	cl_float& Elem( int ix, int iy )
	{
		int index = INDEX( *this, ix, iy );
		return ( *this )[ index ];
	}

public:
	inline cl_int GetWidth( )
	{
		return width;
	}

	inline cl_int GetHeight( )
	{
		return height;
	}


private:
	cl_int width;
	cl_int height;
	cl_float* data;

	static cl_float dummy;
	static int useGPU;

};

