// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectData.h"
#include "InteractableInterface.h"
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

    // Materials
    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInterface* ActiveMaterial;

    UPROPERTY(EditDefaultsOnly, Category = "Materials")
    UMaterialInterface* InactiveMaterial;


    AInteractiveObject();

    virtual void Interact_Implementation();

protected:
    virtual void BeginPlay() override;
};
