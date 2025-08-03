#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ObjectData.h"
#include "GS_GameStateBase.generated.h"

// Delegate to notify UI when object data changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnObjectsUpdated, const TArray<FObjectData>&, AllObjects);

/**
 * GameStateBase as Controller in MVC
 * - Manages Model: FObjectData[]
 * - Broadcasts changes to View via OnObjectsUpdated
 * - Handles Load/Save JSON
 */
UCLASS()
class TESTTASK_API AGS_GameStateBase : public AGameStateBase
{
    GENERATED_BODY()

public:
    /** Load initial objects from JSON at BeginPlay */
    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    void LoadObjectsFromJson();

    /** Save current object states to JSON on session end */
    UFUNCTION(BlueprintCallable, Category = "SaveLoad")
    void SaveObjectsToJson();

    /** Add a new object to array and notify UI */
    void RegisterObject(const FObjectData& NewData);

    /** Update existing object by Id and notify UI */
    void UpdateObjectData(const FObjectData& UpdatedData);

    /** Read-only access to all object data (Model) */
    const TArray<FObjectData>& GetAllObjectData() const { return ObjectDataArray; }

    /** Broadcast when object data array changes */
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnObjectsUpdated OnObjectsUpdated;

protected:
    virtual void BeginPlay() override;

private:
    /** Where to save/load the JSON file (relative to Saved/) */
    static const FString SessionFilePath;

    /** Internal storage of object states (Model) */
    UPROPERTY(VisibleAnywhere, Category = "Model")
    TArray<FObjectData> ObjectDataArray;
};



