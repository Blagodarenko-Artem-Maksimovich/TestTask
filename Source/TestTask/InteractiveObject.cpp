#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"

AInteractiveObject::AInteractiveObject()
{
    // ������������� �����������
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // ��������� ��������� ��������
    ObjectData = FObjectData();
}

void AInteractiveObject::BeginPlay()
{
    Super::BeginPlay();

    // ������������� ������
    if (ObjectData.bIsActive)
    {
        // ���������� ������ � �������
        // ��������, ��������� ����� ��� �������
    }
}

void AInteractiveObject::Interact()
{
    if (ObjectData.bIsActive)
    {
        // ���������� ��������������
        // ��������, ����� ���������
        UE_LOG(LogTemp, Warning, TEXT("�������������� � ��������: %s"), *ObjectData.Name);
    }
}