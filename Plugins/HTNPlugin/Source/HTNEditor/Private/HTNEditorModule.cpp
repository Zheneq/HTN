// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HTNEditorLog.h"
#include "Containers/Array.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"

#include "AssetTools/HTNAssetActions.h"
#include "Styles/HTNEditorStyle.h"
#include "HTNEditorSettings.h"
#include "Customizations/HTNBlackboardSelectorDetails.h"


DEFINE_LOG_CATEGORY(LogHTNEditor);

#define LOCTEXT_NAMESPACE "HTNEditor"

//////////////////////////////////////////////////////////////////////////
// HTNEditor

/**
* Implements the HTNEditor module.
*/
class FHTNEditorModule
	: public IHasMenuExtensibility
	, public IHasToolBarExtensibility
	, public IModuleInterface
{
public:

	//~ IHasMenuExtensibility interface

	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override
	{
		return MenuExtensibilityManager;
	}

public:

	//~ IHasToolBarExtensibility interface

	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override
	{
		return ToolBarExtensibilityManager;
	}

public:

	//~ IModuleInterface interface

	virtual void StartupModule() override
	{
		Style = MakeShareable(new FHTNEditorStyle());

		//		FHTNEditorCommands::Register();

		RegisterAssetTools();
		RegisterMenuExtensions();
		RegisterSettings();
		RegisterCustomizations();

		UE_LOG(LogHTNEditor, Log, TEXT("HTN editor plugin is up and running!"));
	}

	virtual void ShutdownModule() override
	{
		UnregisterAssetTools();
		UnregisterMenuExtensions();
		UnregisterSettings();
		UnregisterCustomizations();
	}

	virtual bool SupportsDynamicReloading() override
	{
		return true;
	}

protected:

	/** Registers asset tool actions. */
	void RegisterAssetTools()
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

		RegisterAssetTypeAction(AssetTools, MakeShareable(new FHTNAssetActions(Style.ToSharedRef())));
	}

	/**
	* Registers a single asset type action.
	*
	* @param AssetTools The asset tools object to register with.
	* @param Action The asset type action to register.
	*/
	void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	{
		AssetTools.RegisterAssetTypeActions(Action);
		RegisteredAssetTypeActions.Add(Action);
	}

	/** Register the text asset editor settings. */
	void RegisterSettings()
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Editor", "Plugins", "HTN",
				LOCTEXT("HTNSettingsName", "Hierarchial Task Network"),
				LOCTEXT("HTNSettingsDescription", "Configure the Hierarchial Task Network plug-in."),
				GetMutableDefault<UHTNEditorSettings>()
			);
		}
	}

	void RegisterCustomizations()
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		PropertyModule.RegisterCustomPropertyTypeLayout("HTNBlackboardKeySelector", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FHTNBlackboardKeySelectorCustomization::MakeInstance));

		PropertyModule.NotifyCustomizationModuleChanged();
	}

	void UnregisterCustomizations()
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
			PropertyModule.UnregisterCustomPropertyTypeLayout("HTNBlackboardKeySelector");

			PropertyModule.NotifyCustomizationModuleChanged();
		}
	}


	/** Unregisters asset tool actions. */
	void UnregisterAssetTools()
	{
		FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");

		if (AssetToolsModule != nullptr)
		{
			IAssetTools& AssetTools = AssetToolsModule->Get();

			for (auto Action : RegisteredAssetTypeActions)
			{
				AssetTools.UnregisterAssetTypeActions(Action);
			}
		}
	}

	/** Unregister the text asset editor settings. */
	void UnregisterSettings()
	{
		ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

		if (SettingsModule != nullptr)
		{
			SettingsModule->UnregisterSettings("Editor", "Plugins", "HTN");
		}
	}

protected:

	/** Registers main menu and tool bar menu extensions. */
	void RegisterMenuExtensions()
	{
		MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
	}

	/** Unregisters main menu and tool bar menu extensions. */
	void UnregisterMenuExtensions()
	{
		MenuExtensibilityManager.Reset();
		ToolBarExtensibilityManager.Reset();
	}

private:

	/** Holds the menu extensibility manager. */
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;

	/** The collection of registered asset type actions. */
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;

	/** Holds the plug-ins style set. */
	TSharedPtr<ISlateStyle> Style;

	/** Holds the tool bar extensibility manager. */
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FHTNEditorModule, HTNEditor);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
