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

    // Инициализация данных
    if (ObjectData.bIsActive)
    {
        // Применение данных к объекту
        // Например, установка цвета или позиции
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
    }
}