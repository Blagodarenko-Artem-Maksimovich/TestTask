// Fill out your copyright notice in the Description page of Project Settings.


#include "GS_GameStateBase.h"

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
