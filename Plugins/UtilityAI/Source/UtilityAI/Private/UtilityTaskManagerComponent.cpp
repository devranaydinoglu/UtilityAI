// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityTaskManagerComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "UtilityTask.h"

// Sets default values for this component's properties
UUtilityTaskManagerComponent::UUtilityTaskManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	bStartTimer = true;

}

// Called when the game starts
void UUtilityTaskManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void UUtilityTaskManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DecreaseCooldowns(DeltaTime);
}


// Initialize component
void UUtilityTaskManagerComponent::Initialize()
{
	// Activate UTM
	ActivateUTM();
}

// Set timer or unpause timer handle
void UUtilityTaskManagerComponent::ActivateUTM()
{
	ConstructTasks();
	UWorld* World = GetWorld();

	// Start UTM timer or unpause UTM timer
	if (bStartTimer)
	{
		World->GetTimerManager().SetTimer(ChooseTaskTimerHandle, this, &UUtilityTaskManagerComponent::ChooseTask, ChooseTaskInterval, true);
		bStartTimer = false;
	}
	else
	{
		World->GetTimerManager().UnPauseTimer(ChooseTaskTimerHandle);
	}
}

// Pause timer handle
void UUtilityTaskManagerComponent::DeactivateUTM()
{
	// Pause UTM timer
	GetWorld()->GetTimerManager().PauseTimer(ChooseTaskTimerHandle);
}

// Construct utility tasks
void UUtilityTaskManagerComponent::ConstructTasks()
{	
	// Construct task objects
	if (TaskTypes.Num() > 0)
	{
		for (TSubclassOf<UUtilityTask> TaskType : TaskTypes)
		{
			if (TaskType)
			{
				UUtilityTask* NewTask = NewObject<UUtilityTask>(GetOwner(), TaskType);
				NewTask->Construct();
				Tasks.Add(NewTask);
			}
		}
	}
}

// Evaluate/Score utility tasks and execute winning task
void UUtilityTaskManagerComponent::ChooseTask()
{
	TaskScores.Empty();

	// Score all tasks
	if (Tasks.Num() > 0)
	{
		for (UUtilityTask* Task : Tasks)
		{
			if (!Cooldowns.Contains(Task->GetClass()))
			{
				TaskScores.Add(Task->Score());
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, Task->GetClass()->GetDisplayNameText().ToString());
			}
		}

		if (TaskScores.Num() > 0)
		{
			int32 MaxValueIndex;
			float MaxValue;

			// Get winning task
			UKismetMathLibrary::MaxOfFloatArray(TaskScores, MaxValueIndex, MaxValue);
			ActiveTask = Tasks[MaxValueIndex];

			// Execute winning task
			ActiveTask->Start();
		}
	}
}

// Set a cooldown for specified task
void UUtilityTaskManagerComponent::SetCooldown(TSubclassOf<UUtilityTask> Task, float CooldownDuration, bool bAddToExistingCooldown, bool bResetCooldown)
{
	// Add a new cooldown or add onto existing cooldown if specified
	if (Cooldowns.Contains(Task))
	{
		if (bAddToExistingCooldown)
		{
			float* CurrentCooldown = Cooldowns.Find(Task);
			*CurrentCooldown = *CurrentCooldown + CooldownDuration;
		}
		else if (bResetCooldown)
		{
			float* CurrentCooldown = Cooldowns.Find(Task);
			*CurrentCooldown = CooldownDuration;
		}
	}
	else
	{
		Cooldowns.Add(Task, CooldownDuration);
	}
}

// Decrease current cooldown times every tick
void UUtilityTaskManagerComponent::DecreaseCooldowns(float DeltaTime)
{
	if (Cooldowns.Num() > 0)
	{
		for (auto& Cooldown : Cooldowns)
		{
			Cooldown.Value = Cooldown.Value - DeltaTime;
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::SanitizeFloat(Cooldown.Value));
			if (Cooldown.Value <= 0.0f)
			{
				Cooldowns.Remove(Cooldown.Key);
			}
		}
	}
}
