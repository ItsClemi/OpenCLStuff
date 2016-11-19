#pragma once

#include <functional>
#include <chrono>

__interface ICLManager;

struct GlobalEnvironment
{
	ICLManager*		gCLMng;
};

extern GlobalEnvironment gEnv;


inline GlobalEnvironment* GetEnv( )
{
	return &gEnv;
}

inline ICLManager* GetCLManager( )
{
	return GetEnv( )->gCLMng;
}

void InitializeSystem( );

#include "CoreExportStruct.h"



//////////////////////////////////////////////////////////////////////////

class FunctionMeasure
{
public:
	//std::chrono::microseconds
	template< typename TM, typename _Fn, typename... _Args >
	inline static long long Measure( _Fn&& _Fx, _Args&&... _Ax )
	{
		auto begin = std::chrono::high_resolution_clock::now( );
		{
			_Fx( std::forward< _Args >( _Ax )... );
		}
		auto end = std::chrono::high_resolution_clock::now( );

		auto microseconds = std::chrono::duration_cast< TM >( end - begin );

		return microseconds.count( );
	}
};