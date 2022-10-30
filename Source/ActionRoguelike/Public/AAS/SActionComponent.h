// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SActionComponent.generated.h"

class USAction;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USActionComponent();	

	virtual void BeginPlay() override;

	UFUNCTION(BLueprintCallable, Category = "Actions")
	void AddAction(TSubclassOf<USAction> ActionClass);

	UFUNCTION(BLueprintCallable, Category = "Actions")
	bool StartActionByName(AActor* Instigator, FName ActionName);

	UFUNCTION(BLueprintCallable, Category = "Actions")
	bool StopActionByName(AActor* Instigator, FName ActionName);

protected:
	UPROPERTY()
	TArray<USAction*> Actions;

	UPROPERTY(EditAnywhere, Category = "Actions")
	TArray<TSubclassOf<USAction>> DefaultActions;

};
