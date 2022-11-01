// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoinsChanged, int32, NewCoinsValue, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Economy")
	void AddCoins(int32 CoinDelta);

	UFUNCTION(BlueprintCallable, Category = "Economy")
	bool RemoveCoins(int32 CoinDelta);

	UFUNCTION(BlueprintCallable, Category = "Economy")
	int32 GetNumCoins() const;

	UPROPERTY(BlueprintAssignable, Category = "Economy")
	FOnCoinsChanged OnCoinsChanged;

	UFUNCTION(Client, Unreliable)
	void ClientCoinsChanged(float NewCoins, float Delta);



protected:
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Economy")
	int32 NumCoins;
};
