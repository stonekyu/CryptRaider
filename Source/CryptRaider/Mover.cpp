// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// 생성자: 기본적으로 틱을 활성화함
UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true; // 매 프레임 TickComponent 실행
}

// 게임 시작 시 호출되는 함수
void UMover::BeginPlay()
{
	Super::BeginPlay();

	// 초기 위치 저장
	OriginalLocation = GetOwner()->GetActorLocation();
}

// 매 프레임마다 호출되는 함수
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 목표 위치 설정 (이동할 위치 또는 원래 위치)
	FVector TargetLocation = OriginalLocation;
	if (ShouldMove)
	{
		TargetLocation = OriginalLocation + MoveOffset;
	}

	// 현재 위치 가져오기
	FVector CurrentLocation = GetOwner()->GetActorLocation();
	
	// 속도 계산: 이동할 거리 / 이동 시간
	float Speed = MoveOffset.Length() / MoveTime;

	// 현재 위치에서 목표 위치로 일정한 속도로 보간 이동
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);

	// 새로운 위치 적용
	GetOwner()->SetActorLocation(NewLocation);
}

// 이동 여부를 설정하는 함수
void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}
