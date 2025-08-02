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
    // �������� ������
    void RegisterObject(FObjectData& ObjectData);

    // �������� ������ ���� ������
    const TArray<FObjectData>& GetAllObjectData() const;

    // �������� ������ �� ID
    void UpdateObjectData(const FObjectData& NewData);

    FOnObjectsUpdated OnObjectsUpdated;

protected:
    // ��� ������ ��������
    UPROPERTY(VisibleAnywhere)
    TArray<FObjectData> ObjectDataArray;
};
