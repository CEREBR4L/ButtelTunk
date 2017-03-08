// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtelTunk.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "TankPlayerController.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)){ return; }

	FoundAimingComponent(AimingComponent);

}

void ATankPlayerController::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	AimTowardsCrosshair();

}


void ATankPlayerController::SetPawn(APawn* InPawn)
{

	Super::SetPawn(InPawn);

	if (InPawn)
	{

		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);

	}

}

void ATankPlayerController::OnTankDeath()
{

	StartSpectatingOnly();

}

void ATankPlayerController::AimTowardsCrosshair()
{
	
	if (!GetPawn()) { return; } // e.g. if not possessing

	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }

	FVector HitLocation;
	bool bGotHitLoc = GetSightRayHitLocation(HitLocation);
	if (bGotHitLoc) //Side-effect: it is going to ray-trace
	{
		AimingComponent->AimAt(HitLocation);
	}

}

///Get world location of linetrace through crosshair, if true hits landscape.
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	//find crosshair pos in px coords  
	int32 ViewPortSizeX, ViewPortSizeY;
	GetViewportSize(ViewPortSizeX, ViewPortSizeY);

	auto ScreenLocation = FVector2D(ViewPortSizeX * CrosshairXLocation, ViewPortSizeY * CrosshairYLocation);

	// deproject screen pos of cursor to world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//line trace along LookDirection - see what we hit

		return GetLookVectorHitLocation(LookDirection, HitLocation);

	}

	return false;

}


bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{

	FVector CameraWorldLocation; // To be discarded 
	return DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection
	);
	
}


bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{

	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			StartLocation,
			EndLocation,
			ECollisionChannel::ECC_Camera
		)
	)
	{
		HitLocation = HitResult.Location;
		return true;
	}

	HitLocation = FVector(0.f);
	return false;

}
