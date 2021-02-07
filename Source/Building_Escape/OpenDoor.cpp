// Copyright Bushuev Zakhar 2021


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	DoorZeroPosition = GetOwner()->GetActorRotation().Yaw;
	
	TargetYaw -= 5;
	
	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(PressurePlate)
	{
		if(PressurePlate->IsOverlappingActor(ActorThatOpen))
		{
			// UE_LOG(LogTemp, Warning, TEXT("Player enter to trigger"));
			RotateDoor(DeltaTime, true);
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
		else
		{
			if(GetWorld()->GetTimeSeconds() >= DoorLastOpened + CloseDelay)
			{
				RotateDoor(DeltaTime, false);
			}	
		}
	}
}

void UOpenDoor::RotateDoor(float DeltaTime, bool isOpening)
{
	// UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetActorRotation().ToString());
	// UE_LOG(LogTemp, Warning, TEXT("Yaw is: %f"), );
	float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	float UpdatedYaw = isOpening ? FMath::FInterpConstantTo(CurrentYaw, TargetYaw, DeltaTime, OpenSpeed)
		: FMath::FInterpConstantTo(CurrentYaw, DoorZeroPosition, DeltaTime, CloseSpeed);
	
	GetOwner()->SetActorRotation({ 0.f, UpdatedYaw, 0.f });
}


