// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SpotLightComponent.h"
#include "Sound/SoundCue.h"
#include "UntitledHorror/Components/HealthComponent.h"
#include "UntitledHorror/Components/InteractComponent.h"
#include "UntitledHorror/Components/InventoryComponent.h"
#include "UntitledHorror/Interactable/BaseInteractableActor.h"
#include "UntitledHorror/Inspectable/BaseInspectable.h"
#include "UntitledHorror/Subsystems/SaveSubSystem.h"
#include "UntitledHorror/SaveGameDefault.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->bUsePawnControlRotation = true;
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	HPComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HPComp"));
	HPComp->CurrentHP = 100;

	FlashLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("FlashLightComp"));
	FlashLightComp->SetupAttachment(Camera);
	FlashLightComp->SetVisibility(false);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	InteractComp = CreateDefaultSubobject<UInteractComponent>(TEXT("InteractComp"));

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	InteractableItemLocation = CreateDefaultSubobject<USceneComponent>(TEXT("InteractableItemLocation"));
	InteractableItemLocation->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			SubSystem->AddMappingContext(IMC_Default, 0);
	}

	Inventory->ConstructInventory();

	if (UGameInstance* GI = GetGameInstance())
	{
		if (USaveSubSystem* SaveSys = GI->GetSubsystem<USaveSubSystem>())
		{
			SaveDelegateHandle = SaveSys->OnSavedGame.AddUObject(this, &AMainCharacter::OnGameSaved);
			LoadDelegateHandle = SaveSys->OnLoadedGame.AddUObject(this, &AMainCharacter::OnGameLoaded);
			if (SaveSys->GetSavedData() && !SaveSys->GetSavedData()->MapName.IsEmpty())
			{
				UE_LOG(LogTemp, Log, TEXT("Character spawned on loaded level. Applying transform..."));
				OnGameLoaded(SaveSys->GetSavedData());
			}
		}
	}
}

void AMainCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (USaveSubSystem* SaveSys = GI->GetSubsystem<USaveSubSystem>())
		{
			SaveSys->OnSavedGame.Remove(SaveDelegateHandle);
			SaveSys->OnLoadedGame.Remove(LoadDelegateHandle);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void AMainCharacter::OnGameSaved(USaveGameDefault* SaveGame)
{
	if (!SaveGame) return;

	UE_LOG(LogTemp, Warning, TEXT("Character: Saving data via delegate..."));

	SaveGame->PlayerInfo.Location = GetActorLocation();
	SaveGame->PlayerInfo.Rotation = GetActorRotation();
}

void AMainCharacter::OnGameLoaded(USaveGameDefault* SaveGame)
{
	if (!SaveGame) return;

	UE_LOG(LogTemp, Warning, TEXT("Character: Loading data via delegate..."));

	SetActorLocationAndRotation(SaveGame->PlayerInfo.Location, SaveGame->PlayerInfo.Rotation, false, nullptr, ETeleportType::TeleportPhysics);
	GetController()->SetControlRotation(SaveGame->PlayerInfo.Rotation);
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//DEFAULT
		EIC->BindAction(IA_PauseGame, ETriggerEvent::Started, this, &AMainCharacter::PauseGame);
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EIC->BindAction(IA_Move, ETriggerEvent::Completed, this, &AMainCharacter::Move);
		EIC->BindAction(IA_LookYaw, ETriggerEvent::Triggered, this, &AMainCharacter::LookYaw);
		EIC->BindAction(IA_LookPitch, ETriggerEvent::Triggered, this, &AMainCharacter::LookPitch);
		EIC->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMainCharacter::StartJumping);
		EIC->BindAction(IA_Jump, ETriggerEvent::Completed, this, &AMainCharacter::StopJump);
		EIC->BindAction(IA_ToggleFlashLight, ETriggerEvent::Started, this, &AMainCharacter::ToggleFlashLight);
		EIC->BindAction(IA_Interact, ETriggerEvent::Started, this, &AMainCharacter::TryInteract);
		EIC->BindAction(IA_ToggleInventory, ETriggerEvent::Started, this, &AMainCharacter::ToggleInventory);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Started, this, &AMainCharacter::StartCrouch);
		EIC->BindAction(IA_Crouch, ETriggerEvent::Completed, this, &AMainCharacter::StopCrouch);
		//INSPECT
		EIC->BindAction(IA_ExitInspect, ETriggerEvent::Started, this, &AMainCharacter::ExitInspect);
		EIC->BindAction(IA_MouseWheel, ETriggerEvent::Started, this, &AMainCharacter::ToggleZoom);
		EIC->BindAction(IA_MouseMove, ETriggerEvent::Triggered, this, &AMainCharacter::ToggleRotation);
		EIC->BindAction(IA_LeftClick, ETriggerEvent::Started, this, &AMainCharacter::SetLeftClick);
		EIC->BindAction(IA_LeftClick, ETriggerEvent::Completed, this, &AMainCharacter::ResetLeftClick);
	}
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.X != 0.f)
		AddMovementInput(GetActorForwardVector(), MovementVector.X);
	if (MovementVector.Y != 0.f)
		AddMovementInput(GetActorRightVector(), MovementVector.Y);

	if (MovementVector.X != 0.0f || MovementVector.Y != 0.0f)
	{
		if (!GetWorldTimerManager().IsTimerActive(FootStepsTimer))
		{
			PlayFootSteps();
			GetWorldTimerManager().SetTimer(FootStepsTimer, this, &AMainCharacter::PlayFootSteps, FootStepsInterval, true);
		}
	}
	else
		GetWorldTimerManager().ClearTimer(FootStepsTimer);
	
}

void AMainCharacter::StartJumping()
{
	Jump();
}

void AMainCharacter::StopJump()
{
	StopJumping();
	UGameplayStatics::PlaySound2D(GetWorld(), JumpSound);
	MakeNoise();
}

void AMainCharacter::StartCrouch()
{
	bIsCrouching = true;
	Crouch();
}

void AMainCharacter::StopCrouch()
{
	bIsCrouching = false;
	UnCrouch();
}

void AMainCharacter::PauseGame()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
		return;

	PauseMenu_UMG = CreateWidget<UUserWidget>(PC, PauseMenu_UMGRef);
	if (!PauseMenu_UMG)
		return;
	
	PauseMenu_UMG->AddToViewport();
	PC->bShowMouseCursor = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AMainCharacter::TryInteract()
{
	InteractComp->TraceInteract();
}

void AMainCharacter::ToggleFlashLight()
{
	bIsFlashLightOn = !bIsFlashLightOn;

	if (bIsFlashLightOn)
	{
		if (RemainingBatteryTime <= 0.f)
		{
			bIsFlashLightOn = false;

			UE_LOG(LogTemp, Warning, TEXT("Battery Empty"));

			return;
		}

		FlashLightComp->SetVisibility(true);

		FlashLightStartTime = GetWorld()->GetTimeSeconds();

		GetWorldTimerManager().SetTimer(
			FlashLightTimer,
			[this]()
			{
				const float CurrentTime = GetWorld()->GetTimeSeconds();

				const float UsedTime = CurrentTime - FlashLightStartTime;

				RemainingBatteryTime -= UsedTime;

				RemainingBatteryTime = FMath::Clamp(RemainingBatteryTime, 0.f, MaxBatteryTime);

				FlashLightStartTime = CurrentTime;

				UE_LOG(LogTemp, Warning, TEXT("Battery Left: %f"), RemainingBatteryTime);

				if (RemainingBatteryTime <= 0.f)
				{
					bIsFlashLightOn = false;

					FlashLightComp->SetVisibility(false);

					GetWorldTimerManager().ClearTimer(FlashLightTimer);

					UE_LOG(LogTemp, Warning, TEXT("Flashlight Battery Dead"));
				}
			},
			1.0f,
			true
		);
	}
	else
	{
		FlashLightComp->SetVisibility(false);

		const float CurrentTime = GetWorld()->GetTimeSeconds();

		const float UsedTime = CurrentTime - FlashLightStartTime;

		RemainingBatteryTime -= UsedTime;

		RemainingBatteryTime = FMath::Clamp(RemainingBatteryTime, 0.f, MaxBatteryTime);

		GetWorldTimerManager().ClearTimer(FlashLightTimer);

		UE_LOG(LogTemp, Warning, TEXT("Battery Saved: %f"), RemainingBatteryTime);
	}
}

void AMainCharacter::ToggleInventory()
{
	bIsInventoryOpen = !bIsInventoryOpen;
	APlayerController* PC = Cast<APlayerController>(GetController());
	UInventory_UMG* InventoryWidget = Cast<UInventory_UMG>(Inventory->GetInventoryWidget());
	if (!InventoryWidget || !PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("In AMainCharacter::ToggleInventory() InventoryWidget or PC is not valid!"));
		return;
	}
	
	if (bIsInventoryOpen)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->GetCachedWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		PC->bShowMouseCursor = true;
		PC->SetInputMode(InputMode);
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true);
		Inventory->MakeVisible();
	}
	else
	{
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		Inventory->HideInventory();
	}
}

void AMainCharacter::PlayFootSteps()
{
	if (bIsCrouching == true)
		return;

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.bReturnPhysicalMaterial = true;
	Params.AddIgnoredActor(GetOwner());
	FVector Start = GetActorLocation();
	FVector End = FVector(Start.X, Start.Y, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FCollisionShape Sphere = FCollisionShape::MakeSphere(GetCapsuleComponent()->GetScaledCapsuleRadius());

	bool bIsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere, Params);

	if (HitResult.PhysMaterial.IsValid())
	{
		EPhysicalSurface SurfaceType =
			UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());

		switch (SurfaceType)
		{
		case SurfaceType1:
			UE_LOG(LogTemp, Warning, TEXT("Concrate"));
			UGameplayStatics::PlaySound2D(GetWorld(), FS_Concrate);
			MakeNoise();
			break;

		case SurfaceType2:
			UE_LOG(LogTemp, Warning, TEXT("Grass"));
			UGameplayStatics::PlaySound2D(GetWorld(), FS_Grass);
			MakeNoise();
			break;

		case SurfaceType3:
			UE_LOG(LogTemp, Warning, TEXT("Snow"));
			UGameplayStatics::PlaySound2D(GetWorld(), FS_Snow);
			MakeNoise();
			break;
		case SurfaceType4:
			UE_LOG(LogTemp, Warning, TEXT("Metal"));
			UGameplayStatics::PlaySound2D(GetWorld(), FS_Metal);
			MakeNoise();
			break;
		case SurfaceType5:
			UE_LOG(LogTemp, Warning, TEXT("Wood"));
			UGameplayStatics::PlaySound2D(GetWorld(), FS_Wood);
			MakeNoise();
			break;
		}
	}
}

void AMainCharacter::LookYaw(const FInputActionValue& Value)
{
	AddControllerYawInput(Value.Get<float>());
}

void AMainCharacter::LookPitch(const FInputActionValue& Value)
{
	AddControllerPitchInput(Value.Get<float>());
}

void AMainCharacter::ToggleZoom(const FInputActionValue& Value)
{
	float ZoomValue = Value.Get<float>();
	CurrentInspectable->ZoomInOut(ZoomValue);
}

void AMainCharacter::ToggleRotation(const FInputActionValue& Value)
{
	if (bIsMouseLeftClicked == false)
		return;

	const FVector2D Rot = Value.Get<FVector2D>();
	CurrentInspectable->UpdateRotation(Rot);
}

void AMainCharacter::SetLeftClick()
{
	bIsMouseLeftClicked = true;
}

void AMainCharacter::ResetLeftClick()
{
	bIsMouseLeftClicked = false;
}

void AMainCharacter::ExitInspect()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("In AMainCharacter::ExitInspect() PC is not valid!"));
		return;
	}
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(IMC_Default, 0);
		UMG_Inspect->SetVisibility(ESlateVisibility::Hidden);
		PC->bShowMouseCursor = false;
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
		CurrentInspectable->SetUpForGame();
	}
}

void AMainCharacter::ReduceHP(int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("AMainCharacter::ReduceHP is called"));
	HPComp->ReduceHP(Value);
	if (HPComp->CurrentHP <= 0)
		Die();
}

void AMainCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("AMainCharacter::Die() is called"));
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("DeathMenu")));
}

void AMainCharacter::AttachActor(AActor* Actor)
{
	if(!Actor)
		return;

	ABaseInteractableActor* Item = Cast<ABaseInteractableActor>(Actor);
	if (!Item)
		return;

	bIsItemAttached = !bIsItemAttached;

	if(bIsItemAttached)
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsItemAttached is true!"));
		Item->MainMesh->SetSimulatePhysics(false);
		Item->AttachToComponent(InteractableItemLocation, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsItemAttached is false!"));
		UGameplayStatics::PlaySound2D(GetWorld(), Item->GetSound());
		MakeNoise();
		Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Item->MainMesh->SetSimulatePhysics(true);
	}
}

void AMainCharacter::Inspect(ABaseInspectable* Inspectable)
{
	CurrentInspectable = Inspectable;
	if (!CurrentInspectable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentInspectable is not valid!"));
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("PC is not valid!"));
		return;
	}

	UMG_Inspect = CreateWidget<UUserWidget>(PC, UMG_InspectClass);
	if (!UMG_Inspect)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMG_Inspect is not valid!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AMainCharacter::Inspect(ABaseInspectable* Inspectable) is called!"));

	UMG_Inspect->AddToViewport();

		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(IMC_Inspect, 0);
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			InputMode.SetHideCursorDuringCapture(false);
			PC->bShowMouseCursor = true;
			PC->SetInputMode(InputMode);
		}
}

