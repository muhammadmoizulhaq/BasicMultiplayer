// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "Net/UnrealNetwork.h"

ADoor::ADoor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bNetLoadOnClient = true;
	SetReplicateMovement(true);
	
	bIsDoorOpening = false;
	DoorRotation = FRotator();
	StartRotation = FRotator();
	RotationDuration = 1.f; // 1 Second
	RotationTimeElapsed = 0.f;
}

void ADoor::NetMulticast_ToggleDoor_Implementation(const FRotator& TargetRotation)
{
	if (!bIsDoorOpening)
	{
		bIsDoorOpening = true;
		DoorRotation = GetActorRotation();
		DoorRotation = TargetRotation;
		RotationTimeElapsed = 0.f;
	}
}

void ADoor::ToggleDoorRotation(const FRotator& TargetRotation)
{
	Server_ToggleDoor(TargetRotation);
}

void ADoor::ToggleDoor(const ABasicMultiplayerCharacter* Character)
{
	const FRotator& Rot = FRotator(FRotator(0.f, 90.f, 0.f));
	ToggleDoorRotation(Rot);
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
}

void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsDoorOpening)
	{
		RotationTimeElapsed += DeltaTime;
		float Alpha = FMath::Clamp(RotationTimeElapsed / RotationDuration, 0.0f, 1.0f);
		FRotator DoorRot = FMath::Lerp(StartRotation, DoorRotation, Alpha);
		SetActorRotation(DoorRotation);
		if (RotationTimeElapsed >= RotationDuration)
		{
			bIsDoorOpening = false;
		}
	}
}

void ADoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADoor, DoorRotation);
}

void ADoor::Server_ToggleDoor_Implementation(const FRotator& TargetRotation)
{
	NetMulticast_ToggleDoor(TargetRotation);
}