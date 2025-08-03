// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDBase.h"

AHUDBase::AHUDBase()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> WBP(
        TEXT("/Game/ThirdPerson/Blueprints/WB_ObjectList.WB_ObjectList_C")
    );
    if (ensureMsgf(WBP.Succeeded(), TEXT("Can't find WB_ObjectList"))) {
        WidgetClass = WBP.Class;
    }
}

void AHUDBase::BeginPlay()
{
    Super::BeginPlay();

    
    APlayerController* PC = Cast<APlayerController>(Owner);
    if (PC)
    {
        
        WidgetInstance = CreateWidget<UUserWidget>(PC, WidgetClass);
        if (WidgetInstance)
        {
            WidgetInstance->AddToViewport(0); // Z-order 0
        }
    }
    
}
