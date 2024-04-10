// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasicMultiplayerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BASICMULTIPLAYER_API ABasicMultiplayerPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABasicMultiplayerPlayerController();

	UFUNCTION(BlueprintCallable, Server, Unreliable)
	void Server_SetName(const FText& Name);
};