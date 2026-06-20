// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class UCameraComponent;
class UInteractComponent;
class UHealthComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;
class USoundCue;
class USpotLightComponent;
class UInventoryComponent;
class ABaseInspectable;

UCLASS()
class UNTITLEDHORROR_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	FDelegateHandle SaveDelegateHandle;
	FDelegateHandle LoadDelegateHandle;

	void OnGameSaved(class USaveGameDefault* SaveGame);
	void OnGameLoaded(class USaveGameDefault* SaveGame);

	UPROPERTY(EditAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UInventoryComponent* Inventory;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	ABaseInspectable* CurrentInspectable = nullptr;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	TSubclassOf<UUserWidget> UMG_InspectClass;

	UPROPERTY(EditAnywhere, Category = "Inspect")
	UUserWidget* UMG_Inspect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UHealthComponent* HPComp;

	UPROPERTY(EditAnywhere, Category = "Interact")
	UInteractComponent* InteractComp;

	UPROPERTY(EditAnywhere, Category = "Interact")
	USceneComponent* InteractableItemLocation;

	bool bIsItemAttached = false;
	bool bIsInventoryOpen = false;
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, Category = "FlashLight")
	USpotLightComponent* FlashLightComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FlashLight")
	float MaxBatteryTime = 120.f;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenu_UMGRef;

	UPROPERTY(EditAnywhere, Category = "UI")
	UUserWidget* PauseMenu_UMG;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FlashLight")
	float RemainingBatteryTime = 120.f;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FlashLight")
	bool bIsFlashLightOn = false;

	float FlashLightStartTime = 0.f;

	FTimerHandle FlashLightTimer;

	bool bIsMouseLeftClicked;

	UPROPERTY(EditAnywhere, Category = "FlashLight")
	float FlashLightDuration = 30.0f;
	//INPUT

	//inspect
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Inspect;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ExitInspect;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_MouseWheel;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_MouseMove;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_LeftClick;

    //default
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Default;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_PauseGame;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Crouch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_LookPitch;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_LookYaw;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleFlashLight;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_Interact;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* IA_ToggleInventory;

	//Audio
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* FS_Grass;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* FS_Concrate;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* FS_Snow;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* FS_Metal;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* FS_Wood;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	float FootStepsInterval = 0.6f;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	FTimerHandle FootStepsTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundCue* JumpSound;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
private:
	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);

	void StartJumping();

	void StopJump();

	void StartCrouch();

	void StopCrouch();

	void PauseGame();

	void TryInteract();

	void ToggleFlashLight();

	void ToggleInventory();

	void PlayFootSteps();

	void LookYaw(const FInputActionValue& Value);

	void LookPitch(const FInputActionValue& Value);

	UFUNCTION()
	void ToggleZoom(const FInputActionValue& Value);

	UFUNCTION()
	void ToggleRotation(const FInputActionValue& Value);

	void SetLeftClick();

	void ResetLeftClick();

	void ExitInspect();
public:
	void ReduceHP(int32 Value);

	void Die();

	void AttachActor(AActor* Actor);

	void Inspect(ABaseInspectable* Inspectable);
};
