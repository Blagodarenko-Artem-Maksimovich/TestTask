#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"          // for UUserWidget
#include "HUDBase.generated.h"

/**
 * AHUDBase — acts as UI manager in MVC:
 * - Creates and owns the root UserWidget representing the screen UI
 * - Does NOT directly manage model data but could pass reference via binding
 */
UCLASS()
class TESTTASK_API AHUDBase : public AHUD
{
    GENERATED_BODY()

public:
    /** Default constructor used to set up default widget class */
    AHUDBase();

    /** Initialize UI on game start */
    virtual void BeginPlay() override;

    /** Class of the widget blueprint to instantiate (must derive from UUserWidget) */
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> WidgetClass = nullptr;

protected:
    /** Runtime instance of the top-level widget (stored as UUserWidget for generality) */
    UPROPERTY()
    UUserWidget* WidgetInstance = nullptr;
};



