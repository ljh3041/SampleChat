// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyGameLogic.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API UMyGameLogic : public UObject
{
	GENERATED_BODY()
public:
	static FString GenerateRandomNumber();
	static bool IsValidGuess(const FString& Guess);
	static void CalculateResult(const FString& Secret, const FString& Guess, int32& OutStrikes, int32& OutBalls);
	
};
