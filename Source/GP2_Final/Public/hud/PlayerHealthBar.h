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
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) int DefaultHealth;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) int CurrentHealth;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite) FText Name;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void SetDefaults();
	UFUNCTION(BlueprintImplementableEvent) void ChangeHealth();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
