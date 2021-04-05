// Fill out your copyright notice in the Description page of Project Settings.


#include "SMineSweepWidget.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/MessageDialog.h"


void SMineSweepWidget::Construct(const FArguments& InArgs)
{
	const FMargin margin = 1.0f;

	ChildSlot
		[
			SNew(SVerticalBox) //Width & Height Edit box
			+ SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top).AutoHeight()
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Width:"))).Margin(margin)
				]
				+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
					SAssignNew(EditableTextBox_WidthPtr, SEditableTextBox).MinDesiredWidth(60.0f)
				]
				+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Height:"))).Margin(margin)
				]
				+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
					SAssignNew(EditableTextBox_HeightPtr, SEditableTextBox).MinDesiredWidth(60.0f)
				]
			]
			+ SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top).AutoHeight() //Generate new grid button
			[
				SNew(SButton)
				.OnClicked_Raw(this, &SMineSweepWidget::OnGenerateGridButtonClicked)
				.Content()
				[
					SNew(STextBlock)
					.Text(FText::FromString(TEXT("Generate Grid")))
				]
			]
			+ SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top).AutoHeight() //Grid show here
			[
				SAssignNew(GridContainer, SVerticalBox)
			]
		];
}

FReply SMineSweepWidget::OnGenerateGridButtonClicked()
{
	if (EditableTextBox_WidthPtr->GetText().IsNumeric() && EditableTextBox_HeightPtr->GetText().IsNumeric())
	{
		GridWidth = FCString::Atoi(*EditableTextBox_WidthPtr->GetText().ToString());
		GridHeight = FCString::Atoi(*EditableTextBox_HeightPtr->GetText().ToString());
		if (GridWidth > 0 && GridHeight > 0)
		{
			RefreshGrid(GridWidth, GridHeight);
		}
	}

	return FReply::Handled();
}

void SMineSweepWidget::RefreshGrid(const int32& width, const int32& height)
{
	//Clear all elements inside container
	GridContainer->ClearChildren();
	//Reset ButtonTextTArray
	ButtonTextTArray.Empty();
	//Initialize bClickedTArray
	bClickedTArray.Init(false, width * height);

	GridWidth = width;
	GridHeight = height;

	//Generate new grid in container 
	for (int32 i = 0; i < height; i++)
	{
		TSharedPtr<SHorizontalBox> horizontalBox;
		GridContainer->AddSlot().VAlign(VAlign_Top).HAlign(HAlign_Left)
			[
				SAssignNew(horizontalBox, SHorizontalBox)
			];
		for (int32 j = 0; j < width; j++)
		{
			TSharedPtr<STextBlock> ButtonLabel;
			const int32 ID = i * width + j;
			horizontalBox->AddSlot().VAlign(VAlign_Top).HAlign(HAlign_Left).Padding(1.0f)
				[
					SNew(SBox)
					.WidthOverride(40)
				.HeightOverride(40)
				[
					SNew(SButton)
					.OnClicked_Raw(this, &SMineSweepWidget::OnSingleButtonClicked, ID)
				.Content()
				[
					SAssignNew(ButtonLabel, STextBlock)
					.Text(FText::FromString(TEXT("")))
				]
				]
				];
			ButtonTextTArray.Add(ButtonLabel);
		}
	}
	//Generate a random mine, currently there's only one mine 
	MinePos = FMath::RandRange(0, width * height - 1);
}

FReply SMineSweepWidget::OnSingleButtonClicked(const int32 id)
{
	UE_LOG(LogTemp, Warning, TEXT("Button id is %d ."), id);

	int32 row = (int32)(id / GridWidth);
	int32 column = (int32)(id % GridWidth);
	int32 mineRow = (int32)(MinePos / GridWidth);
	int32 mineColumn = (int32)(MinePos % GridWidth);

	bClickedTArray[id] = true;

	if (id == MinePos)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString(TEXT("BOOM! GAME OVER!"))));
		ButtonTextTArray[id]->SetText(FText::FromString(FString(TEXT("M"))));
	}
	else
	{
		if (FMath::Abs(row - mineRow) <= 1 && FMath::Abs(column - mineColumn) <= 1)
		{
			ButtonTextTArray[id]->SetText(FText::FromString(FString(TEXT("1"))));
		}
		else
		{
			int32 downerRow = FMath::Max(0, row - 1);
			int32 upperRow = FMath::Min(GridHeight - 1, row + 1);
			int32 leftColumn = FMath::Max(0, column - 1);
			int32 rightColumn = FMath::Min(GridWidth - 1, column + 1);

			ButtonTextTArray[id]->SetText(FText::FromString(FString(TEXT("0"))));

			for (int32 i = downerRow; i <= upperRow; i++)
			{
				for (int32 j = leftColumn; j <= rightColumn; j++)
				{
					if (!bClickedTArray[i * GridWidth + j])
					{
						OnSingleButtonClicked(i * GridWidth + j);
					}
				}
			}
		}
	}

	return FReply::Handled();
}