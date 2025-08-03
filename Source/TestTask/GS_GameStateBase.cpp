// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_GameStateBase.h"
#include "SaveobjectContainer.h"
#include "JsonObjectConverter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "InteractiveObject.h"

void AGS_GameStateBase::RegisterObject(FObjectData& ObjectData)
{
	ObjectDataArray.Add(ObjectData);
    OnObjectsUpdated.Broadcast(ObjectDataArray);
}

const TArray<FObjectData>& AGS_GameStateBase::GetAllObjectData() const
{
	return ObjectDataArray;
}

void AGS_GameStateBase::UpdateObjectData(const FObjectData& NewData)
{
    for (FObjectData& Data : ObjectDataArray)
    {
        if (Data.Id == NewData.Id)
        {
            Data = NewData;
            OnObjectsUpdated.Broadcast(ObjectDataArray);
            return;
        }
    }
}

void AGS_GameStateBase::LoadObjectsFromJson()
{
    const FString FullPath = FPaths::ProjectDir() / TEXT("Content/Save/Save.json");
    if (!FPaths::FileExists(FullPath))
    {
        UE_LOG(LogTemp, Warning, TEXT("JSON load file not found: %s"), *FullPath);
        return;
    }

   FString JsonString;
    if (!FFileHelper::LoadFileToString(JsonString, *FullPath))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("LoadObjectsFromJson: File not found or unreadable: %s"), *FullPath);
        return;
    }

    FSaveObjectsContainer Container;
    FText ErrorMsg;

    const bool bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(JsonString, &Container, 0, 0, true);

    if (!bSuccess)
    {
        UE_LOG(LogTemp, Error,
            TEXT("LoadObjectsFromJson: JSON parsing failed: %s"), *ErrorMsg.ToString());
        return;
    }

    // Удалили старые данные перед загрузкой
    ObjectDataArray.Empty();

    // Сразу уведомляем подписчиков — UI очистится
    OnObjectsUpdated.Broadcast(ObjectDataArray);

    // Зарегистрировать каждый объект (дерайвинг-загрузка структуры)
    for (FObjectData& Obj : Container.objects)
    {
        AInteractiveObject* NewObject = GetWorld()->SpawnActor<AInteractiveObject>(AInteractiveObject::StaticClass());
        if (NewObject->InitFromData(Obj)) {
            RegisterObject(Obj);
        }
        else {
            NewObject->Destroy();
        }
    }

    UE_LOG(LogTemp, Log,
        TEXT("LoadObjectsFromJson: Loaded and registered %d objects"), ObjectDataArray.Num());
}

void AGS_GameStateBase::SaveObjectsToJson()
{
    FSaveObjectsContainer Container;
    Container.objects = ObjectDataArray;

    FString OutString;
    bool bOk = FJsonObjectConverter::UStructToJsonObjectString(
        FSaveObjectsContainer::StaticStruct(),
        &Container,
        OutString,
        /*CheckFlags=*/0, /*SkipFlags=*/0,
        /*Indent=*/2,
        /*ExportCb=*/nullptr,
        /*bPrettyPrint=*/true
    );

    if (!bOk)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveObjectsToJson: UStruct->JSON conversion failed"));
        return;
    }

    const FString SaveDir = FPaths::ProjectDir();
    const FString Filename = TEXT("Content/Save/Save.json");
    const FString FullPath = SaveDir / Filename;

    IPlatformFile& PF = FPlatformFileManager::Get().GetPlatformFile();
    if (!PF.DirectoryExists(*SaveDir))
    {
        PF.CreateDirectoryTree(*SaveDir);
    }

    bool bSaved = FFileHelper::SaveStringToFile(
        OutString,
        *FullPath,
        FFileHelper::EEncodingOptions::ForceUTF8WithoutBOM,
        &IFileManager::Get(),
        FILEWRITE_None
    );

    if (bSaved)
    {
        UE_LOG(LogTemp, Log, TEXT("Successfully saved JSON to %s"), *FullPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save JSON to %s"), *FullPath);
    }
}

void AGS_GameStateBase::BeginPlay()
{
    Super::BeginPlay();

    LoadObjectsFromJson();
}
