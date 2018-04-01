// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HTNRuntimeModule.h"
//#include "Misc/ConfigCacheIni.h"
//#include "Modules/ModuleManager.h"
#include "HTNPrivate.h"
//#include "PaperRenderSceneProxy.h"

//DEFINE_STAT(STAT_PaperRender_SetSpriteRT);

//////////////////////////////////////////////////////////////////////////
// FPaper2DModule

class FHTNModule : public IHTNModuleInterface
{
public:
	virtual void StartupModule() override
	{
		/*
		check(GConfig);

		if (!GConfig->GetVector(TEXT("Paper2D"), TEXT("PaperAxisX"), PaperAxisX, GEngineIni))
		{
			PaperAxisX = FVector(1.0f, 0.0f, 0.0f);
		}
		if (!GConfig->GetVector(TEXT("Paper2D"), TEXT("PaperAxisY"), PaperAxisY, GEngineIni))
		{
			PaperAxisY = FVector(0.0f, 0.0f, 1.0f);
		}

		PaperAxisZ = FVector::CrossProduct(PaperAxisX, PaperAxisY);

		FPaperSpriteTangents::SetTangentsFromPaperAxes();
		*/
	}

	virtual void ShutdownModule() override
	{
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FHTNModule, HTN);
DEFINE_LOG_CATEGORY(LogHTN);

