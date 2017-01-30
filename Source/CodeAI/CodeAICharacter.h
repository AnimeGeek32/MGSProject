// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "CodeAICharacter.generated.h"

UCLASS(config = Game)
class ACodeAICharacter : public ACharacter
{
	GENERATED_BODY()

private:

	/******************************************
	*		COMPONENTS
	******************************************/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TransitionCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* TopDownBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* LeftBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* LeftCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* RightBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* RightCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* DeathBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* DeathCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FPPCamera;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* MenuSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* KnockingSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* PlayerHurt_1;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* PlayerHurt_2;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* PlayerDead;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* PlayerCover;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* InventoryOpen;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* ItemEquip;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundBase* ItemUnequip;

	UPROPERTY(EditDefaultsOnly)
		TArray<UAnimationAsset*> DeathAnimations;
	UPROPERTY(EditDefaultsOnly)
		TArray<UAnimSequence*> StaggerAnimations;

public:

	ACodeAICharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/******************************************
	*		COVER
	******************************************/

	//Performs a line trace and returns whether the hit result was a blocking hit
	bool InCoverLeftLineTrace(bool bToTheLeft, bool bForward);
	//Handles the logic to determine whether or not the player is in cover and
	//from which direction the cover was triggered
	void HandleCoverLogic();
	//Handles variable changes when the player leaves cover
	void OnEndCover();
	//Handles the line traces to check whether or not the player is at the edge of cover
	void HandleCoverLineTrace();
	//"Sub-function" executed by the HandleCoverLineTrace() function
	void HandleForwardCover();
	//"Sub-function" executed by the HandleCoverLineTrace() function
	void HandleRightCover();
	//Toggles between the top-down and front cameras
	void ToggleCamera(bool bDefaultCamera, bool bLeftCamera = false);

	//Returns whether or not the player is in cover
	UFUNCTION(BlueprintCallable, Category = Cover)
		FORCEINLINE bool InCover() const { return bIsInCover; }


	/******************************************
	*		ITEMS
	******************************************/

	//The array that stores the player's items
	UPROPERTY(BlueprintReadOnly, Category = Inventory)
		TArray<class AInventoryGameItem*> InventoryArray;

	bool AddItem(class AGameItem* Item);
	bool AddNewItem(class AInventoryGameItem* NewItem);
	bool AddAmmoItem(class AAmmoItem* AmmoItem);
	bool AddWeapon(class AWeaponItem* WeaponItem);
	bool AddStackable(class AStackableGameItem* StackableItem);
	void EquipWeapon(class AWeaponItem* WeaponItem);
	//Called from the Menu widget to set the new index
	UFUNCTION(BlueprintCallable, Category = Item)
		void SetEquippedIndex(int32 Index);
	//Called to set the new index
	void SetIndex(int32 Index);
	
	//Returns whether or not a pistol is equipped
	UFUNCTION(BlueprintCallable, Category = Weapon)
		FORCEINLINE bool IsPistolEquipped() const { return bPistolEquipped; }
	//Returns whether or not a rifle is equipped
	UFUNCTION(BlueprintCallable, Category = Weapon)
		FORCEINLINE bool IsRifleEquipped() const { return bRifleEquipped; }
	//Returns the index of the currently equipped item
	UFUNCTION(BlueprintCallable, Category = Inventory)
		FORCEINLINE int32 GetEquippedIndex() const { return EquippedIndex; }
	//Returns the player's inventory
	FORCEINLINE TArray<class AInventoryGameItem*> GetInventory() const { return InventoryArray; }

	/******************************************
	*		HEALTH
	******************************************/

	//The player's health on a scale of 0 to 1
	UPROPERTY(BlueprintReadOnly, Category = Health)
		float HUDHealth;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser);
	void OnDeath();
	void SwitchToDeathCamera();
	UFUNCTION(BlueprintCallable, Category = Health)
		virtual void AddHealth(float Regen);
	void AddEnemy(class AMyAICharacter* NewEnemy);
	void RemoveEnemy(class AMyAICharacter* NewEnemy);

	//Returns the player's health
	UFUNCTION(BlueprintCallable, Category = Health)
		FORCEINLINE float GetHealth() const { return Health; }
	UFUNCTION(BlueprintCallable, Category = Health)
		FORCEINLINE bool FullHealth() const { return Health == MaxHealth; }
	UFUNCTION(BlueprintCallable, Category = Health)
		FORCEINLINE bool IsDead() const { return bIsDead; }


	/******************************************
	*		MOVEMENT
	******************************************/

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;
	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	//Called when the player's movement is being blocked by an actor
	virtual void MoveBlockedBy(const FHitResult & Impact) override;

	//Returns whether or not the player is walking
	UFUNCTION(BlueprintCallable, Category = Movement)
		FORCEINLINE	bool GetIsWalking() const { return bIsWalking; }
	UFUNCTION(BlueprintCallable, Category = Movement)
		FORCEINLINE	bool GetIsCrouching() const { return bIsCrouching; }
	UFUNCTION(BlueprintCallable, Category = Movement)
		FORCEINLINE	bool GetIsProne() const { return bIsProne; }


	/******************************************
	*		NOISE
	******************************************/

	/*Plays a sound in-game and reports it to entities who have a UPawnSensingComponent (AI)*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPawnNoiseEmitterComponent* PawnNoiseEmitterComp;

	UFUNCTION(BlueprintCallable, Category = Sound)
		void ReportNoise(USoundBase* SoundToPlay, float Volume);

	//Used to notify all the bots that there was a loud noise
	void NotifyLoudNoise(FVector Loc);


private:

	/******************************************
	*		ITEMS
	******************************************/

	//Indicates the index of the currently equipped item
	int32 EquippedIndex;
	//Indicates the index of the previously equipped item
	int32 PreviousIndex;
	//Used to keep track of menu navigation
	int32 ExtraIndex;


	/******************************************
	*		MOVEMENT
	******************************************/

	//Indicates the factor at which the player's speed decreases when walking
	float WalkSpeedDecrease;
	//Registers the X component of the mouse control
	float XRate;
	//Registers the Y component of the mouse control
	float YRate;

	//Used to time the crouch movement
	FTimerHandle CrouchHandle;
	//Keeps track of the player's movement while crouching
	float HorizontalCrouchMov;
	float VerticalCrouchMov;
	//The amount of time to wait before the player goes prone after crouching
	UPROPERTY(EditAnywhere)
		float CrouchWait;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.2", ClampMax = "6.0", UIMin = "0.2", UIMax = "6.0"))
		float ProneMovement;

	FTimerHandle AnimationHandle;
	UPROPERTY(EditAnywhere)
		UAnimSequence* ProneMontage;
	UPROPERTY(EditAnywhere)
		float ProneMontageLength;
	UPROPERTY(EditAnywhere)
		UAnimSequence* CrouchMontage;
	UPROPERTY(EditAnywhere)
		float CrouchMontageLength;

	//Determines whether the player is sprinting or not
	uint8 bIsWalking : 1;
	//Determines whether the player can move or not
	uint8 bAllowMovement : 1;
	//Determines whether the player is crouching
	uint8 bIsCrouching : 1;
	//Determines whether the player is pressing the crouching button
	uint8 bIsCrouchPressed : 1;
	//Determines whether the player is prone
	uint8 bIsProne: 1;
	//Determines whether the player is staggering
	uint8 bIsStaggering : 1;

	//A reference to the world camera currently being used
	UCameraComponent* TemporaryCamera;


	/******************************************
	*		COVER
	******************************************/

	//Indicates the factor at which the player's speed decreases when in cover
	UPROPERTY(EditAnywhere, Category = Cover)
		float CoverSpeedDecrease;
	//Indicates the distance to the side of the player used to make
	//the line trace from - HandleCoverLineTrace() function
	UPROPERTY(EditAnywhere, Category = Cover)
		float CoverPeakDistance;
	//Registers the character's forward movement on tick
	float ForwardMov;
	//Registers the character's right movement on tick
	float RightMov;
	//Registers the amount of movement that is "illegal" while in cover
	float NoMov;

	//Determines whether the player is in cover or not
	uint8 bIsInCover : 1;
	//Determines when to exit cover from a forward movement
	uint8 bShouldBeInCoverForward : 1;
	//Determines when to exit cover from a right movement
	uint8 bShouldBeInCoverRight : 1;
	//Determines which direction the NoMov applies to
	uint8 bNoMovForward : 1;


	/******************************************
	*		MENU
	******************************************/

	/*Registers for how long the player has to hold down the menu button
	in order for the menu to pop up*/
	float MenuTimer;
	UPROPERTY(EditAnywhere)
		float MenuHeldDownTime;

	//Determines whether the left menu is open or closed
	uint8 bLeftMenuOpen : 1;
	//Determines whether the right menu is open or closed
	uint8 bRightMenuOpen : 1;
	//Determines whether the player pressed the left menu button
	uint8 bLeftMenuPressed : 1;
	//Determines whether the player is holding down the menu button
	uint8 bHeldDownMenu : 1;
	//Determines whether the player can navigate through the inventory
	uint8 bAllowNavigation : 1;
	//Determines whether the player is using a First Person Camera
	uint8 bUsingFPP : 1;

	/******************************************
	*		HEALTH
	******************************************/

	TArray<class AMyAICharacter*> AttackingEnemies;
	TArray<class AMyAICharacter*> AllEnemies;

	//The player's health
	float Health;
	//The player's max health
	UPROPERTY(EditAnywhere)
		float MaxHealth;

	//Determines whether the player is dead
	uint8 bIsDead : 1;

	/******************************************
	*		ITEMS
	******************************************/

	//Whether or not the player has a pistol equipped
	uint8 bPistolEquipped : 1;
	//Whether or not the player has a rifle equipped
	uint8 bRifleEquipped : 1;
	//Determines whether an item is equipped or not
	uint8 bItemEquipped : 1;
	//Determines whether the player should add the picked up item to the inventory
	uint8 bShouldAddItem : 1;

protected:

	/******************************************
	*		INPUT
	******************************************/

	virtual void SetupPlayerInputComponent(class UInputComponent* inputComponent) override;
	//Handles the player's input over the menu
	void HandleMenuInput(float Value);
	//Switches between firing a weapon and moving regurarly
	void ToogleCharacterControls(bool bAllow);
	//Handles the player's rotation when using an item
	void HandlePlayerRotation();
	//Called when the player presses the left menu button
	void PressedLeftMenu();
	//Called when the player holds the left menu button
	void ShowLeftMenu();
	//Called when the player releases the left menu button
	void HideLeftMenu();
	//Called when the player presses the right menu button
	void PressedRightMenu();
	//Called when the player holds the right menu button
	void ShowRightMenu();
	//Called when the player releases the right menu button
	void HideRightMenu();
	//Called when the action button is pressed
	void ActionPressed();
	//Called when the action button is released
	void ActionReleased();
	//Called when the use button is pressed
	void UsePressed();
	//Called when the use button is released
	void UseReleased();
	//Called when the reload button is pressed
	void ReloadPressed();
	//Called when the reload button is released
	void ReloadReleased();
	//Called when the First Person Perspective button is pressed
	void FPPPressed();
	//Called when the First Person Perspective button is released
	void FPPReleased();
	//Called when the Pause button is pressed
	void PausePressed();
	//Called when the crouch button is pressed
	void CrouchPressed();
	//Called when the crouch button is released
	void CrouchReleased();
	//Updates the player's rotation (used when crouching)
	void UpdateRotation(float NewHor, float NewVer);
	//Make the proper changes to allow crouching to prone functionality
	void PrepareProne();
	//Change the variables to start prone
	void StartProne();
	//Make the proper changes to allow prone to crouching functionality
	void PrepareCrouch();
	//When the prone animation is finished
	void FinishProne();
	//Handle the staggering mechanic
	void HandleStagger();
	//When the stagger mechanic has finished
	void FinishStagger();
	//Lowers the player's speed
	void Walk();
	//Resumes running by the player
	void StopWalking();

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);
	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	void MouseTurnX(float Rate);
	void MouseTurnY(float Rate);

	/** Called for forwards/backward input */
	void MoveForward(float Value);
	//Handle the vertical portion of crouching
	void HandleVerticalCrouch(float Value);
	//Handle the prone movement
	void HandleProneMovement(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);
	//Handle the horizontal portion of crouching
	void HandleHorizontalCrouch(float Value);
	//Handle the prone rotation
	void HandleProneRotation(float Value);

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCamera() const { return TopDownCamera; }
	 
};

