// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "HTNPrivate.h"


//////////////////////////////////////////////////////////////////////////
// FHTNModule

class FHTNModule
	: public IModuleInterface
{
public:
	//~ IModuleInterface interface

	virtual void StartupModule() override { }
	virtual void ShutdownModule() override { }

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FHTNModule, HTN);
DEFINE_LOG_CATEGORY(LogHTN);

