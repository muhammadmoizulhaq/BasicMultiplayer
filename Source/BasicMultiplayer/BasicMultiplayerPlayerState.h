// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BasicMultiplayerPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASICMULTIPLAYER_API ABasicMultiplayerPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABasicMultiplayerPlayerState();

	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	const FText& GetMyPlayerName() const;

	UFUNCTION(BlueprintCallable)
	void SetMyPlayerName(const FText& NewName);
private:
	UPROPERTY(ReplicatedUsing = "OnRep_MyPlayerName")
	FText MyPlayerName;

	UFUNCTION()
	void OnRep_MyPlayerName();
};