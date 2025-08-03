// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectData.h"
#include "InteractableInterface.h"
#include "GS_GameStateBase.h"
//#include "Materials/MaterialInstanceDynamic.h"
#include "InteractiveObject.generated.h"

UCLASS()
class TESTTASK_API AInteractiveObject : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactive Object")
    FObjectData ObjectData;

    // Components
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    UPROPERTY()
    AGS_GameStateBase* GameState = nullptr;

    UPROPERTY(VisibleAnywhere)
    UMaterialInstanceDynamic* MI_Dynamic;

    static UMaterialInterface* M_Dynamic;

    UFUNCTION(BlueprintCallable)
    bool InitFromData(const FObjectData& InData);

    UFUNCTION(BlueprintCallable)
    void SetColorByName(const FString& ColorName);

    UFUNCTION(BlueprintCallable)
    FString GetRandomColor();

    AInteractiveObject();

    virtual void Interact_Implementation();

    TArray <FString> Keys;

protected:
    virtual void BeginPlay() override;

private:
    static TMap<FString, UStaticMesh*> NameToMeshMap;

    static TMap<FString, FLinearColor> NameToColorMap;

    void UpdateMIDColor();

};





