/**********************************************************************
Copyright 2013 Advanced Micro Devices, Inc. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
********************************************************************/

// Kernel code
// Matrices are stored in row-major order:
// M(row, col) = *(M.elements + row * M.width + col)
typedef struct
{
	int width;
	int height;
	int stride;	//> not always width?

	__global float* elements;
} Matrix;

// Thread block size
#define BLOCK_SIZE 16

// Get a matrix element
float GetElement( const Matrix A, int row, int col )
{
	return A.elements[ row * A.stride + col ];
}

// Set a matrix element
void SetElement( Matrix A, int row, int col, float value )
{
	A.elements[ row * A.stride + col ] = value;
}


// Get the BLOCK_SIZExBLOCK_SIZE sub-matrix Asub of A that is
// located col sub-matrices to the right and row sub-matrices down
// from the upper-left corner of A
Matrix GetSubMatrix( Matrix A, int row, int col )
{
	Matrix Asub;
	{
		Asub.width = BLOCK_SIZE;
		Asub.height = BLOCK_SIZE;
		Asub.stride = A.stride;
		Asub.elements = &A.elements[ A.stride * BLOCK_SIZE * row + BLOCK_SIZE * col ];
	}

	return Asub;
}
__kernel void MatMulKernel(
	int Awidth, int Aheight, __global float* Aelements,
	int Bwidth, int Bheight, __global float* Belements,
	int Cwidth, int Cheight, __global float* Celements
)
{
	Matrix A = { Awidth, Aheight, Awidth, Aelements };
	Matrix B = { Bwidth, Bheight, Bwidth, Belements };
	Matrix C = { Cwidth, Cheight, Cwidth, Celements };

	__local float As[ BLOCK_SIZE ][ BLOCK_SIZE ];
	__local float Bs[ BLOCK_SIZE ][ BLOCK_SIZE ];

	const int blockCol = get_group_id( 0 );
	const int blockRow = get_group_id( 1 );

	Matrix Csub = GetSubMatrix( C, blockRow, blockCol );

	const int col = get_local_id( 0 );
	const int row = get_local_id( 1 );

	float Cvalue = 0.0f;

	const int blocks = A.width / BLOCK_SIZE;
	for( int i = 0; i < blocks; i++ )
	{
		//> get sub matrices
		const Matrix Asub = GetSubMatrix( A, blockRow, i );
		const Matrix Bsub = GetSubMatrix( B, i, blockCol );

		As[ row ][ col ] = GetElement( Asub, row, col );
		Bs[ row ][ col ] = GetElement( Bsub, row, col );

		barrier( CLK_LOCAL_MEM_FENCE );

		for( int it = 0; it < BLOCK_SIZE; it++ )
		{
			Cvalue += As[ row ][ it ] * Bs[ it ][ col ];
		}

		barrier( CLK_LOCAL_MEM_FENCE );
	}


	SetElement( Csub, row, col, Cvalue );
}



