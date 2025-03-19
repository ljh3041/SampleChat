// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberBaseballState.h"
#include "Net/UnrealNetwork.h"

void ANumberBaseballState::SetScore(int32 NewHostScore, int32 NewGuestScore)
{
    HostScore = NewHostScore;

    GuestScore = NewGuestScore;
    OnRep_Score();  // 클라이언트에서 복제 처리
}

void ANumberBaseballState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Score 변수를 복제할 대상에 추가
    DOREPLIFETIME(ANumberBaseballState, GuestScore);

    // Score 변수를 복제할 대상에 추가
    DOREPLIFETIME(ANumberBaseballState, HostScore);
}