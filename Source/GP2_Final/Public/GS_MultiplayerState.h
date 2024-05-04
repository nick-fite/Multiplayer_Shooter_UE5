// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GS_MultiplayerState.generated.h"

/**
 * 
 */
UCLASS()
class GP2_FINAL_API AGS_MultiplayerState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void TestPrint()
	{
		UE_LOG(LogTemp, Warning, TEXT("print from game state"));
	}
};
