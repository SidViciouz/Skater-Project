// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateBoardMovementComponent.h"

#define DebugBodyContact


// Sets default values for this component's properties
USkateBoardMovementComponent::USkateBoardMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bCustomPhysics = false;
	bLockRotation = false;
}


// Called when the game starts
void USkateBoardMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Init();
}


// Called every frame
void USkateBoardMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateContact();

	if(bCustomPhysics)
		PhysicsTick(DeltaTime);


#ifdef DebugBodyContact

	for (auto& Wheel : WheelData)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Magenta, FString::Printf(TEXT("%s In Touch : %d"), *Wheel.Key.ToString(), (int32)Wheel.Value.bInTouch));
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Magenta, FString::Printf(TEXT("body In Touch : %d"), (int32)BodyData.bInTouch));
#endif

}

void USkateBoardMovementComponent::Init()
{
	// Set AffectedComponent
	if (AffectedComponent == nullptr)
	{
		if (AActor* Parent = GetOwner())
		{
			if (USceneComponent* ParentComponent = Parent->GetRootComponent())
			{
				AffectedComponent = ParentComponent;

				// Set hit delegate
				if (SkeletalComponent = Cast<USkeletalMeshComponent>(AffectedComponent))
				{
					SkeletalComponent->OnComponentHit.AddDynamic(this, &USkateBoardMovementComponent::OnHit);
				}

				// Store wheel collider
				const TArray<TObjectPtr<USceneComponent>>& Children = AffectedComponent->GetAttachChildren();

				for (auto& Child : Children)
				{
					if (UWheelCollision* Wheel = Cast<UWheelCollision>(Child))
					{
						WheelCollision.Add(Wheel->GetWheelBoneName(), Wheel);
						Wheel->OnComponentBeginOverlap.AddDynamic(Wheel, &UWheelCollision::OnOverlapBegin);
						Wheel->OnComponentEndOverlap.AddDynamic(Wheel, &UWheelCollision::OnOverlapEnd);

						//Initialize wheel data
						WheelData.Add(Wheel->GetWheelBoneName());
					}
				}
			}
		}
	}


	// Always ticked after AffectedComponent
	AddTickPrerequisiteComponent(AffectedComponent);
}


void USkateBoardMovementComponent::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	FName closestBone;
	float ClosetDist = 10000.0f;

	TArray<FName> BoneNames;
	SkeletalComponent->GetBoneNames(BoneNames);


	// find hit bone
	for (auto& BoneName : BoneNames)
	{
		FVector BoneLoc = SkeletalComponent->GetBoneLocation(BoneName);

		float Dist = (Hit.ImpactPoint - BoneLoc).Length();

		if (Dist <= ClosetDist)
		{
			ClosetDist = Dist;
			closestBone = BoneName;
		}
	}

	// change wheel state if wheel is hit
	FWheelData* HitWheel = WheelData.Find(closestBone);
	if (HitWheel)
	{
		// only one hit event
		HitWheel->bInTouch = true;
		HitWheel->WorldContactNormal = Hit.ImpactNormal;
		HitWheel->Velocity = HitWheel->Velocity - HitWheel->Velocity.ProjectOnTo(Hit.ImpactNormal);
	}
}

float USkateBoardMovementComponent::GetSpeed() const
{
	return BodyPhysicsProperty.Velocity.Length();
}

FVector USkateBoardMovementComponent::GetVelocity() const
{
	return BodyPhysicsProperty.Velocity;
}

void USkateBoardMovementComponent::SetVelocity(FVector Velocity)
{
	BodyPhysicsProperty.Velocity = Velocity;

	for (auto& OneWheelData : WheelData)
	{
		FWheelData& Wheel = OneWheelData.Value;

		Wheel.Velocity = Velocity;
	}
}

FVector USkateBoardMovementComponent::GetGravity() const
{
	return BodyPhysicsProperty.Gravity;
}

void USkateBoardMovementComponent::ApplyForce(FVector Force, FVector PointOfActionLocation)
{
	// Add force
	BodyPhysicsProperty.Force += Force;

	// Add torque
	//BodyPhysicsProperty.Torque += ((PointOfActionLocation - SkeletalComponent->GetComponentLocation()).Cross(Force) * 0.005f);
}

void USkateBoardMovementComponent::ApplyImpulse(FVector Impulse, FVector PointOfActionLocation)
{
	BodyPhysicsProperty.Velocity += Impulse;
	
	for (auto& OneWheelData : WheelData)
	{
		FName WheelName = OneWheelData.Key;
		FWheelData& Wheel = OneWheelData.Value;

		Wheel.Velocity += Impulse;
	}
	//BodyPhysicsProperty.AngularVelocity += ((PointOfActionLocation - SkeletalComponent->GetComponentLocation()).Cross(Impulse) * 0.0005f);
}


bool USkateBoardMovementComponent::IsInAir()
{
	return bInAir;
}


void USkateBoardMovementComponent::Push()
{
	FVector Forward = FVector::VectorPlaneProject(SkeletalComponent->GetForwardVector(), FVector(0, 0, 1));

	Forward = SkeletalComponent->GetForwardVector();

	float Multipler = 300.0f;

	ApplyImpulse(Forward * Multipler, SkeletalComponent->GetComponentLocation());
}

void USkateBoardMovementComponent::Turn(float Direction)
{
	SkeletalComponent->AddRelativeRotation(FRotator(0,Direction,0), true);

	BodyPhysicsProperty.Velocity = BodyPhysicsProperty.Velocity.RotateAngleAxis(Direction, FVector(0, 0, 1));

	for (auto& OneWheelData : WheelData)
	{
		FName WheelName = OneWheelData.Key;
		FWheelData& Wheel = OneWheelData.Value;

		Wheel.Velocity = Wheel.Velocity.RotateAngleAxis(Direction, FVector(0, 0, 1));;
	}

	DeckData.RollDegree = FMath::FInterpTo(DeckData.RollDegree, Direction * 20.0f, GetWorld()->GetDeltaSeconds(), 2.0f);
}

void USkateBoardMovementComponent::Jump()
{
	SkeletalComponent->AddWorldOffset(FVector(0, 0, 5));
	ApplyImpulse(FVector(0, 0, 300.0f), SkeletalComponent->GetComponentLocation());
}

void USkateBoardMovementComponent::PhysicsTick(float DeltaTime)
{
	// calculate force on wheels
	WheelPhysics(DeltaTime);

	// integrate wheel physics property and move body
	BodyPhysics(DeltaTime);

	// when at least one of the wheels is in contact, rotate the body
	if(!bLockRotation)
		RotateBody();

	// clear force on wheels and body
	ClearForce();

	// when the body is stuck in other object, recover.
	RecoverFromStuck();
}

void USkateBoardMovementComponent::WheelPhysics(float DeltaTime)
{
	for (auto& OneWheelData : WheelData)
	{
		FWheelData& Wheel = OneWheelData.Value;

		// Add gravity
		Wheel.Force += Wheel.Mass * FVector(0, 0, -980.0f);

		// Add Normal force
		FVector NormalForce = Wheel.NormalForce();
		Wheel.Force += NormalForce;

		// Add frictional force
		Wheel.Force += Wheel.FrictionalForce(NormalForce, SkeletalComponent->GetForwardVector());
	}
}

void USkateBoardMovementComponent::BodyPhysics(float DeltaTime)
{

	BodyPhysicsProperty.Velocity = FVector::ZeroVector;

	for (auto& OneWheelData : WheelData)
	{
		FName WheelName = OneWheelData.Key;
		FWheelData& Wheel = OneWheelData.Value;

		BodyPhysicsProperty.Force += Wheel.Force;
		BodyPhysicsProperty.Velocity += Wheel.Velocity * 0.25f;
	}

	// update linear movement
	BodyPhysicsProperty.Velocity += BodyPhysicsProperty.Force * BodyPhysicsProperty.InvMass * DeltaTime;

	FVector DeltaLocation = BodyPhysicsProperty.Velocity * DeltaTime;

	SkeletalComponent->AddWorldOffset(DeltaLocation);


	// update wheel velocity
	for (auto& OneWheelData : WheelData)
	{
		FName WheelName = OneWheelData.Key;
		FWheelData& Wheel = OneWheelData.Value;

		FVector WheelRelativeLocation = SkeletalComponent->GetBoneLocation(WheelName) - SkeletalComponent->GetComponentLocation();

		Wheel.Velocity = BodyPhysicsProperty.Velocity;
	}
}

void USkateBoardMovementComponent::ClearForce()
{
	for (auto& OneWheelData : WheelData)
	{
		OneWheelData.Value.Force = FVector::ZeroVector;
	}

	BodyPhysicsProperty.ClearForce();
}


void USkateBoardMovementComponent::RotateBody()
{
	int WheelsInTouchCnt = 0;
	FVector WheelInAirCOM = FVector::ZeroVector;

	for (auto& OneWheelData : WheelData)
	{
		FName WheelName = OneWheelData.Key;
		FWheelData& Wheel = OneWheelData.Value;

		if (Wheel.bInTouch)
		{
			++WheelsInTouchCnt;
		}
		else
		{
			WheelInAirCOM += SkeletalComponent->GetBoneLocation(WheelName);
		}
	}

	if (WheelsInTouchCnt == 4)
	{
		BodyPhysicsProperty.PrevRotator = FRotator::ZeroRotator;
		return;
	}

	else if (WheelsInTouchCnt == 0)
	{
		SkeletalComponent->AddWorldRotation(BodyPhysicsProperty.PrevRotator * 0.2f);
		return;
	}

	WheelInAirCOM /= (4 - WheelsInTouchCnt);

	FVector COMDir = WheelInAirCOM - SkeletalComponent->GetComponentLocation();
	COMDir.Normalize();

	DrawDebugLine(GetWorld(), SkeletalComponent->GetComponentLocation(), SkeletalComponent->GetComponentLocation() + COMDir * 50.0f, FColor::Green, false, -1, 0, 3.0f);

	FVector Axis = COMDir.Cross(-SkeletalComponent->GetUpVector());

	FVector SafeAxis = Axis.GetSafeNormal();
	FRotator Rotator = FQuat(SafeAxis, FMath::DegreesToRadians(2.0f)).Rotator();

	SkeletalComponent->AddWorldRotation(Rotator);

	BodyPhysicsProperty.PrevRotator = Rotator;
}

void USkateBoardMovementComponent::UpdateContact()
{
	if (SkeletalComponent)
	{
		int InAirWheelCnt = 0;

		// Update wheel contact
		for (auto& OneWheelCollision : WheelCollision)
		{
			FName WheelBone = OneWheelCollision.Key;

			if (OneWheelCollision.Value->bInTouchCount == 0)
			{
				WheelData[WheelBone].bInTouch = false;
				++InAirWheelCnt;
			}
		}

		if (InAirWheelCnt == 4)
			bInAir = true;
		else
			bInAir = false;
	}
}


void USkateBoardMovementComponent::RecoverFromStuck()
{
	for (auto& WheelBone : WheelBoneName)
	{
		FVector End = SkeletalComponent->GetBoneLocation(WheelBone);
		FVector Start = (SkeletalComponent->GetUpVector() * WheelData[WheelBone].StuckRecoveryHeight) + End;

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			float ActualRecoveryHeight = (WheelData[WheelBone].StuckRecoveryHeight + 1.0f - HitResult.Distance);

			if(HitResult.Distance > 10.0f)
				SkeletalComponent->AddWorldOffset(HitResult.Normal * ActualRecoveryHeight);
		}
	}
}