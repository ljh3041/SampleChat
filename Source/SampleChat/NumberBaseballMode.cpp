// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberBaseballMode.h"
#include "MyGameLogic.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "NumberBaseballState.h"

ANumberBaseballMode::ANumberBaseballMode()
{
    HostAttempts = 0;
    GuestAttempts = 0;
    bHostTurn = true;
    HostWins = 0;
    GuestWins = 0;
}

void ANumberBaseballMode::BeginPlay()
{
    Super::BeginPlay();
    SecretNumber = UMyGameLogic::GenerateRandomNumber();
    UE_LOG(LogTemp, Warning, TEXT("Secret Number: %s"), *SecretNumber);

    // ù �� ���� �� Ÿ�̸� ����
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::ProcessChatMessage(const FString& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("Secret Number: %s"), *SecretNumber);// �� Ȯ�ο� ����
    // �޽������� "Host" �Ǵ� "Guest" �����ϱ�
    FString PlayerID, Guess;
    if (!Message.Split(":", &PlayerID, &Guess)) // �޽��� �Ľ�
    {
        if (!Guess.StartsWith("/"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid message! Use '/123'"));
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Invalid message! Use '/123'"));
            return;
        }
    }

    // ���� ���� �÷��̾ �´��� Ȯ���ϱ�. �÷��̾� ���̵� ������ �� �� ȣ��Ʈ �� �� ������ �Ǵ�
    if ((bHostTurn && PlayerID != "Host") || (!bHostTurn && PlayerID != "Guest"))
    {
        UE_LOG(LogTemp, Warning, TEXT("It is not your turn!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("It is not your turn!"));
        return;
    }

    // ���ڸ� ����� (/ ����)
    Guess = Guess.RightChop(1);

    // ��ȿ�� �������� Ȯ��
    if (!UMyGameLogic::IsValidGuess(Guess))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Guess!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Invalid Guess!"));
        return;
    }

    // ��Ʈ����ũ & �� ����
    int32 Strikes, Balls;
    UMyGameLogic::CalculateResult(SecretNumber, Guess, Strikes, Balls);
    UE_LOG(LogTemp, Warning, TEXT("%dS%dB"), Strikes, Balls);
    
    // ��Ʈ����ũ�� �� ������ ���ڿ��� �����
    FString ResultMessage = FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage);
    
    if (Strikes == 3) // ������ ���
    {
        UpdateScoreboard();


        UE_LOG(LogTemp, Warning, TEXT("%s Win! Restarting Game..."), *PlayerID);
        // ��Ʈ����ũ�� �� ������ ���ڿ��� �����
        FString ResultMessage0 = FString::Printf(TEXT("%s Win! Restarting Game..."), *PlayerID);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage0);
        ResetGame();
        return;
    }

    // �õ� Ƚ�� ����
    if (bHostTurn) HostAttempts++;
    else GuestAttempts++;

    // ���º� ���� üũ
    if (HostAttempts >= 3 && GuestAttempts >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Draw! Restarting Game..."));
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Draw! Restarting Game..."));
        ResetGame();
        return;
    }


    // �� ��ü
    bHostTurn = !bHostTurn;
    UE_LOG(LogTemp, Warning, TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));

    FString ResultMessage1 = FString::Printf(TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage1);


    GetWorldTimerManager().ClearTimer(TurnTimerHandle); // ���� Ÿ�̸� ����
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::ResetGame()
{
    SecretNumber = UMyGameLogic::GenerateRandomNumber();
    UE_LOG(LogTemp, Warning, TEXT("Secret Number: %s"), *SecretNumber); // �׽�Ʈ��... ������ ����
    HostAttempts = 0;
    GuestAttempts = 0;
    bHostTurn = true;

    // Ÿ�̸� �ʱ�ȭ �� ����
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);           // ���� Ÿ�̸� ����

    // �� �� Ÿ�̸� ����
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::HandleTurnTimeout()
{
    UE_LOG(LogTemp, Warning, TEXT("%s Time's up!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    FString ResultMessage2 = FString::Printf(TEXT("%s Time's up!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage2);
    // �õ� Ƚ�� ����
    if (bHostTurn) HostAttempts++;
    else GuestAttempts++;

    // ���º� ���� üũ
    if (HostAttempts >= 3 && GuestAttempts >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Draw! Restarting Game..."));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Draw! Restarting Game..."));
        ResetGame();
        return;
    }

    // �� ��ü
    bHostTurn = !bHostTurn;
    UE_LOG(LogTemp, Warning, TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    FString ResultMessage3 = FString::Printf(TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage3);

    // �� ���� �� Ÿ�̸� �缳��
    // ���� �� ���ѽð� ����
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::UpdateScoreboard()
{
    // �¸��� ó��
    if (bHostTurn)
    {
        HostWins++;
    }
    else
    {
        GuestWins++;
    }

    FString ScoreboardMessage = FString::Printf(TEXT("Host Wins: %d | Guest Wins: %d"), HostWins, GuestWins);

    // ����Ʈ�� �ǽð� ���ھ� ���� ǥ�� (3�� ����)
    GEngine->AddOnScreenDebugMessage(-1, 7.0f, FColor::Blue, *ScoreboardMessage);
    UpdateScore(HostWins, GuestWins);
}

int32 ANumberBaseballMode::GetGuestScore() const
{
    return GuestWins; 
}

int32 ANumberBaseballMode::GetHostScore() const
{
    return HostWins;
}




void ANumberBaseballMode::UpdateScore(int32 HostWin, int32 GuestWin)
{
    // GameState�� �����Ͽ� SetScore ȣ��
    ANumberBaseballState* NumberBaseballState = GetGameState<ANumberBaseballState>();
    if (NumberBaseballState)
    {
        NumberBaseballState->SetScore(HostWin, GuestWin);  // ���ھ� ����
    }
}