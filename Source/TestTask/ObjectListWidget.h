#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectData.h"
#include "GS_GameStateBase.h"
#include "ObjectListWidget.generated.h"

class UVerticalBox;
class UButton;

/**
 * UMG Widget displaying list of objects and their states.
 * Acts as the View in MVC, subscribing to GameState's data updates.
 */
UCLASS()
class TESTTASK_API UObjectListWidget : public UUserWidget
{
    GENERATED_BODY()

public:

    /** Setup initial binding to GameState and populate widget */
    UFUNCTION(BlueprintCallable, Category = "Initialization")
    void InitializeWidget();

    /** Refresh the list UI when object data changes */
    UFUNCTION(BlueprintCallable, Category = "UI")
    void RefreshList(const TArray<FObjectData>& ObjectDataArray);

protected:

    /** Vertical container holding rows for objects */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ListBox = nullptr;

    /** Button to signal session end and trigger save */
    UPROPERTY(meta = (BindWidget))
    UButton* FinishSessionButton = nullptr;

    /** Cached reference to the Controller (GameState) */
    UPROPERTY()
    AGS_GameStateBase* GS = nullptr;

    /** Called once widget is constructed in the viewport */
    virtual void NativeConstruct() override;

private:

    /** Handler for the finish session button's OnClicked event */
    UFUNCTION()
    void OnFinishClicked();
};



