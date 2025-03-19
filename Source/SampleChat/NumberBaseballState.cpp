// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberBaseballState.h"
#include "Net/UnrealNetwork.h"

void ANumberBaseballState::SetScore(int32 NewHostScore, int32 NewGuestScore)
{
    HostScore = NewHostScore;

    GuestScore = NewGuestScore;
    OnRep_Score();  // Ŭ���̾�Ʈ���� ���� ó��
}

void ANumberBaseballState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Score ������ ������ ��� �߰�
    DOREPLIFETIME(ANumberBaseballState, GuestScore);

    // Score ������ ������ ��� �߰�
    DOREPLIFETIME(ANumberBaseballState, HostScore);
}