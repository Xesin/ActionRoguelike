// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

bool ASPlayerState::ApplyCoinChange(int32 Delta)
{
	if (Delta < 0 && NumCoins < Delta) return false;

	int32 OldCoins = NumCoins;

	NumCoins = FMath::Clamp(NumCoins += Delta, 0, TNumericLimits<int32>::Max());

	OnCoinsChanged.Broadcast(NumCoins, Delta);

	return true;
}

int32 ASPlayerState::GetNumCoins() const
{
	return NumCoins;
}
