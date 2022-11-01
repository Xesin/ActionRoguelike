// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"

void ASPlayerState::AddCoins(int32 CoinDelta)
{
	if (!ensure(CoinDelta > 0.0f))return; // Only allows positive numbers

	int32 OldCoins = NumCoins;

	NumCoins = FMath::Clamp(NumCoins += CoinDelta, 0, TNumericLimits<int32>::Max());

	ClientCoinsChanged(NumCoins, CoinDelta);

	SetReplicates(true);
}

bool ASPlayerState::RemoveCoins(int32 CoinDelta)
{
	if (!ensure(CoinDelta >= 0.0f)) return false;  // Only allows positive numbers

	if (NumCoins < CoinDelta) return false;

	NumCoins = NumCoins -= CoinDelta;
	OnCoinsChanged.Broadcast(NumCoins, CoinDelta);
	return true;
}

int32 ASPlayerState::GetNumCoins() const
{
	return NumCoins;
}

void ASPlayerState::ClientCoinsChanged_Implementation(float NewCoins, float Delta)
{
	OnCoinsChanged.Broadcast(NewCoins, Delta);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(ASPlayerState, NumCoins);
}