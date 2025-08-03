// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectData.h"
#include "SaveobjectContainer.generated.h"


USTRUCT(BlueprintType)
struct FSaveObjectsContainer
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FObjectData> objects;

    FSaveObjectsContainer() {}
};