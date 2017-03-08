// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtelTunk.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "TankAIController.h"
//Depends on movement component via pathfinding system

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{

	Super::SetPawn(InPawn);

	if (InPawn)
	{
		
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }	

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);

	}

}

void ATankAIController::OnPossessedTankDeath()
{

	if (!GetPawn()) { return; }
	
	GetPawn()->DetachFromControllerPendingDestroy();


}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto ControlledTank = GetPawn();

	if (!ensure(PlayerTank && ControlledTank)) { return; }

	//move towards player
	MoveToActor(PlayerTank, AcceptanceRadius);

	//Aim Towards player
	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::Locked)
	{
		AimingComponent->Fire(); // limit fire rate.
	}

}



