#include "InteractiveObject.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

// Static members initialization
TMap<FString, UStaticMesh*> AInteractiveObject::NameToMeshMap;
TMap<FString, FLinearColor> AInteractiveObject::NameToColorMap;
UMaterialInterface* AInteractiveObject::M_Dynamic = nullptr;

AInteractiveObject::AInteractiveObject()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = MeshComponent;

    // Load mesh references once
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

    // Load material if not already loaded
    if (!M_Dynamic)
    {
        ConstructorHelpers::FObjectFinder<UMaterialInterface> Finder(TEXT("/Game/StarterContent/Materials/M_Dynamic.M_Dynamic"));
        M_Dynamic = Finder.Succeeded() ? Finder.Object : nullptr;
    }

    // Populate color map
    NameToColorMap.Add(TEXT("red"), FLinearColor(255, 0, 0));
    NameToColorMap.Add(TEXT("blue"), FLinearColor(0, 0, 255));
    NameToColorMap.Add(TEXT("green"), FLinearColor(0, 255, 0));
    NameToColorMap.Add(TEXT("white"), FLinearColor(255, 255, 255));
    NameToColorMap.Add(TEXT("black"), FLinearColor(0, 0, 0));
    NameToColorMap.Add(TEXT("yellow"), FLinearColor::Yellow);
    NameToColorMap.Add(TEXT("gray"), FLinearColor::Gray);

    // Cache keys for random selection
    NameToColorMap.GetKeys(Keys);
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

bool AInteractiveObject::InitFromData(const FObjectData& InData)
{
    ObjectData = InData;
    SetActorLocation(ObjectData.Position);

    UStaticMesh* MeshToSet = nullptr;
    if (UStaticMesh** Found = NameToMeshMap.Find(ObjectData.Name))
    {
        MeshToSet = *Found;
    }
    else
    {
        // Fallback: search by substring match
        for (auto& Pair : NameToMeshMap)
        {
            if (ObjectData.Name.Contains(Pair.Key))
            {
                MeshToSet = Pair.Value;
                break;
            }
        }
    }

    if (!ensure(M_Dynamic)) return false;

    MI_Dynamic = UMaterialInstanceDynamic::Create(M_Dynamic, this);
    MeshComponent->SetMaterial(0, MI_Dynamic);

    UpdateMIDColor();

    if (MeshToSet)
    {
        MeshComponent->SetStaticMesh(MeshToSet);
    }
    else
    {
        // Default to cube
        MeshComponent->SetStaticMesh(NameToMeshMap.FindRef(TEXT("Cube")));
    }

    return true;
}

void AInteractiveObject::SetColorByName(const FString& ColorName)
{
    ObjectData.ColorName = ColorName;
    UpdateMIDColor();

    if (GameState)
    {
        GameState->UpdateObjectData(ObjectData);
    }
}

FString AInteractiveObject::GetRandomColor()
{
    // Safety check in case Keys is empty
    if (Keys.Num() == 0)
    {
        return TEXT("white");
    }

    return *Keys[FMath::RandRange(0, Keys.Num() - 1)];
}

void AInteractiveObject::UpdateMIDColor()
{
    if (!MI_Dynamic) return;

    // Convert to lowercase to avoid mismatches
    FLinearColor* Found = NameToColorMap.Find(ObjectData.ColorName.ToLower());
    FLinearColor ColorToUse = Found ? *Found : FLinearColor::White;

    MI_Dynamic->SetVectorParameterValue(TEXT("ColorParam"), ColorToUse);
}

void AInteractiveObject::Interact_Implementation()
{
    ObjectData.bIsActive = !ObjectData.bIsActive;

    if (MeshComponent)
    {
        SetColorByName(GetRandomColor());
        if (GameState)
        {
            GameState->UpdateObjectData(ObjectData);
        }
    }
}



