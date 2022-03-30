// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "Engine/World.h"
#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

float UOpenDoor::GetTotalMassOfActors() const
{
	float TotalMass = 0.f;

	if (PressurePlate)
	{
		TArray<AActor *> OverlappingActors;
		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		for (AActor *Actor : OverlappingActors)
		{
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}

	return TotalMass;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	InitComponent(this, &AudioComponent, (void(UOpenDoor::*)()) nullptr);
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActors() >= TargetMass)
	{
		OpenDoor(DeltaTime);
	}
	else if (CurrentYaw > InitialYaw && GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
	{
		CloseDoor(DeltaTime);
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	RotateDoor(DeltaTime, TargetYaw, DoorOpenSpeed);
	DoorLastOpened = GetWorld()->GetTimeSeconds();

	if (AudioComponent && !OpenDoorSoundPlayed)
	{
		OpenDoorSoundPlayed = true;
		CloseDoorSoundPlayed = false;
		AudioComponent->SetPitchMultiplier(2.f);
		AudioComponent->Play();
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	RotateDoor(DeltaTime, InitialYaw, DoorCloseSpeed);

	if (AudioComponent && !CloseDoorSoundPlayed)
	{
		CloseDoorSoundPlayed = true;
		OpenDoorSoundPlayed = false;
		AudioComponent->SetPitchMultiplier(1.f);
		AudioComponent->Play();
	}
}

void UOpenDoor::RotateDoor(float DeltaTime, float FinalYaw, float Speed)
{
	CurrentYaw = FMath::FInterpTo(CurrentYaw, FinalYaw, DeltaTime, Speed);
	FRotator DoorRotation = GetOwner()->GetActorRotation();
	DoorRotation.Yaw = CurrentYaw;
	GetOwner()->SetActorRotation(DoorRotation);
}