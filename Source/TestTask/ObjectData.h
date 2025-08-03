#pragma once

#include "CoreMinimal.h"
#include "ObjectData.generated.h"

/**
 * FObjectData is the Model struct representing the state of an object in the scene.
 * 
 * This struct is serialized to/from JSON via JsonUtilities. The property names are designed
 * to match exactly the keys used in the JSON schema:
 *
 * {
 *   "id": 1,
 *   "name": "Box1",
 *   "position": { "X": 0.0, "Y": 0.0, "Z": 0.0 },
 *   "colorName": "red",
 *   "isActive": true
 * }
 *
 * BlueprintType allows using this struct in UMG/Blueprints if needed.
 */
USTRUCT(BlueprintType)
struct TESTTASK_API FObjectData
{
    GENERATED_BODY()

    /** Unique object identifier (must be stable between sessions) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLoad|Data")
    int32 Id = 0;

    /** Object name, used to identify mesh type and dictionary lookups */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLoad|Data")
    FString Name = TEXT("Unknown");

    /** World position; serialized as an object { "X": ..., "Y": ..., "Z": ... } */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLoad|Data")
    FVector Position = FVector::ZeroVector;

    /** String key representing color; must match NameToColorMap keys (case-insensitive) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLoad|Data")
    FString ColorName = TEXT("white");

    /** Whether the object is active (true) or inactive (false) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLoad|Data")
    bool bIsActive = true;

    // Default constructor is auto-generated; all defaults are set above.

    /**
     * Equality operator, useful for debugging or unit tests.
     * Compares all fields; position uses FVector::Equals with default tolerance.
     */
    bool operator==(const FObjectData& Other) const
    {
        return (Id == Other.Id)
            && (Name == Other.Name)
            && Position.Equals(Other.Position)
            && (ColorName == Other.ColorName)
            && (bIsActive == Other.bIsActive);
    }

    /**
     * REQUIRED for JsonObjectConverter: map "colorName" key (not "color"), matching JSON schema.
     * If you later rename ColorName field, update the JSON or supply custom mappings to JsonObjectConverter.
     */
};



