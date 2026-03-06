#include "RangedWeaponData.h"

#include "BuffMage/Interfaces/Poolable/ProjectilePoolable.h"
#include "BuffMage/Subsystems/ObjectPooling/ObjectPoolingSubsystem.h"

void URangedWeaponData::Attack(FVector StartPos, FRotator Rotation, AActor* Instigator, TArray<AActor*>& ActorsHit)
{
	UObjectPoolingSubsystem* PoolingSubsystem = Instigator->GetWorld()->GetSubsystem<UObjectPoolingSubsystem>();
	
	TScriptInterface<IProjectilePoolable> ProjectileInterface = PoolingSubsystem->GetActor(Projectile);

	if (!ProjectileInterface.GetObject())
		return ;

	IProjectilePoolable::Execute_Activate(ProjectileInterface.GetObject(),ProjectileVelocity,Damage,0, Instigator,StartPos,Rotation);
}
