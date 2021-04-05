#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Input/SEditableTextBox.h"


class SMineSweepWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SMineSweepWidget) { }
	SLATE_END_ARGS()
public:

	void Construct(const FArguments& InArgs);


private:

	void RefreshGrid(const int32& width, const int32& height);
	FReply OnGenerateGridButtonClicked();
	FReply OnSingleButtonClicked(const int32 id);

private:

	TSharedPtr<SEditableTextBox> EditableTextBox_WidthPtr;
	TSharedPtr<SEditableTextBox> EditableTextBox_HeightPtr;
	TSharedPtr<SVerticalBox> GridContainer;

	TArray<TSharedPtr<STextBlock>> ButtonTextTArray;
	TArray<bool> bClickedTArray;

	int32 GridWidth;
	int32 GridHeight;
	int32 MinePos;
};
