// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HTNEditor : ModuleRules
{
	public HTNEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateIncludePaths.Add("HTNEditor/Private");
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"UnrealEd", // for FAssetEditorManager
			"KismetWidgets",
			"Kismet",  // for FWorkflowCentricApplication
			"PropertyEditor",
			"RenderCore",
			"HTN",
			"ContentBrowser"
		});
	}
}
