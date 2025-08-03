#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"

AInteractiveObject::AInteractiveObject()
{
MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComponent;

    if (NameToMeshMap.Num() == 0)
    {
        struct FStaticMeshLoader
        {
            UStaticMesh* Load(const TCHAR* Path) const
            {
                ConstructorHelpers::FObjectFinder<UStaticMesh> Finder(Path);
                return Finder.Succeeded() ? Finder.Object : nullptr;
            }
        } Loader;

        NameToMeshMap.Add(TEXT("Cube"), Loader.Load(TEXT("/Game/StarterContent/Shapes/Shape_Cube")));
        NameToMeshMap.Add(TEXT("Sphere"), Loader.Load(TEXT("/Game/StarterContent/Shapes/Shape_Sphere")));
        NameToMeshMap.Add(TEXT("Cylinder"), Loader.Load(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder")));
        NameToMeshMap.Add(TEXT("Cone"), Loader.Load(TEXT("/Game/StarterContent/Shapes/Shape_Cone")));
        NameToMeshMap.Add(TEXT("Plane"), Loader.Load(TEXT("/Game/StarterContent/Shapes/Shape_Plane")));
        NameToMeshMap.Add(TEXT("Box"), Loader.Load(TEXT("/Game/StarterContent/Shapes/Shape_Cube")));
    }
}

void AInteractiveObject::BeginPlay()
{
    Super::BeginPlay();

    if (GetWorld())
    {
        GameState = GetWorld()->GetGameState<AGS_GameStateBase>();

        InitFromData(ObjectData);
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

bool AInteractiveObject::InitFromData(const FObjectData& InData)
{
    ObjectData = InData;
    SetActorLocation(ObjectData.Position);

    UStaticMesh** Found = NameToMeshMap.Find(ObjectData.Name);
    UStaticMesh* MeshToSet = Found && *Found ? *Found : nullptr;

    if (!MeshToSet)
    {
        // ≈сли им€ содержит подстроку (например "Cube1", "Cube_123"), ищем через Contains
        for (auto& Pair : NameToMeshMap)
        {
            if (ObjectData.Name.Contains(Pair.Key))
            {
                MeshToSet = Pair.Value;
                break;
            }
        }
    }

    if (MeshToSet)
    {
        MeshComponent->SetStaticMesh(MeshToSet);
    }
    else
    {
        
        UStaticMesh* DefaultMesh = NameToMeshMap.FindRef(TEXT("Cube"));
        MeshComponent->SetStaticMesh(DefaultMesh);
        
    }

    UMaterialInterface* Mat = ObjectData.bIsActive && ActiveMaterial ? ActiveMaterial : InactiveMaterial;
    if (Mat)
        MeshComponent->SetMaterial(0, Mat);
    return true;
}