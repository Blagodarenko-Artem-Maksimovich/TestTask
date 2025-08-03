// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GS_GameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "HUDBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTTASK_API AHUDBase : public AHUD
{
	GENERATED_BODY()
	

public:

    AHUDBase();

    virtual void BeginPlay() override;

    // В Blueprint: выбери Widget, основанный на UObjectListWidget
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> WidgetClass = nullptr;

protected:

    UPROPERTY()
    UUserWidget* WidgetInstance = nullptr;
};