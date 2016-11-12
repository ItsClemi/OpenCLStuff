#include "stdafx.h"
#include "CoreExport.h"

#include "System/System.h"


GlobalEnvironment gEnv;


void InitializeSystem( )
{
	GetEnv( )->gCLMng = gSystem->GetCLManager( );
}
