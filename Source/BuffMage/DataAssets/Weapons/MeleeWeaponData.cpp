#include "MeleeWeaponData.h"

#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

void UMeleeWeaponData::Attack(FVector StartPos, FRotator Rotation, AActor* Instigator, TArray<AActor*>& ActorsHit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Append(CollisionChannel);

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(Instigator);
	IgnoreActors.Append(ActorsHit);

	UClass* SeekClass = nullptr;
	TArray<AActor*> OutActors;

	if (bDebugActive)
		DrawDebugSphere(Instigator->GetWorld(), StartPos, Range, 12, FColor::Red, true, 100.f, 0, 0);
	UKismetSystemLibrary::SphereOverlapActors(Instigator->GetWorld(), StartPos, Range, traceObjectTypes, SeekClass, IgnoreActors, OutActors);

	if (OutActors.Num() < 1)
		return;

	float ConeDot = -1; // set as the whole circle
	if (AngleDetection != 360)
	{
		// Get the half Angle of the cone and transform it to Radiant
		float RadAngle = (AngleDetection / 2) * (PI / 180);

		ConeDot = FMath::Cos(RadAngle); // get the Cone in dot product 
	}

	FDamageEvent DamageEvent;

	for (AActor* Actor : OutActors)
	{
		if (AngleDetection != 360.f)
		{
			FVector Dir = Actor->GetActorLocation() - Instigator->GetActorLocation();
			Dir.Normalize();
			float Dot = FVector::DotProduct(Instigator->GetActorForwardVector(), Dir);

			if (Dot < ConeDot) // 0.7f
				continue;
		}

		Actor->TakeDamage(Damage, DamageEvent, nullptr, Instigator);
		ActorsHit.Add(Actor);
	}
}
