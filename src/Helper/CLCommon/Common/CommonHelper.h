#pragma once

template< typename T >
inline void SafeDelete( T& t )
{
	if( t != nullptr )
	{
		delete( t );
		t = nullptr;
	}
}

template< typename T >
inline void SafeDeleteArray( T& t )
{
	if( t != nullptr )
	{
		delete[ ] t;
		t = nullptr;
	}
}