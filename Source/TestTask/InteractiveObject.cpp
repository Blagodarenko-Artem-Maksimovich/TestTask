#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"

AInteractiveObject::AInteractiveObject()
{
    // Инициализация компонентов
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Установка начальных значений
    ObjectData = FObjectData();

    ActiveMaterial = nullptr;
    InactiveMaterial = nullptr;
}

void AInteractiveObject::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        GameState = GetWorld()->GetGameState<AGS_GameStateBase>();
        if (GameState)
        {
            GameState->RegisterObject(ObjectData);
        }
    }
}

void AInteractiveObject::Interact_Implementation()
{
    ObjectData.bIsActive = !ObjectData.bIsActive;

    // Set material
    if (MeshComponent)
    {
        if (ObjectData.bIsActive && ActiveMaterial)
        {
            MeshComponent->SetMaterial(0, ActiveMaterial);
            ObjectData.ColorName= ActiveMaterial->GetName();
        }
        else if (!ObjectData.bIsActive && InactiveMaterial)
        {
            MeshComponent->SetMaterial(0, InactiveMaterial);
            ObjectData.ColorName = InactiveMaterial->GetName();
        }

        GameState->UpdateObjectData(ObjectData);
    }
}