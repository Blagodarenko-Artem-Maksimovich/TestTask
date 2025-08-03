#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectData.h"
#include "ObjectListWidget.generated.h"

class UVerticalBox;
class UButton;

UCLASS()
class TESTTASK_API UObjectListWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    void InitializeWidget();

    UFUNCTION(BlueprintCallable)
    void RefreshList(const TArray<FObjectData>& ObjectDataArray);

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* ListBox;

    UPROPERTY(meta = (BindWidget))
    UButton* FinishSessionButton;

    virtual void NativeConstruct() override;

private:
    UFUNCTION()
    void OnFinishClicked();
};