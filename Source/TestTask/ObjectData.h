// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObjectData.generated.h"

/**
 * struct object data
 */

USTRUCT(BlueprintType)
struct FObjectData
{
    GENERATED_BODY()

    // Object id
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data")
    int32 Id;

    // Object name
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data")
    FString Name;

    // Object world position
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data")
    FVector Position;

    // Object color
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data")
    FString ColorName;

    // Object is active ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Data")
    bool bIsActive;

    FObjectData()
        : Id(0), Name(TEXT("Unknown")), Position(FVector::ZeroVector), ColorName(TEXT("White")), bIsActive(true)
    {}
};