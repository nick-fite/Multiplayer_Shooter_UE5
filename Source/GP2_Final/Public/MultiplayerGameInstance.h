// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MultiplayerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GP2_FINAL_API UMultiplayerGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Name") FString PlayerName;
};
