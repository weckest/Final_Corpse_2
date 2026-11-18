// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AiControllers/BTTask_Attack.h"

#include "Enemies/AiControllers/BaseAiController.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAiController* AI = Cast<ABaseAiController>(OwnerComp.GetAIOwner());
	ABaseEnemy* Enemy = AI ? AI->GetControlledEnemy() : nullptr;
	if (Enemy || Enemy->isDead() || !AI->IsAttackReady())
	{
		return EBTNodeResult::Failed;
	}
	
	Enemy->Attack();
	AI->MarkAttacked();
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
