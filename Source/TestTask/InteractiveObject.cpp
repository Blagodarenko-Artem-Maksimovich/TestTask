#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"

AInteractiveObject::AInteractiveObject()
{
    // Инициализация компонентов
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Установка начальных значений
    ObjectData = FObjectData();
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

void AInteractiveObject::Interact()
{
    if (ObjectData.bIsActive)
    {
        // Реализация взаимодействия
        // Например, вывод сообщения
        UE_LOG(LogTemp, Warning, TEXT("Взаимодействие с объектом: %s"), *ObjectData.Name);
    }
}