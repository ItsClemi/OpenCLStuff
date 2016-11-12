#pragma once


class Matrix
{
public:
	~Matrix( );

	Matrix( );
	Matrix( int w, int h );
	Matrix( const Matrix& m );

	Matrix& operator=( const Matrix& m );
	Matrix operator+( const Matrix& m );

	cl_float& operator[]( int index );
	cl_float& Elem( int ix, int iy );

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
	cl_float *data;

	static cl_float dummy;
	static int useGPU;

};

