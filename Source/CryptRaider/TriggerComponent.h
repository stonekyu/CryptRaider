// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

/**
 * 트리거 역할을 하는 박스 컴포넌트 클래스입니다.
 * 특정 태그를 가진 액터가 충돌하면 Mover가 작동하도록 설정합니다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	// 생성자: 기본적으로 틱을 활성화합니다.
	UTriggerComponent();

protected:
	// 게임이 시작될 때 호출되는 함수
	virtual void BeginPlay() override;

public:
	// 매 프레임마다 호출되는 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	// Mover를 설정하는 함수 (Blueprint에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* Mover);

private:
	// 트리거가 반응할 액터의 태그 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere)
	FName AcceptableActorTag;

	// 움직임을 제어하는 Mover 객체 포인터
	UMover* Mover;

	// 충돌한 액터 중 조건을 만족하는 액터를 반환하는 함수
	AActor* GetAcceptableActor() const;
};
