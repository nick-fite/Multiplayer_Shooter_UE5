// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class GP2_FINAL_API UPlayerHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int DefaultHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Name;
	
	UFUNCTION(BlueprintImplementableEvent) void SetDefaults();
	UFUNCTION(BlueprintImplementableEvent) void ChangeHealth();
	
};
