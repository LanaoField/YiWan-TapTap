#pragma once

#include "CoreMinimal.h"
#include "AAUBackViewWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/HorizontalBox.h"
#include "Components/RichTextBlock.h"
#include "Components/VerticalBox.h"
#include "Model/China/AAUChinaConfigModel.h"
#include "AAUManualRealNameWidget.generated.h"

enum class EManualRealNameTextBoxType : uint8
{
	Name,
	Id
};
/**
 *  
 */
UCLASS()
class ANTIADDICTION_API UAAUManualRealNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UAAUManualRealNameWidget(const FObjectInitializer& ObjectInitializer);

	static UAAUManualRealNameWidget * ShowUI(AAURealNameWordType Type);
	void Dismiss();

	// 按钮回调，AAURealNameWordTypeVerify, AAURealNameWordTypeVerifyFail这两种状态下Name，CardID可用
	TFunction<void(const FString& Name, const FString& CardID)> SubmitBlock;

	TFunction<void()> CloseBlock;

	TFunction<void()> BackBlock;

	void ShowError(const FString& ErrorMsg);

	void SetFocusEditableTextBox(EManualRealNameTextBoxType BoxType);

	void ChangeType(AAURealNameWordType _Type);

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackBtnClick();
	
	UFUNCTION()
	void OnCloseBtnClick();

	UFUNCTION()
	void OnSubmitBtnClick();

	UFUNCTION()
	void OnNameEditableTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UFUNCTION()
	void OnCardIdEditableTextBoxCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
	UPROPERTY(meta = (BindWidget))
	UAAUBackViewWidget* BackView;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UEditableTextBox* NameTF;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* CardIDTF;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* ContentRichLabel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ErrorTipLabel;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ErrorTipView;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* InputView;

private:
	
	AAURealNameWordType Type;

	
};
