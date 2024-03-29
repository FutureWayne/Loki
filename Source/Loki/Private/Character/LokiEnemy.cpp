// Copyright Ludens Studio


#include "Character/LokiEnemy.h"

#include "AbilitySystem/LokiAbilitySystemComponent.h"
#include "AbilitySystem/LokiAbilitySystemLibrary.h"
#include "AbilitySystem/LokiAttributeSet.h"
#include "AI/LokiAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Loki/Loki.h"
#include "Singleton/LokiGameplayTags.h"
#include "UI/Widget/LokiUserWidget.h"

ALokiEnemy::ALokiEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<ULokiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<ULokiAttributeSet>(TEXT("AttributeSet"));

	HealthBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
	HealthBar->SetupAttachment(GetRootComponent());

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void ALokiEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	LokiAIController = Cast<ALokiAIController>(NewController);
	LokiAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	LokiAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	LokiAIController->GetBlackboardComponent()->SetValueAsBool(FName("IsRangedAttacker"), CharacterClass != ECharacterClass::Warrior);
	LokiAIController->RunBehaviorTree(BehaviorTree);
}

void ALokiEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilityActorInfo();

	ULokiAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);

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
	LokiAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
}

int32 ALokiEnemy::GetCharacterLevel()
{
	return Level;
}

void ALokiEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_STENCIL);
}

void ALokiEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
}

void ALokiEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ALokiEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
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
	LokiAIController->BehaviorTreeComponent->StopLogic(TEXT("Enemy Died"));
	Super::Die();
}
