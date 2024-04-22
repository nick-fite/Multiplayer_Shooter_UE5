// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Crosshair.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class GP2_FINAL_API UCrosshair : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) APlayerCharacter* AttachedPlayer;
};
