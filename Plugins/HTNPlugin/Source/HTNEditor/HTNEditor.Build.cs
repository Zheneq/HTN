// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HTNEditor : ModuleRules
{
	public HTNEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		DynamicallyLoadedModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"MainFrame",
//				"WorkspaceMenuStructure",
			});

		PrivateIncludePaths.AddRange(
			new string[] {
				"HTNEditor/Private",
                "HTNEditor/Private/AssetTools",
                "HTNEditor/Private/Factories",
                "HTNEditor/Private/Shared",
                "HTNEditor/Private/Styles",
                "HTNEditor/Private/Toolkits",
                "HTNEditor/Private/Widgets",
			});

		PrivateDependencyModuleNames.AddRange(
			new string[] {
				"ContentBrowser",
				"Core",
				"CoreUObject",
				"DesktopWidgets",
				"EditorStyle",
				"Engine",
				"InputCore",
				"Projects",
				"Slate",
				"SlateCore",
				"UnrealEd",
                "KismetWidgets",
                "HTNRuntime",
                "AIModule",
            });

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"UnrealEd",
//				"WorkspaceMenuStructure",
			});
	}
}
