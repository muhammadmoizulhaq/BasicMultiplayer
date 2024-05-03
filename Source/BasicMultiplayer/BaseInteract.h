// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseInteract.generated.h"

class ABasicMultiplayerCharacter;

UINTERFACE(MinimalAPI)
class UBaseInteract : public UInterface
{
	GENERATED_BODY()
};

class BASICMULTIPLAYER_API IBaseInteract
{
	GENERATED_BODY()
public:
	virtual void ToggleDoor(const ABasicMultiplayerCharacter* Character) = 0;
};