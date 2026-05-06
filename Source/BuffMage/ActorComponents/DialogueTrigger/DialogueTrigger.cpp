#include "DialogueTrigger.h"

UDialogueTrigger::UDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UDialogueTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueTrigger::TriggerDialogue_Implementation()
{
	
}


// Called every frame
void UDialogueTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

