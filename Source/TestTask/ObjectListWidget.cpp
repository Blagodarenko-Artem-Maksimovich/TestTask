// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectListWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"
#include "GS_GameStateBase.h"
#include "TestTaskGameMode.h"

void UObjectListWidget::InitializeWidget()
{
    if (AGS_GameStateBase* GS = GetWorld()->GetGameState<AGS_GameStateBase>())
    {
        // Правильная подписка: AddUniqueDynamic, не AddUObject
        GS->OnObjectsUpdated.AddUniqueDynamic(this, &UObjectListWidget::RefreshList);

        // Инициализируем список сразу
        RefreshList(GS->GetAllObjectData());
    }
}

void UObjectListWidget::RefreshList(const TArray<FObjectData>& ObjectDataArray)
{
    ListBox->ClearChildren();

    for (const FObjectData& Data : ObjectDataArray)
    {
        UBorder* Row = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
        UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

        FString Status = Data.bIsActive ? TEXT("Active") : TEXT("Inactive");
        FString RowText = FString::Printf(TEXT("Name: %s   Colour: %s   [%s]"), *Data.Name, *Data.ColorName, *Status);
        Text->SetText(FText::FromString(RowText));
        Row->SetBrushColor(FLinearColor::Black);
        Row->SetPadding(FMargin(5));
        Row->AddChild(Text);
        ListBox->AddChild(Row);
    }
}

void UObjectListWidget::NativeConstruct()
{
    Super::NativeConstruct();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              

    if (FinishSessionButton)
    {
        FinishSessionButton->OnClicked.AddDynamic(this, &UObjectListWidget::OnFinishClicked);
    }

    InitializeWidget();
}

void UObjectListWidget::OnFinishClicked()
{
    if (ATestTaskGameMode* GM = GetWorld()->GetAuthGameMode<ATestTaskGameMode>())
    {
     //   GM->SaveCurrentObjectsToJson();
    }
}
