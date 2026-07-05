#include "CapsuleWeaponData.h"

#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

void UCapsuleWeaponData::Attack(float AddedDamage, FVector StartPos, FRotator Rotation, AActor* Instigator, TArray<AActor*>& ActorsHit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Append(CollisionChannel);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Instigator);
	IgnoreActors.Append(ActorsHit);

	UClass* SeekClass = nullptr;
	TArray<AActor*> OutActors;
	
	if (bDebugActive)
		DrawDebugCapsule(Instigator->GetWorld(), StartPos, HalfHeight, Radius , Rotation.Quaternion(), FColor::Red, true, 100.f);
	
	UKismetSystemLibrary::CapsuleOverlapActorsWithOrientation(Instigator->GetWorld(), StartPos, Radius, HalfHeight, Rotation, CollisionChannel, SeekClass, IgnoreActors, OutActors);
	if (OutActors.Num() < 1)
		return;

	FDamageEvent DamageEvent;

	for (AActor* HitActor : OutActors)
	{
		HitActor->TakeDamage(Damage + AddedDamage, DamageEvent, nullptr, Instigator);
		ActorsHit.Add(HitActor);
	}
}
