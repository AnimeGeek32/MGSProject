// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "DeathMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class CODEAI_API UDeathMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	//Shows the pause menu
	void Show();

	//Hides the pause menu
	void Hide();
	
};
