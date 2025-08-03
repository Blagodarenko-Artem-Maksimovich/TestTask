// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_GameStateBase.h"
#include "SaveobjectContainer.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "InteractiveObject.h"

// Define the save/load path once (Saved/SessionSave/SessionSave.json)
const FString AGS_GameStateBase::SessionFilePath = TEXT("SessionSave/SessionSave.json");

void AGS_GameStateBase::BeginPlay()
{
    Super::BeginPlay();
    // Load initial state from JSON
    LoadObjectsFromJson();
}

void AGS_GameStateBase::RegisterObject(const FObjectData& NewData)
{
    ObjectDataArray.Add(NewData);
    // Notify any UI subscribers
    OnObjectsUpdated.Broadcast(ObjectDataArray);
}

void AGS_GameStateBase::UpdateObjectData(const FObjectData& UpdatedData)
{
    for (FObjectData& Entry : ObjectDataArray)
    {
        if (Entry.Id == UpdatedData.Id)
        {
            Entry = UpdatedData;
            OnObjectsUpdated.Broadcast(ObjectDataArray);
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("UpdateObjectData: ID %d not found"), UpdatedData.Id);
}

void AGS_GameStateBase::LoadObjectsFromJson()
{
    // Full path in Saved/SessionSave/SessionSave.json
    const FString FullPath = FPaths::ProjectSavedDir() / SessionFilePath;

    // Early out if missing
    if (!FPaths::FileExists(FullPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("LoadObjectsFromJson: File not found: %s"), *FullPath);
        return;
    }

    FString JsonContent;
    if (!FFileHelper::LoadFileToString(JsonContent, *FullPath))
    {
        UE_LOG(LogTemp, Error, TEXT("LoadObjectsFromJson: Unable to read file: %s"), *FullPath);
        return;
    }

    // Deserialize into container struct
    FSaveObjectsContainer Container;
    FText Error;
    const bool bParsed = FJsonObjectConverter::JsonObjectStringToUStruct(JsonContent, &Container, 0, 0, /*bStrict=*/ true);

    if (!bParsed)
    {
        UE_LOG(LogTemp, Error, TEXT("LoadObjectsFromJson: JSON parse failed: %s"), *Error.ToString());
        return;
    }

    // Clear old state and notify UI (empty list)
    ObjectDataArray.Empty();
    OnObjectsUpdated.Broadcast(ObjectDataArray);

    // Spawn and init each actor, register if successful
    for (const FObjectData& Data : Container.objects)
    {
        if (UWorld* World = GetWorld())
        {
            AInteractiveObject* Obj = World->SpawnActor<AInteractiveObject>(AInteractiveObject::StaticClass());
            if (Obj && Obj->InitFromData(Data))
            {
                RegisterObject(Data);
            }
            else if (Obj)
            {
                Obj->Destroy();
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("LoadObjectsFromJson: Registered %d objects"), ObjectDataArray.Num());
}

void AGS_GameStateBase::SaveObjectsToJson()
{
    // Prepare container with current model
    FSaveObjectsContainer Container;
    Container.objects = ObjectDataArray;

    // Serialize with pretty-print
    FString OutJson;
    const bool bConverted = FJsonObjectConverter::UStructToJsonObjectString(
        FSaveObjectsContainer::StaticStruct(),
        &Container,
        OutJson,
        /*CheckFlags*/0, /*SkipFlags*/0,
        /*Indent=*/2, nullptr, /*bPrettyPrint=*/true);

    if (!bConverted)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveObjectsToJson: Struct->JSON conversion failed"));
        return;
    }

    // Ensure directory exists
    const FString FullPath = FPaths::ProjectSavedDir() / SessionFilePath;
    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    PF.CreateDirectoryTree(*FPaths::GetPath(FullPath));

    // Write out
    if (FFileHelper::SaveStringToFile(
        OutJson,
        *FullPath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM))
    {
        UE_LOG(LogTemp, Log, TEXT("SaveObjectsToJson: Saved to %s"), *FullPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SaveObjectsToJson: Failed to save to %s"), *FullPath);
    }
}



