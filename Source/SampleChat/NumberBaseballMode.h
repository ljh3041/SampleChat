// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NumberBaseballMode.generated.h"

/**
 * 
 */
UCLASS()
class SAMPLECHAT_API ANumberBaseballMode : public AGameModeBase
{
	GENERATED_BODY()
	
private:
    FString SecretNumber;
    UPROPERTY(BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
    int32 HostAttempts;
    UPROPERTY(BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
    int32 GuestAttempts;
    UPROPERTY(BlueprintReadOnly, Category = "Game", meta = (AllowPrivateAccess = "true"))
    bool bHostTurn;

    FTimerHandle TurnTimerHandle; // �� ���ѽð� Ÿ�̸�
    const float TurnTimeLimit = 10.0f; // ���� �ð� (��)

    void HandleTurnTimeout(); // ���ѽð� �ʰ� ó�� �Լ�.




public:
    UPROPERTY(BlueprintReadOnly, Category = "Game")
    int32 HostWins;
    UPROPERTY(BlueprintReadOnly, Category = "Game")
    int32 GuestWins;

    UFUNCTION(BlueprintCallable, Category = "Game")
    int32 GetGuestScore() const;

    UFUNCTION(BlueprintCallable, Category = "Game")
    int32 GetHostScore() const;

    ANumberBaseballMode();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Game")
    void ProcessChatMessage(const FString& Message);

    void ResetGame();

    void UpdateScoreboard();

    void UpdateScore(int32 HostWin, int32 GuestWin);
};
