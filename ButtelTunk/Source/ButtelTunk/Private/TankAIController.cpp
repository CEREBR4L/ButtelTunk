// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtelTunk.h"
#include "Tank.h"
#include "TankAIController.h"
//Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

}

void ATankAIController::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);

	PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	ControlledTank = Cast<ATank>(GetPawn());

	if(ensure(PlayerTank))
	{
		//move towards player
		MoveToActor(PlayerTank, AcceptanceRadius);

		//Aim Towards player
		ControlledTank->AimAt(PlayerTank->GetActorLocation());

		//Fire if ready
		ControlledTank->Fire(); // limit fire rate.

	}

}


