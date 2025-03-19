// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NumberBaseballState.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ANumberBaseballState : public AGameStateBase
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Score, Category = "Game")
    int32 HostScore;

    UPROPERTY(BlueprintReadOnly, Replicated, ReplicatedUsing = OnRep_Score, Category = "Game")
    int32 GuestScore;

    UFUNCTION()
    void OnRep_Score()
    {
        // Ŭ���̾�Ʈ���� ���ھ� ����� �� �۾� ó��
        UE_LOG(LogTemp, Log, TEXT("Score has been updated"));
    }

    void SetScore(int32 NewHostScore, int32 NewGuestScore);
};
