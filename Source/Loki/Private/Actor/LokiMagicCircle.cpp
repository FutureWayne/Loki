// Copyright Ludens Studio


#include "Actor/LokiMagicCircle.h"

#include "Components/DecalComponent.h"

// Sets default values
ALokiMagicCircle::ALokiMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("MagicCircleDecal"));
	MagicCircleDecal->SetupAttachment(RootComponent);
}

void ALokiMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALokiMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

