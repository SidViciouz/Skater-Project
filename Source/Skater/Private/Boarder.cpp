// Fill out your copyright notice in the Description page of Project Settings.


#include "Boarder.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "SkateBoardPawn.h"

// Sets default values
ABoarder::ABoarder()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set push montage
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PushMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Objects/Boarder/Blueprint/PushMontage.PushMontage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KickflipMontageAsset(TEXT("/Script/Engine.AnimMontage'/Game/Objects/Boarder/Blueprint/KickFlipMontage.KickFlipMontage'"));
	PushMontage = PushMontageAsset.Object;
	KickflipMontage = KickflipMontageAsset.Object;

	Capsule = GetCapsuleComponent();

	SkeletalMesh = GetMesh();
	SkeletalMesh->SetRelativeScale3D(FVector(0.85f, 0.85f, 0.85f));
	SkeletalMesh->SetRelativeRotation(FRotator(0, -90.0f, 0));
	SkeletalMesh->SetRelativeLocation(FVector(0, 0, -90.0f));
	SkeletalMesh->SetCollisionProfileName(TEXT("Custom"));
	SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
	SkeletalMesh->SetTickGroup(ETickingGroup::TG_PostPhysics);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeRotation(FRotator(-30.0f, 0, 0));
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);


	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ABoarder::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoarder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsFalling())
		MovementState.SetIsFalling();
	else
		MovementState.ResetIsFalling();

	// move character
	MovementState.Update(this,DeltaTime);

	UpdateRotation();
}

// Called to bind functionality to input
void ABoarder::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("XAxis"), this, &ABoarder::MoveForward);
	PlayerInputComponent->BindAxis(FName("YAxis"), this, &ABoarder::MoveRight);
	PlayerInputComponent->BindAction(FName("LeftShift"), EInputEvent::IE_Pressed, this, &ABoarder::SetIsRun);
	PlayerInputComponent->BindAction(FName("LeftShift"), EInputEvent::IE_Released, this, &ABoarder::ResetIsRun);
	PlayerInputComponent->BindAction(FName("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAxis(FName("MouseX"), this, &ABoarder::TurnYaw);
	PlayerInputComponent->BindAxis(FName("MouseY"), this, &ABoarder::TurnPitch);
}


void ABoarder::MoveForward(float Value)
{
	MovementState.AddInputDirection(GetActorForwardVector() * Value);
}

void ABoarder::MoveRight(float Value)
{
	MovementState.AddInputDirection(GetActorRightVector() * Value);
}

void ABoarder::SetIsRun()
{
	MovementState.SetIsRun();
}

void ABoarder::ResetIsRun()
{
	MovementState.ResetIsRun();
}

void ABoarder::Jump()
{
	Super::Jump();

	MovementState.SetIsJumping();
}

void ABoarder::TurnPitch(float Value)
{
	SpringArm->AddRelativeRotation(FRotator(Value, 0, 0));
}

void ABoarder::TurnYaw(float Value)
{
	AddActorWorldRotation(FRotator(0, Value, 0));
	SkeletalMesh->AddRelativeRotation(FRotator(0, Value, 0));
}

void ABoarder::Detach()
{
	FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	DetachFromActor(DetachRules);
}

void ABoarder::AttachTo(AActor* Actor, FName SocketName)
{
	FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, true);
	AttachToActor(Actor, AttachRule, SocketName);
}

void ABoarder::PresetTobeAttached()
{
	SkeletalMesh->SetCollisionProfileName(TEXT("Custom"));
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

	Capsule->SetCollisionProfileName(TEXT("Pawn"));
}

void ABoarder::PresetToAttachTo()
{
	SkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"));
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	Capsule->SetCollisionProfileName(TEXT("NoCollision"));
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
}

void ABoarder::SetOnBoard(bool Value)
{
	bOnBoard = Value;
}

void ABoarder::UpdateRotation()
{
	if (bOnBoard)
	{
		if (AActor* Parent = GetAttachParentActor())
		{
			SkeletalMesh->SetWorldRotation(FRotator(0, Parent->GetActorRotation().Yaw -90.0f, 0));
		}
	}
}

void ABoarder::PlayPush()
{
	PlayAnimMontage(PushMontage);
}

void ABoarder::PlayJump()
{
	PlayAnimMontage(KickflipMontage);
}

void ABoarder::SetTurnSlowAnim(bool Value)
{
	bTurnSlowAnim = Value;
}

void ABoarder::SetSit(bool Value)
{
	bSit = Value;
}

void ABoarder::SetOllie(bool Value)
{
	bOllie = Value;
}

void ABoarder::SetGrind(bool Value)
{
	bGrind = Value;
}