// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_MenuWidget.h"
#include "Components/Button.h"
#include "Engine/GameEngine.h"
#include "GameFramework/PlayerController.h"
#include "inTheNameOfGod/MainPlayer.h"


void UUW_MenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    startGameButton->OnClicked.AddDynamic(this, &UUW_MenuWidget::StartGame);
    exitgameButton->OnClicked.AddDynamic(this, &UUW_MenuWidget::ExitGame);

}


void UUW_MenuWidget::StartGame()
{
    //FName menuName = "TutorialLevel";
	//UGameplayStatics::OpenLevel(this, menuName);
    SetVisibility(ESlateVisibility::Hidden);
    SetIsEnabled(false);
    UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
    if (GameEngine)
    {
        APlayerController* PlayerController = GameEngine->GetFirstLocalPlayerController(GetWorld());

        if (PlayerController)
        {
            PlayerController->SetInputMode(FInputModeGameOnly());
        }
    }
    
}
void UUW_MenuWidget::ExitGame()
{
    UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
    if (GameEngine)
    {
        APlayerController* PlayerController = GameEngine->GetFirstLocalPlayerController(GetWorld());

        if (PlayerController)
        {
            PlayerController->ConsoleCommand("quit");
        }
    }
}