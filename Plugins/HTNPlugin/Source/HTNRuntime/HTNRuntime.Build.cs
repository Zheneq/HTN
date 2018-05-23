// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HTNRuntime : ModuleRules
{
	public HTNRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
                "AIModule",
            });

		PrivateIncludePaths.AddRange(
			new string[] {
                "HTNRuntime/Private",
			});
	}
}
