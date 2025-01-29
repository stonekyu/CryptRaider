// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"

/**
 * UMover 클래스는 액터를 일정한 거리만큼 이동시키는 기능을 담당하는 컴포넌트입니다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:
	// 생성자: 기본값을 설정합니다.
	UMover();

protected:
	// 게임 시작 시 호출되는 함수
	virtual void BeginPlay() override;

public:
	// 매 프레임 호출되는 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 이동 여부를 설정하는 함수
	void SetShouldMove(bool ShouldMove);

private:
	// 이동할 거리 벡터 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	// 이동에 걸리는 시간 (초 단위, 에디터에서 수정 가능)
	UPROPERTY(EditAnywhere)
	float MoveTime = 4;

	// 이동 여부를 결정하는 변수 (기본값: false)
	bool ShouldMove = false;

	// 초기 위치 저장 변수
	FVector OriginalLocation;
};
