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

    // 첫 턴 시작 시 타이머 설정
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::ProcessChatMessage(const FString& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("Secret Number: %s"), *SecretNumber);// 나 확인용 ㅎㅎ
    // 메시지에서 "Host" 또는 "Guest" 추출하기
    FString PlayerID, Guess;
    if (!Message.Split(":", &PlayerID, &Guess)) // 메시지 파싱
    {
        if (!Guess.StartsWith("/"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Invalid message! Use '/123'"));
            GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Invalid message! Use '/123'"));
            return;
        }
    }

    // 현재 턴의 플레이어가 맞는지 확인하기. 플레이어 아이디 추출한 값 및 호스트 턴 불 값으로 판단
    if ((bHostTurn && PlayerID != "Host") || (!bHostTurn && PlayerID != "Guest"))
    {
        UE_LOG(LogTemp, Warning, TEXT("It is not your turn!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("It is not your turn!"));
        return;
    }

    // 숫자만 남기기 (/ 제거)
    Guess = Guess.RightChop(1);

    // 유효한 숫자인지 확인
    if (!UMyGameLogic::IsValidGuess(Guess))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid Guess!"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Invalid Guess!"));
        return;
    }

    // 스트라이크 & 볼 판정
    int32 Strikes, Balls;
    UMyGameLogic::CalculateResult(SecretNumber, Guess, Strikes, Balls);
    UE_LOG(LogTemp, Warning, TEXT("%dS%dB"), Strikes, Balls);
    
    // 스트라이크와 볼 정보를 문자열로 만들기
    FString ResultMessage = FString::Printf(TEXT("%dS%dB"), Strikes, Balls);
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage);
    
    if (Strikes == 3) // 정답일 경우
    {
        UpdateScoreboard();


        UE_LOG(LogTemp, Warning, TEXT("%s Win! Restarting Game..."), *PlayerID);
        // 스트라이크와 볼 정보를 문자열로 만들기
        FString ResultMessage0 = FString::Printf(TEXT("%s Win! Restarting Game..."), *PlayerID);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage0);
        ResetGame();
        return;
    }

    // 시도 횟수 증가
    if (bHostTurn) HostAttempts++;
    else GuestAttempts++;

    // 무승부 조건 체크
    if (HostAttempts >= 3 && GuestAttempts >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Draw! Restarting Game..."));
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Draw! Restarting Game..."));
        ResetGame();
        return;
    }


    // 턴 교체
    bHostTurn = !bHostTurn;
    UE_LOG(LogTemp, Warning, TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));

    FString ResultMessage1 = FString::Printf(TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage1);


    GetWorldTimerManager().ClearTimer(TurnTimerHandle); // 기존 타이머 제거
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::ResetGame()
{
    SecretNumber = UMyGameLogic::GenerateRandomNumber();
    UE_LOG(LogTemp, Warning, TEXT("Secret Number: %s"), *SecretNumber); // 테스트용... 뭔지를 몰라서
    HostAttempts = 0;
    GuestAttempts = 0;
    bHostTurn = true;

    // 타이머 초기화 및 리셋
    GetWorldTimerManager().ClearTimer(TurnTimerHandle);           // 기존 타이머 제거

    // 새 턴 타이머 시작
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::HandleTurnTimeout()
{
    UE_LOG(LogTemp, Warning, TEXT("%s Time's up!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    FString ResultMessage2 = FString::Printf(TEXT("%s Time's up!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage2);
    // 시도 횟수 증가
    if (bHostTurn) HostAttempts++;
    else GuestAttempts++;

    // 무승부 조건 체크
    if (HostAttempts >= 3 && GuestAttempts >= 3)
    {
        UE_LOG(LogTemp, Warning, TEXT("Draw! Restarting Game..."));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Draw! Restarting Game..."));
        ResetGame();
        return;
    }

    // 턴 교체
    bHostTurn = !bHostTurn;
    UE_LOG(LogTemp, Warning, TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    FString ResultMessage3 = FString::Printf(TEXT("Now %s turn!"), bHostTurn ? TEXT("Host") : TEXT("Guest"));
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, *ResultMessage3);

    // 턴 변경 후 타이머 재설정
    // 다음 턴 제한시간 설정
    GetWorldTimerManager().SetTimer(TurnTimerHandle, this, &ANumberBaseballMode::HandleTurnTimeout, TurnTimeLimit, false);
}

void ANumberBaseballMode::UpdateScoreboard()
{
    // 승리자 처리
    if (bHostTurn)
    {
        HostWins++;
    }
    else
    {
        GuestWins++;
    }

    FString ScoreboardMessage = FString::Printf(TEXT("Host Wins: %d | Guest Wins: %d"), HostWins, GuestWins);

    // 뷰포트에 실시간 스코어 보드 표시 (3초 동안)
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
    // GameState에 접근하여 SetScore 호출
    ANumberBaseballState* NumberBaseballState = GetGameState<ANumberBaseballState>();
    if (NumberBaseballState)
    {
        NumberBaseballState->SetScore(HostWin, GuestWin);  // 스코어 변경
    }
}