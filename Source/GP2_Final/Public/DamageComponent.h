// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP2_FINAL_API UDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly) int Health{100};
public:
	UFUNCTION(BlueprintCallable)
	int GetHealth()
	{
		return Health;
	}

	UFUNCTION(BlueprintCallable) void ChangeHealth(int HealthToAdd)
	{
		Health += HealthToAdd;
	}

	UFUNCTION(BlueprintCallable)
	void SetHealth(int newHealthValue)
	{
		Health = newHealthValue;
	}

	UFUNCTION(BlueprintCallable) void Kill()
	{
		UE_LOG(LogTemp, Warning, TEXT("Player is head"));
	}
};

