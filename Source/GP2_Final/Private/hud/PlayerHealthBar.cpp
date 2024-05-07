// Fill out your copyright notice in the Description page of Project Settings.


#include "hud/PlayerHealthBar.h"

#include "Net/UnrealNetwork.h"

void UPlayerHealthBar::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerHealthBar, Name);
	DOREPLIFETIME(UPlayerHealthBar, DefaultHealth);
	DOREPLIFETIME(UPlayerHealthBar, CurrentHealth);
}
