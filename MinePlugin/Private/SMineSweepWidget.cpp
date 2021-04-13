
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
			+ SVerticalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top).AutoHeight() //Assign num of mines
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
				SNew(STextBlock).Text(FText::FromString(TEXT("Number of Mines:"))).Margin(margin).MinDesiredWidth(180.0f)
				]
				+ SHorizontalBox::Slot().HAlign(HAlign_Left).VAlign(VAlign_Top)
				[
				SAssignNew(EditableTextBox_MinesNumPtr, SEditableTextBox).MinDesiredWidth(60.0f)
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
	if (EditableTextBox_WidthPtr->GetText().IsNumeric()
		&& EditableTextBox_HeightPtr->GetText().IsNumeric()
		&& EditableTextBox_MinesNumPtr->GetText().IsNumeric())
	{
		GridWidth = FCString::Atoi(*EditableTextBox_WidthPtr->GetText().ToString());
		GridHeight = FCString::Atoi(*EditableTextBox_HeightPtr->GetText().ToString());
		MinesNum = FCString::Atoi(*EditableTextBox_MinesNumPtr->GetText().ToString());

		if (GridWidth > 0 && GridHeight > 0 
			&& MinesNum >= 0 && MinesNum <= GridWidth * GridHeight)
		{
			RefreshGrid(GridWidth, GridHeight, MinesNum);
		}
	}

	return FReply::Handled();
}

void SMineSweepWidget::RefreshGrid(const int32& width, const int32& height, const int32& minesNum)
{
	//Clear all elements inside container
	GridContainer->ClearChildren();
	//Reset ButtonTextTArray
	ButtonTextTArray.Empty();
	//Initialize bClickedTArray
	bClickedTArray.Init(false, width * height);
	//Reset MinePosTArray
	MinePosTArray.Empty();

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
	//Generate random mines
	GenerateRandomMines(width * height, minesNum);
}

void SMineSweepWidget::GenerateRandomMines(const int32& size, const int32& minesNum)
{
	TArray<int32> TempArr;
	for (int32 i = 0; i < size; i++)
	{
		TempArr.Add(i);
	}
	//Shuffle temp array
	const int32 LastIndex = TempArr.Num() - 1;
	for (int32 i = 0; i < LastIndex; ++i)
	{
		int32 Index = FMath::RandRange(0, LastIndex);
		if (i != Index)
		{
			TempArr.Swap(i, Index);
		}
	}
	//Pick up the first MinesNum number from temp array to get unique random array with size MinesNum
	for (int32 i = 0; i < minesNum; i++)
	{
		MinePosTArray.Add(TempArr[i]);
	}
}

int32 SMineSweepWidget::CountSurroundingMines(const int32& clickRow, const int32& clickColumn)
{
	int32 downerRow = FMath::Max(0, clickRow - 1);
	int32 upperRow = FMath::Min(GridHeight - 1, clickRow + 1);
	int32 leftColumn = FMath::Max(0, clickColumn - 1);
	int32 rightColumn = FMath::Min(GridWidth - 1, clickColumn + 1);

	int32 counter = 0;

	for (int32 i = downerRow; i <= upperRow; i++)
	{
		for (int32 j = leftColumn; j <= rightColumn; j++)
		{
			if (MinePosTArray.Find(i * GridWidth + j) != INDEX_NONE)
			{
				counter++;
			}
		}
	}

	if (counter == 0) //There's no surrounding bombs, recursively click surrounding button
	{
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

	return counter;
}

FReply SMineSweepWidget::OnSingleButtonClicked(const int32 id)
{
	UE_LOG(LogTemp, Warning, TEXT("Button id is %d ."), id);

	int32 clickRow = (int32)(id / GridWidth);
	int32 clickColumn = (int32)(id % GridWidth);

	bClickedTArray[id] = true;

	if (MinePosTArray.Find(id) != INDEX_NONE)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString(TEXT("BOOM! GAME OVER!"))));
		ButtonTextTArray[id]->SetText(FText::FromString(FString(TEXT("M"))));
	}
	else
	{
		ButtonTextTArray[id]->SetText(FText::FromString( FString::FromInt(CountSurroundingMines(clickRow, clickColumn)) ));
	}

	return FReply::Handled();
}