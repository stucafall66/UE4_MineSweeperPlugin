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

	void RefreshGrid(const int32& width, const int32& height, const int32& minesNum);
	void GenerateRandomMines(const int32& size, const int32& minesNum);
	FReply OnGenerateGridButtonClicked();
	FReply OnSingleButtonClicked(const int32 id);
	int32 CountSurroundingMines(const int32& clickRow, const int32& clickColumn);

private:

	TSharedPtr<SEditableTextBox> EditableTextBox_WidthPtr;
	TSharedPtr<SEditableTextBox> EditableTextBox_HeightPtr;
	TSharedPtr<SEditableTextBox> EditableTextBox_MinesNumPtr;
	TSharedPtr<SVerticalBox> GridContainer;

	TArray<TSharedPtr<STextBlock>> ButtonTextTArray;
	TArray<bool> bClickedTArray;
	TArray<int32> MinePosTArray;

	int32 GridWidth;
	int32 GridHeight;
	int32 MinesNum;
};
