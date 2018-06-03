// 

#pragma once

#include "EditorUndoClient.h"
#include "Templates/SharedPointer.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "UObject/GCObject.h"

class FSpawnTabArgs;
class ISlateStyle;
class IToolkitHost;
class SDockTab;
class UHTNAsset;


/**
 * Implements an Editor toolkit for textures.
 */
class FHTNEditorToolkit
	: public FAssetEditorToolkit
	, public FEditorUndoClient
	, public FGCObject
{
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InStyle The style set to use.
	 */
	FHTNEditorToolkit(const TSharedRef<ISlateStyle>& InStyle);

	/** Virtual destructor. */
	virtual ~FHTNEditorToolkit();

public:

	/**
	 * Initializes the editor tool kit.
	 *
	 * @param InHTNAsset The UHTNAsset asset to edit.
	 * @param InMode The mode to create the toolkit in.
	 * @param InToolkitHost The toolkit host.
	 */
	void Initialize(UHTNAsset* InHTNAsset, const EToolkitMode::Type InMode, const TSharedPtr<IToolkitHost>& InToolkitHost);

public:

	//~ FAssetEditorToolkit interface

//	virtual FString GetDocumentationLink() const override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager) override;

public:

	//~ IToolkit interface

	virtual FText GetBaseToolkitName() const override;
	virtual FName GetToolkitFName() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FString GetWorldCentricTabPrefix() const override;

public:

	//~ FGCObject interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	
protected:

	//~ FEditorUndoClient interface

	virtual void PostUndo(bool bSuccess) override;
	virtual void PostRedo(bool bSuccess) override;

private:

	/** Callback for spawning the Properties tab. */
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);

public:
	// Returns object to be observed in the editor's details panel
	UObject* GetObjectForDetailsPanel() const;

	TWeakObjectPtr<UHTNAsset> GetAsset() { return HTNAsset; }

	// Returns the Composite task which is currently selected (nullptr if none selected)
	FHTNBuilder_CompositeTask* GetSelectedCompositeTask() const;

	void SelectCompositeTask(FName Name);

private:

	/** The text asset being edited. */
	UHTNAsset* HTNAsset;

	/** Pointer to the style set to use for toolkits. */
	TSharedRef<ISlateStyle> Style;

	/** Selected task (can be primitive or Composite) */
	FName SelectedTask;
};
