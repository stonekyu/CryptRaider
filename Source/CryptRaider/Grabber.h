// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"

/**
 * UGrabber 클래스는 물체를 집고 놓을 수 있도록 하는 컴포넌트입니다.
 * PhysicsHandle을 활용하여 일정 거리 내의 물체를 감지하고 집는 기능을 제공합니다.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRYPTRAIDER_API UGrabber : public USceneComponent
{
	GENERATED_BODY()

public:	
	// 생성자: 기본값을 설정합니다.
	UGrabber();

protected:
	// 게임이 시작될 때 호출되는 함수
	virtual void BeginPlay() override;

public:	
	// 매 프레임마다 호출되는 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 물체를 집는 함수 (Blueprint에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void Grab();
	
	// 집고 있던 물체를 놓는 함수 (Blueprint에서 호출 가능)
	UFUNCTION(BlueprintCallable)
	void Release();

private:
	// 최대 잡을 수 있는 거리 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 200;
	
	// 물체 감지 반경 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere)
	float GrabRadius = 100;

	// 물체를 잡은 후 유지할 거리 (에디터에서 수정 가능)
	UPROPERTY(EditAnywhere)
	float HoldDistance = 150;

	// UPhysicsHandleComponent를 가져오는 함수
	UPhysicsHandleComponent* GetPhysicsHandle() const;

	// 지정된 거리 내에서 잡을 수 있는 액터를 찾는 함수
	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};
