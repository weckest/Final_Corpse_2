// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AiControllers/BTService_UpdateTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/AiControllers/BaseAiController.h"


UBTService_UpdateTarget::UBTService_UpdateTarget()
{
	NodeName = TEXT("Update Target");
	bNotifyTick = true;
	Interval = 0.2f;
	RandomDeviation = 0.05f;
}

void UBTService_UpdateTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);
	
	ABaseAiController* AI = Cast<ABaseAiController>(OwnerComp.GetAIOwner());
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!AI || !BB) return;
	
	AActor* Player = AI->GetTarget();
	BB->SetValueAsObject(ABaseAiController::TargetActor, Player);
	BB->SetValueAsBool(ABaseAiController::InAttackRange, Player && AI->IsTargetInRange(Player));
}