#include "HUDBase.h"
#include "Kismet/GameplayStatics.h"              // for error logging and utility access

AHUDBase::AHUDBase()
{
    // Automatically locate WB_ObjectList_C widget blueprint in project
    static ConstructorHelpers::FClassFinder<UUserWidget> Finder(
        TEXT("/Game/ThirdPerson/Blueprints/WB_ObjectList.WB_ObjectList_C")
    );
    if (ensureMsgf(Finder.Succeeded(), TEXT("WB_ObjectList widget class not found at startup")))
    {
        WidgetClass = Finder.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AHUDBase::AHUDBase failed to load WidgetClass from path"));
    }
}

void AHUDBase::BeginPlay()
{
    Super::BeginPlay();

    if (WidgetClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("AHUDBase::BeginPlay WidgetClass is null"));
        return;
    }

    // Use the controller that owns this HUD
    APlayerController* PC = GetOwningPlayerController();  // use Unreal Engine's API
    if (PC == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("AHUDBase::BeginPlay no owning player controller found"));
        return;
    }

    // Instantiate the widget, add to viewport, and set ownership
    WidgetInstance = CreateWidget<UUserWidget>(PC, WidgetClass);
    if (!WidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("AHUDBase::BeginPlay CreateWidget failed"));
        return;
    }

    WidgetInstance->AddToViewport(0);
    WidgetInstance->SetOwningPlayer(PC);  // ensure widget knows which player owns it

    UE_LOG(LogTemp, Log, TEXT("AHUDBase: WidgetInstance created and added to viewport"));
}



