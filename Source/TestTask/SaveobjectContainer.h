#pragma once

#include "CoreMinimal.h"
#include "ObjectData.h"
#include "SaveobjectContainer.generated.h" // This include must be last

/**
 * Container struct for serializing and deserializing an array of FObjectData.
 * Used by JsonObjectConverter to wrap the array in a top-level "objects" key.
 *
 * Matches JSON schema:
 * {
 *    "objects": [ ... ]
 * }
 */
USTRUCT(BlueprintType)
struct TESTTASK_API FSaveObjectsContainer
{
    GENERATED_BODY()

    /** Array of object states (Model) to be saved or loaded */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveLoad")
    TArray<FObjectData> objects;

    /** Default constructor */
    FSaveObjectsContainer() = default;
};



