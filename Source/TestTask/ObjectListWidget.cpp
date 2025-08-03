#include "ObjectListWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "GS_GameStateBase.h"
#include "Kismet/KismetSystemLibrary.h"

void UObjectListWidget::NativeConstruct()
{
    Super::NativeConstruct();

    GS = GetWorld() ? GetWorld()->GetGameState<AGS_GameStateBase>() : nullptr;

    if (!ensure(GS != nullptr))
    {
        UE_LOG(LogTemp, Error, TEXT("UObjectListWidget::NativeConstruct failed to get GameState"));
        return;
    }

    if (FinishSessionButton)
    {
        FinishSessionButton->OnClicked.AddUniqueDynamic(this, &UObjectListWidget::OnFinishClicked);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFinishClicked: FinishSessionButton not bound"));
    }

    InitializeWidget();
}

void UObjectListWidget::InitializeWidget()
{
    if (!ensure(GS))
    {
        return;
    }

    // Bind to the multicast delegate to update UI on data change
    GS->OnObjectsUpdated.AddUniqueDynamic(this, &UObjectListWidget::RefreshList);

    // Initial population
    RefreshList(GS->GetAllObjectData());
}

void UObjectListWidget::RefreshList(const TArray<FObjectData>& ObjectDataArray)
{
    if (!ensure(ListBox))
    {
        return;
    }

    ListBox->ClearChildren();

    for (const FObjectData& Data : ObjectDataArray)
    {
        // border: row container
        UBorder* Row = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
        // text: row label
        UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

        if (Row && TextBlock)
        {
            const FString Status = Data.bIsActive ? TEXT("Active") : TEXT("Inactive");
            const FString Label = FString::Printf(
                TEXT("Name: %s   Color: %s   [%s]"), *Data.Name, *Data.ColorName, *Status);

            TextBlock->SetText(FText::FromString(Label));

            Row->SetBrushColor(FLinearColor::Black);
            Row->SetPadding(FMargin(5.0f));
            Row->AddChild(TextBlock);

            ListBox->AddChild(Row);
        }
    }
}

void UObjectListWidget::OnFinishClicked()
{
    if (GS)
    {
        GS->SaveObjectsToJson();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFinishClicked: GameState is null"));
    }

    APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr;

    if (PC)
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFinishClicked: PlayerController is null"));
    }
}



