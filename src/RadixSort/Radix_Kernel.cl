

//based on: http://www.eecs.umich.edu/courses/eecs570/hw/parprefix.pdf

__kernel void scan_kernel( __global int* in, __global int* out, int n )
{
	int gid = get_global_id( 0 );
	int lid = get_local_id( 0 );
	int groupid = get_group_id( 0 );

	__local int localArray[ 256 ];
	
	localArray[ lid ] = in[ gid ];
	barrier( CLK_LOCAL_MEM_FENCE );

	int offset = 1;
	for( int i = n >> 1; i > 0; i >>= 1 )
	{
		if( lid < i )
		{
			int ai = offset * ( ( lid << 1 ) + 1 ) - 1;
			int bi = offset * ( ( lid << 1 ) + 2 ) - 1;

			localArray[ bi ] += localArray[ ai ];
		}

		offset *= 2;

		barrier( CLK_LOCAL_MEM_FENCE );
	}

	if( lid == 0 )
	{
		localArray[ n - 1 ] = 0;
	}

	for( int i = 1; i < n; i *= 2 )
	{
		offset >>= 1;

		if( lid < i )
		{
			int ai = offset * ( ( lid << 1 ) + 1 ) - 1;
			int bi = offset * ( ( lid << 1 ) + 2 ) - 1;

			int t = localArray[ ai ];
			localArray[ ai ] = localArray[ bi ];
			localArray[ bi ] += t;
		}

		barrier( CLK_LOCAL_MEM_FENCE );
	}
	

	out[ gid ] = localArray[ lid ];
}
