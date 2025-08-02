// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectData.h"
#include "InteractiveObject.generated.h"

UCLASS()
class TESTTASK_API AInteractiveObject : public AActor
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactive Object")
    FObjectData ObjectData;

    // Components
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* MeshComponent;

    AInteractiveObject();

    void Interact();

protected:
    virtual void BeginPlay() override;
};
