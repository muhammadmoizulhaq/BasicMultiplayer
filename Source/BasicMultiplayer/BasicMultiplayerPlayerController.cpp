// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicMultiplayerPlayerController.h"
#include "BasicMultiplayerPlayerState.h"

ABasicMultiplayerPlayerController::ABasicMultiplayerPlayerController()
{
}

void ABasicMultiplayerPlayerController::Server_SetName_Implementation(const FText& Name)
{
	if (ABasicMultiplayerPlayerState* PS = Cast<ABasicMultiplayerPlayerState>(PlayerState))
	{
		PS->SetMyPlayerName(Name);
		UE_LOG(LogTemp, Warning, TEXT("Player Name {Server}: [%s]"), *Name.ToString());
	}
}