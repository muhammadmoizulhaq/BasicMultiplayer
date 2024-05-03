// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BasicMultiplayerGameMode.generated.h"

UCLASS(minimalapi)
class ABasicMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABasicMultiplayerGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
};