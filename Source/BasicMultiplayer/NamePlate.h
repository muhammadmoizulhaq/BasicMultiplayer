// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NamePlate.generated.h"

/**
* 
*/
UCLASS()
class BASICMULTIPLAYER_API UNamePlate : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	TObjectPtr<class UTextBlock> NameTXT;

	UFUNCTION(BlueprintCallable)
	void SetName(const FText& Name);
};