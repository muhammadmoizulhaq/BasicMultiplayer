// Copyright Epic Games, Inc. All Rights Reserved.

#include "BasicMultiplayerGameMode.h"
#include "BasicMultiplayerCharacter.h"
#include "BasicMultiplayerPlayerState.h"

ABasicMultiplayerGameMode::ABasicMultiplayerGameMode()
{
}

void ABasicMultiplayerGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// Iterator over all player controllers
	//for (FConstPlayerControllerIterator IT = GetWorld()->GetPlayerControllerIterator(); IT; ++IT)
	//{
	//	if (APlayerController* PC = IT->Get())
	//	{
	//		// Get the player controller associated with the player controller
	//		if (APawn* PP = PC->GetPawn())
	//		{
	//			if (ABasicMultiplayerPlayerState* PS = Cast<ABasicMultiplayerPlayerState>(PC->PlayerState))
	//			{
	//				ABasicMultiplayerCharacter* PLC = Cast<ABasicMultiplayerCharacter>(PP);
	//				if (PLC && PLC != NewPlayer->GetPawn())
	//				{
	//					PLC->NameSet(PS->GetMyPlayerName());
	//					UE_LOG(LogTemp, Warning, TEXT("Joined Player Name {%s}"), *PS->GetMyPlayerName().ToString());
	//				}
	//			}
	//		}
	//	}
	//}
}