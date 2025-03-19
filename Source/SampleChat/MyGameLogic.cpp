// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameLogic.h"


FString UMyGameLogic::GenerateRandomNumber()
{
    TArray<int32> Numbers = { 1,2,3,4,5,6,7,8,9 };
    if (Numbers.Num() > 0)
    {
        int RandomIndex = FMath::RandRange(0, Numbers.Num() - 1);
    }

    FString Result;
    for (int i = 0; i < 3; i++)
    {
        int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
        Result.Append(FString::FromInt(Numbers[Index]));
        Numbers.RemoveAt(Index);
    }
    return Result;
}

bool UMyGameLogic::IsValidGuess(const FString& Guess)
{
    if (Guess.Len() != 3) return false;
    for (TCHAR Char : Guess)
    {
        if (!FChar::IsDigit(Char) || Char == '0') return false;
    }
    return true;
}

void UMyGameLogic::CalculateResult(const FString& Secret, const FString& Guess, int32& OutStrikes, int32& OutBalls)
{
    OutStrikes = 0;
    OutBalls = 0;
    for (int32 i = 0; i < 3; i++)
    {
        if (Guess[i] == Secret[i]) OutStrikes++;
        else if (Secret.Contains(Guess.Mid(i, 1))) OutBalls++;
    }
}
