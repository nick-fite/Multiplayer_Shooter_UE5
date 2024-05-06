// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerGameMode.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class GP2_FINAL_API AMultiplayerGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent) void RespawnPlayer(AController* playerController);
};
