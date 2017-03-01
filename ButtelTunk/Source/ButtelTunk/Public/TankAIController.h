// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

UCLASS()
class BUTTELTUNK_API ATankAIController : public AAIController
{

	GENERATED_BODY()
	
private:
	void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//How close can the AI tank get to the player.
	float AcceptanceRadius = 3000;

};
