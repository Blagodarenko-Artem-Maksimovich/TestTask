// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ObjectData.h"
#include "GS_GameStateBase.generated.h"

/**
 * 
 */
DECLARE_MULTICAST_DELEGATE(FOnObjectsUpdated);

UCLASS()
class TESTTASK_API AGS_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
    // Добавить объект
    void RegisterObject(FObjectData& ObjectData);

    // Получить список всех данных
    const TArray<FObjectData>& GetAllObjectData() const;

    // Обновить данные по ID
    void UpdateObjectData(const FObjectData& NewData);

    FOnObjectsUpdated OnObjectsUpdated;

protected:
    // Все данные объектов
    UPROPERTY(VisibleAnywhere)
    TArray<FObjectData> ObjectDataArray;
};
