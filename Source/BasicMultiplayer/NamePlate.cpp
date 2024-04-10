// Fill out your copyright notice in the Description page of Project Settings.

#include "NamePlate.h"
#include "Components/TextBlock.h"

void UNamePlate::SetName(const FText& Name)
{
	NameTXT->SetText(Name);
}