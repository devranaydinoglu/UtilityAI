// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UtilityTaskManagerComponent.generated.h"

class UUtilityTask;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTILITYAI_API UUtilityTaskManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUtilityTaskManagerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Time between choosing task
	UPROPERTY(EditAnywhere)
	float ChooseTaskInterval;

	// Tasks that will be taken into consideration when choosing an executable task
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<UUtilityTask>> TaskTypes;

	// Current active task
	UPROPERTY(BlueprintReadOnly)
	UUtilityTask* ActiveTask;

	// Task objects
	UPROPERTY()
	TArray<UUtilityTask*> Tasks;

	// Used to start timer once during initialization
	bool bStartTimer;

	FTimerHandle ChooseTaskTimerHandle;
	
	// Holds each task's score
	TArray<float> TaskScores;

	// Holds current active cooldowns for each task
	TMap<TSubclassOf<UUtilityTask>, float> Cooldowns;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Construct utility tasks
	void ConstructTasks();

	// Evaluate/Score utility tasks and execute winning task
	void ChooseTask();

	// Set a cooldown for specified task
	UFUNCTION(BlueprintCallable)
	void SetCooldown(TSubclassOf<UUtilityTask> Task, float CooldownDuration, bool bAddToExistingCooldown, bool bResetCooldown);

	// Decrease current cooldown times every tick
	void DecreaseCooldowns(float DeltaTime);

	// Initialize component
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Activate UTM"))
	void Initialize();

	// Set timer or unpause timer handle
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Construct Tasks.\nIf UTM timer isn't active yet, start timer. If UTM timer is active, unpause timer"))
	void ActivateUTM();

	// Pause timer handle
	UFUNCTION(BlueprintCallable, meta = (ToolTip = "Pause UTM timer"))
	void DeactivateUTM();
};
