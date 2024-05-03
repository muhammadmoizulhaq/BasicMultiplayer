// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "BaseInteract.h"
#include "Engine/NetSerialization.h"
#include "Door.generated.h"

class UCurveFloat;

//USTRUCT(BlueprintType)
//struct FDoorRotation
//{
//	GENERATED_BODY()
//
//	UPROPERTY(BlueprintReadOnly, Transient)
//	FVector_NetQuantize DoorRotation;
//
//	FORCEINLINE FDoorRotation() :FDoorRotation(FRotator(0.f, 0.f, 0.f)) {  }
//	FORCEINLINE FDoorRotation(const FRotator& NewDoorRotation) :DoorRotation(NewDoorRotation.Pitch, NewDoorRotation.Yaw, NewDoorRotation.Roll) {  }
//
//	FORCEINLINE const FDoorRotation& operator* (const FDoorRotation& Rotation)
//	{
//		FDoorRotation Rot = FDoorRotation();
//		Rot.DoorRotation = DoorRotation * Rotation.DoorRotation;
//		return Rot;
//	}
//
//	FORCEINLINE const FDoorRotation& operator/ (const FDoorRotation& Rotation)
//	{
//		FDoorRotation Rot = FDoorRotation();
//		Rot.DoorRotation = DoorRotation / Rotation.DoorRotation;
//		return Rot;
//	}
//
//	FORCEINLINE const FDoorRotation& operator+ (const FDoorRotation& Rotation)
//	{
//		FDoorRotation Rot = FDoorRotation();
//		Rot.DoorRotation = DoorRotation + Rotation.DoorRotation;
//		return Rot;
//	}
//
//	FORCEINLINE const FDoorRotation& operator- (const FDoorRotation& Rotation)
//	{
//		FDoorRotation Rot = FDoorRotation();
//		Rot.DoorRotation = DoorRotation - Rotation.DoorRotation;
//		return Rot;
//	}
//
//	FORCEINLINE const FDoorRotation& operator= (const FDoorRotation& Rotation)
//	{
//		DoorRotation = Rotation.DoorRotation;
//		return *this;
//	}
//};

UCLASS()
class BASICMULTIPLAYER_API ADoor : public AActor, public IBaseInteract
{
	GENERATED_BODY()
public:	
	ADoor();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDoorIsOpened(const bool& NewOpened) { bIsDoorOpening = NewOpened; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetDoorIsOpened() const { return bIsDoorOpening; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetDoorRotation(const FRotator& NewDoorRotation) { DoorRotation = NewDoorRotation; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FRotator GetDoorRotation() const { return DoorRotation; }

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Server_ToggleDoor(const FRotator& TargetRotation);

	UFUNCTION(BlueprintCallable, NetMulticast, Unreliable)
	void NetMulticast_ToggleDoor(const FRotator& TargetRotation);

	UFUNCTION(BlueprintCallable)
	void ToggleDoorRotation(const FRotator& TargetRotation);

	virtual void ToggleDoor(const ABasicMultiplayerCharacter* Character) override;
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY()
	bool bIsDoorOpening;

	FRotator StartRotation;

	UPROPERTY(Replicated)
	FRotator DoorRotation;

	float RotationDuration;
	float RotationTimeElapsed;
};