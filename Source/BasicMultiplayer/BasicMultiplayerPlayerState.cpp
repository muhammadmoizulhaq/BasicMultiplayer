// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicMultiplayerPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "BasicMultiplayerCharacter.h"

ABasicMultiplayerPlayerState::ABasicMultiplayerPlayerState()
{
	bReplicates = true;
	NetUpdateFrequency = 100.f;
}

void ABasicMultiplayerPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	/*if (HasAuthority())
	{
		OnRep_MyPlayerName();
	}*/
}

void ABasicMultiplayerPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABasicMultiplayerPlayerState, MyPlayerName);
}

const FText& ABasicMultiplayerPlayerState::GetMyPlayerName() const
{
	return MyPlayerName;
}

void ABasicMultiplayerPlayerState::SetMyPlayerName(const FText& NewName)
{
	MyPlayerName = NewName;
	UE_LOG(LogTemp, Warning, TEXT("Player Name {Replicated Player State}: [%s]"), *GetMyPlayerName().ToString());
}

void ABasicMultiplayerPlayerState::OnRep_MyPlayerName()
{
	if (ABasicMultiplayerCharacter* C = Cast<ABasicMultiplayerCharacter>(GetPawn()))
	{
		C->NameSet(GetMyPlayerName());
	}
}