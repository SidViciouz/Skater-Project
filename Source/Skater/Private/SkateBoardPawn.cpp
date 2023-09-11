// Fill out your copyright notice in the Description page of Project Settings.


#include "SkateBoardPawn.h"

FName ASkateBoardPawn::SkateSkeletalMeshName(TEXT("SkateBoardSkeletalMesh"));
FName ASkateBoardPawn::MovementComponentName(TEXT("SkateBoardMovementComponent"));
FName ASkateBoardPawn::WheelFRBoneName(TEXT("Wheel_FR"));
FName ASkateBoardPawn::WheelFLBoneName(TEXT("Wheel_FL"));
FName ASkateBoardPawn::WheelRRBoneName(TEXT("Wheel_RR"));
FName ASkateBoardPawn::WheelRLBoneName(TEXT("Wheel_RL"));

// Sets default values
ASkateBoardPawn::ASkateBoardPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create skate board skeletal mesh
	SkateSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(SkateSkeletalMeshName);
	SkateSkeletalMesh->SetAllBodiesNotifyRigidBodyCollision(true);
	SkateSkeletalMesh->SetCollisionProfileName(TEXT("Custom"));
	SkateSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkateSkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	SkateSkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SkateSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	SkateSkeletalMesh->SetTickGroup(ETickingGroup::TG_PostPhysics);

	SetRootComponent(SkateSkeletalMesh);

	// create movement component
	MovementComponent = CreateDefaultSubobject<USkateBoardMovementComponent>(MovementComponentName);

	// Custom pass component

	CustomPass = CreateDefaultSubobject<UCustomPassComponent>(TEXT("Custom Pass"));

	// Tutorial component
	TutorialComponent = CreateDefaultSubobject<UTutorialComponent>(TEXT("TutorialComponent"));

	// Initialize Wheel Collider
	UWheelCollision* WheelCollisionComponent = CreateDefaultSubobject<UWheelCollision>(WheelFRBoneName);
	WheelCollisionComponent->SetWheelBoneName(WheelFRBoneName);
	WheelCollisionComponent->SetupAttachment(SkateSkeletalMesh, WheelFRBoneName);
	WheelCollider.Add(WheelFRBoneName, WheelCollisionComponent);

	WheelCollisionComponent = CreateDefaultSubobject<UWheelCollision>(WheelFLBoneName);
	WheelCollisionComponent->SetWheelBoneName(WheelFLBoneName);
	WheelCollisionComponent->SetupAttachment(SkateSkeletalMesh, WheelFLBoneName);
	WheelCollider.Add(WheelFLBoneName, WheelCollisionComponent);

	WheelCollisionComponent = CreateDefaultSubobject<UWheelCollision>(WheelRRBoneName);
	WheelCollisionComponent->SetWheelBoneName(WheelRRBoneName);
	WheelCollisionComponent->SetupAttachment(SkateSkeletalMesh, WheelRRBoneName);
	WheelCollider.Add(WheelRRBoneName, WheelCollisionComponent);

	WheelCollisionComponent = CreateDefaultSubobject<UWheelCollision>(WheelRLBoneName);
	WheelCollisionComponent->SetWheelBoneName(WheelRLBoneName);
	WheelCollisionComponent->SetupAttachment(SkateSkeletalMesh, WheelRLBoneName);
	WheelCollider.Add(WheelRLBoneName, WheelCollisionComponent);

	// Set anim montage object
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KickflipMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Objects/SkateBoard/Blueprint/SBKickFlipMontage.SBKickFlipMontage'"));
	KickflipMontage = KickflipMontageAsset.Object;

	// Animation states
	bOllie = false;
	bBoarderSit = false;
	bGrindable = false;
	bGrind = false;
	bRootMotion = true;
}

// Called when the game starts or when spawned
void ASkateBoardPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASkateBoardPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Process Root Motion
	if (bRootMotion)
	{
		FRootMotionMovementParams RootMotion = SkateSkeletalMesh->ConsumeRootMotion();

		if (RootMotion.bHasRootMotion)
		{
			SetLockRotation(true);

			FVector LocOffset = RootMotion.GetRootMotionTransform().GetLocation();
			FQuat RotOffset = RootMotion.GetRootMotionTransform().GetRotation();

			SkateSkeletalMesh->AddLocalRotation(RotOffset);
			SkateSkeletalMesh->AddWorldOffset(LocOffset);
		}
		else
		{
			SetLockRotation(false);
		}
	}

	// Grind Update
	if (bGrind)
		GrindUpdate(DeltaTime);
	
	if (MovementComponent->GetSpeed() > 1000.0f)
		CustomPass->SetPassEnabled(true);
	else
		CustomPass->SetPassEnabled(false);

}

// Called to bind functionality to input
void ASkateBoardPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Push"), EInputEvent::IE_Pressed, this, &ASkateBoardPawn::PlayPush);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ASkateBoardPawn::PlayJump);
	PlayerInputComponent->BindAction(FName("Sit"), EInputEvent::IE_Pressed, this, &ASkateBoardPawn::Sit);
	PlayerInputComponent->BindAction(FName("Sit"), EInputEvent::IE_Released, this, &ASkateBoardPawn::Stand);
	PlayerInputComponent->BindAction(FName("Ollie"), EInputEvent::IE_Pressed, this, &ASkateBoardPawn::Ollie);
	PlayerInputComponent->BindAxis(FName("YAxis"), this, &ASkateBoardPawn::Turn);

}

void ASkateBoardPawn::SetCustomPhysics(bool Value)
{
	MovementComponent->bCustomPhysics = Value;
}

void ASkateBoardPawn::SetLockRotation(bool Value)
{
	MovementComponent->bLockRotation = Value;
}

void ASkateBoardPawn::SetCollision(bool Value)
{
	if(Value)
		SkateSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	else
		SkateSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASkateBoardPawn::EnableRootMotion()
{
	bRootMotion = true;
}

void ASkateBoardPawn::DisableRootMotion()
{
	bRootMotion = false;
}

float ASkateBoardPawn::PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	UAnimInstance* AnimInstance = (SkateSkeletalMesh) ? SkateSkeletalMesh->GetAnimInstance() : nullptr;
	if (AnimMontage && AnimInstance)
	{
		float const Duration = AnimInstance->Montage_Play(AnimMontage, InPlayRate);

		if (Duration > 0.f)
		{
			if (StartSectionName != NAME_None)
			{
				AnimInstance->Montage_JumpToSection(StartSectionName, AnimMontage);
			}

			return Duration;
		}
	}

	return 0.f;
}

void ASkateBoardPawn::Detach()
{
	FDetachmentTransformRules DetachRule(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRule);
}

void ASkateBoardPawn::AttachTo(USceneComponent* SceneComponent, FName SocketName)
{
	FAttachmentTransformRules Rule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	AttachToComponent(SceneComponent, Rule, SocketName);
}

void ASkateBoardPawn::PresetTobeAttached()
{
	SetCustomPhysics(true);
	SetCollision(true);
}

void ASkateBoardPawn::PresetToAttachTo()
{
	SetCollision(false);
	SetCustomPhysics(false);
}

void ASkateBoardPawn::PlayPush()
{
	TutorialComponent->OnButtonPress.Broadcast('W');

	if (Boarder)
		Boarder->PlayPush();	
}

void ASkateBoardPawn::Push()
{
	MovementComponent->Push();
}

void ASkateBoardPawn::PlayJump()
{
	if(Boarder)
		Boarder->PlayJump();

	PlayAnimMontage(KickflipMontage);

	Jump();
}

void ASkateBoardPawn::Jump()
{
	MovementComponent->Jump();
}

void ASkateBoardPawn::Turn(float Direction)
{
	float BoardSpeed = MovementComponent->GetSpeed();

	if (Direction != 0.0f && BoardSpeed < 500.0f)
	{
		Boarder->StopAnimMontage();

		// Set board animation infos
		SetLockRotation(true);
		bSlowTurnAnim = true;
		if (Direction > 0.0f)
			bSlowTurnRight = true;
		else
			bSlowTurnRight = false;

		// Set boarder animation infos
		Boarder->SetTurnSlowAnim(true);
	}
	else
	{
		SetLockRotation(false);
		bSlowTurnAnim = false;
		MovementComponent->Turn(Direction);

		Boarder->SetTurnSlowAnim(false);
	}
}

void ASkateBoardPawn::Sit()
{
	Boarder->StopAnimMontage();

	bBoarderSit = true;
	Boarder->SetSit(true);
}

void ASkateBoardPawn::Stand()
{
	Boarder->StopAnimMontage();

	bBoarderSit = false;
	Boarder->SetSit(false);
}

void ASkateBoardPawn::Ollie()
{
	if (bBoarderSit)
	{
		Boarder->StopAnimMontage();

		Boarder->SetOllie(true);

		bOllie = true;

		Jump();
	}
}

void ASkateBoardPawn::SetGrindable(AGrindable* Grindable, float Distance)
{
	bGrindable = true;

	if (!bGrind && bOllie)
	{
		GridnableObject = Grindable;
		Grind(Distance);
	}
}

void ASkateBoardPawn::ResetGrindable()
{
	if (bGrind)
	{
		GrindOff();
	}
	
	bGrindable = false;
	GridnableObject = nullptr;
}

void ASkateBoardPawn::Grind(float Distance)
{
	Boarder->StopAnimMontage();

	Boarder->SetGrind(true);

	bGrind = true;

	SetCustomPhysics(false);
	DisableRootMotion();

	FVector Velocity = MovementComponent->GetVelocity();

	GrindDistance = Distance;

	USplineComponent* Spline = GridnableObject->Spline;

	FVector Tangent = Spline->GetTangentAtDistanceAlongSpline(GrindDistance, ESplineCoordinateSpace::World);

	Velocity = Velocity.ProjectOnTo(Tangent);
	MovementComponent->SetVelocity(Velocity);

	if (Velocity.Dot(Tangent) < 0.0f)
	{
		GrindSpeed = -Velocity.Length();
	}
	else
	{
		GrindSpeed = Velocity.Length();
	}

	FVector HitLocation = Spline->GetLocationAtDistanceAlongSpline(GrindDistance, ESplineCoordinateSpace::World);

	FVector Target = FMath::VInterpConstantTo(GetActorLocation(), HitLocation, GetWorld()->GetDeltaSeconds(), 500.0f);

	SetActorLocation(Target);

}

void ASkateBoardPawn::GrindOff()
{
	Boarder->SetGrind(false);

	bGrind = false;

	SetCustomPhysics(true);
	EnableRootMotion();
}


void ASkateBoardPawn::GrindUpdate(float DeltaTime)
{
	USplineComponent* Spline = GridnableObject->Spline;

	FVector Tangent = Spline->GetTangentAtDistanceAlongSpline(GrindDistance, ESplineCoordinateSpace::World);

	FVector Velocity = MovementComponent->GetVelocity();
	Velocity += MovementComponent->GetGravity() * DeltaTime;

	Velocity = Velocity.ProjectOnTo(Tangent);
	MovementComponent->SetVelocity(Velocity);
	
	if (Velocity.Dot(Tangent) < 0.0f)
		GrindSpeed = -Velocity.Length();
	else
		GrindSpeed = Velocity.Length();

	GrindDistance += GrindSpeed * DeltaTime;

	FVector NextLocation = Spline->GetLocationAtDistanceAlongSpline(GrindDistance, ESplineCoordinateSpace::World);

	FVector Target = FMath::VInterpConstantTo(GetActorLocation(), NextLocation, GetWorld()->GetDeltaSeconds(), 500.0f);

	SetActorLocation(Target);

	//SetActorLocation(NextLocation);
	
	if (GrindDistance >= Spline->GetSplineLength() || GrindDistance <= 0.0f)
		GrindOff();
}

void ASkateBoardPawn::PowerSlide()
{
	bPowerSlide = true;
}