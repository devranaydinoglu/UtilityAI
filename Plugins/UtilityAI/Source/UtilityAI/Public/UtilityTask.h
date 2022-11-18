// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UtilityTask.generated.h"


UCLASS(Blueprintable)
class UTILITYAI_API UUtilityTask : public UObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UUtilityTask();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Construct();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float Score();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Start();

	virtual bool Construct_Implementation();
	virtual float Score_Implementation();
	virtual bool Start_Implementation();

};
