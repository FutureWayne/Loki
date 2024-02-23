// Copyright Ludens Studio


#include "Character/LokiEnemy.h"

#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "AbilitySystem/LokiAbilitySystemLibrary.h"
#include "AbilitySystem/LokiAttributeSet.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Singleton/LokiGameplayTags.h"
#include "UI/Widget/LokiUserWidget.h"

ALokiEnemy::ALokiEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<ULokiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<ULokiAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());
}

void ALokiEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	
	InitAbilityActorInfo();

	ULokiAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent);

	if (ULokiUserWidget* LokiUserWidget = Cast<ULokiUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		LokiUserWidget->SetWidgetController(this);
	}

	if (const ULokiAttributeSet* LokiAttributeSet = CastChecked<ULokiAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LokiAttributeSet->GetHealthAttribute()).
		                        AddLambda([this](const FOnAttributeChangeData& Data)
		                        {
			                        OnHealthChanged.Broadcast(Data.NewValue);
		                        });

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LokiAttributeSet->GetMaxHealthAttribute()).
		                        AddLambda([this](const FOnAttributeChangeData& Data)
		                        {
			                        OnMaxHealthChanged.Broadcast(Data.NewValue);
		                        });

		AbilitySystemComponent->RegisterGameplayTagEvent(FLokiGameplayTags::Get().Effect_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &ALokiEnemy::HitReactTagChanged
		);

		// Broadcast initial values
		OnHealthChanged.Broadcast(LokiAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(LokiAttributeSet->GetMaxHealth());
	}
}

void ALokiEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : 600.f;
	
}

int32 ALokiEnemy::GetCharacterLevel()
{
	return Level;
}

void ALokiEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<ULokiAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

void ALokiEnemy::InitializeDefaultAttributes() const
{
	ULokiAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

void ALokiEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}
