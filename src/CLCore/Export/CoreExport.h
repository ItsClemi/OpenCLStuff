#pragma once

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
